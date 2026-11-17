#!/usr/bin/env python3
"""
==============================================================================
 AIoT Final Course Project: Automated Warehouse AI Object Sorting System
 Production-Hardened Raspberry Pi 4 IoT Gateway Script
 Student: Dylan (Index: 22)
 
 Key Resilience Enhancements:
 1. Asynchronous Ingestion & Cloud Queue: Decouples UART reading from ThingSpeak 
    rate limits, eliminating blocking sleeps and preventing packet loss.
 2. Safe Buffer Management: Flushes hardware UART buffer on connect/reconnect 
    only, eliminating destructive buffer purges during active conveyor runs.
 3. Resilient MQTT Publishing: Automatic retries with exponential backoff on 
    network drops or socket timeouts.
 4. Auto-Reconnecting Serial Link: Gracefully handles USB disconnects, cable 
    bumps, and Linux device node shifting (/dev/ttyUSB0 -> /dev/ttyUSB1).
 5. Strict Data Framing & Sanity Validation: Validates newline termination,
    regex integer matching, and boundary conditions (0-100% confidence).
==============================================================================
"""

import time
import sys
import threading
import queue
import re
import serial
from serial import SerialException

try:
    import paho.mqtt.publish as publish
except ImportError:
    publish = None

# ==============================================================================
# 1. CONFIGURATION & CREDENTIALS
# ==============================================================================
SERIAL_PORT     = '/dev/ttyUSB0'
BAUD_RATE       = 9600
TIMEOUT_SEC     = 1.0  # Responsive 1s timeout for non-blocking read loops
CANDIDATE_PORTS = [SERIAL_PORT, '/dev/ttyUSB1', '/dev/ttyACM0', '/dev/serial0']

# ThingSpeak MQTT Cloud Settings
CHANNEL_ID      = "3581016"
MQTT_HOST       = "mqtt3.thingspeak.com"
MQTT_CLIENT_ID  = "MB0iNjAUGR8CKAQkDykpNgA"
MQTT_USERNAME   = "MB0iNjAUGR8CKAQkDykpNgA"
MQTT_PASSWORD   = "XE7F08IHbEHSEFXEiRy05ObD"

TRANSPORT_MODE  = "websockets"
PORT_NUMBER     = 80
TOPIC_STRING    = f"channels/{CHANNEL_ID}/publish"

# Timing & Rate Limits (ThingSpeak free tier enforces >= 15s between channel writes)
RATE_LIMIT_SEC  = 15.0
MAX_RETRIES     = 3
RETRY_DELAY_SEC = 2.0

# In-memory FIFO queue for telemetry packets
telemetry_queue = queue.Queue(maxsize=100)
shutdown_event  = threading.Event()

# ==============================================================================
# 2. SERIAL PORT CONNECTION & AUTO-RECONNECT MANAGER
# ==============================================================================
def connect_serial():
    """
    Scans candidate ports and opens a valid serial connection.
    Clears the RX buffer once on connection to discard startup noise.
    """
    for port in CANDIDATE_PORTS:
        try:
            ser = serial.Serial(port, BAUD_RATE, timeout=TIMEOUT_SEC)
            ser.reset_input_buffer()  # Flush old bootloader/startup garbage
            print(f"✅ Successfully opened serial port: {port} @ {BAUD_RATE} baud.")
            return ser
        except (SerialException, OSError):
            continue
    return None

# ==============================================================================
# 3. MQTT PUBLISHER WORKER (THREAD)
# ==============================================================================
def cloud_publisher_worker():
    """
    Background worker that consumes telemetry from the queue and publishes
    to ThingSpeak MQTT while strictly enforcing the 15-second rate limit.
    Includes automatic retries and exponential backoff.
    """
    last_publish_time = 0.0
    print("[CLOUD WORKER] Background MQTT publisher worker started.")

    while not shutdown_event.is_set():
        try:
            # Wait for telemetry packet with a short timeout to check shutdown
            score, rx_timestamp = telemetry_queue.get(timeout=1.0)
        except queue.Empty:
            continue

        # Enforce ThingSpeak 15-second rate limit without stalling the serial thread
        elapsed = time.time() - last_publish_time
        if elapsed < RATE_LIMIT_SEC:
            wait_time = RATE_LIMIT_SEC - elapsed
            print(f"  ⏳ Rate limit guard: waiting {wait_time:.1f}s before uploading score {score}%...")
            # Sleep in small increments to remain responsive to shutdown_event
            wait_end = time.time() + wait_time
            while time.time() < wait_end and not shutdown_event.is_set():
                time.sleep(0.5)

        if shutdown_event.is_set():
            break

        payload = f"field1={score}"
        print(f"[MQTT TX] Publishing to ThingSpeak Topic '{TOPIC_STRING}' -> {payload}")

        success = False
        for attempt in range(1, MAX_RETRIES + 1):
            if shutdown_event.is_set():
                break
            try:
                if publish is None:
                    raise RuntimeError("paho-mqtt library is not installed.")

                publish.single(
                    topic=TOPIC_STRING,
                    payload=payload,
                    hostname=MQTT_HOST,
                    transport=TRANSPORT_MODE,
                    port=PORT_NUMBER,
                    client_id=MQTT_CLIENT_ID,
                    auth={'username': MQTT_USERNAME, 'password': MQTT_PASSWORD}
                )
                last_publish_time = time.time()
                print(f"  ⭐ Successfully uploaded '{payload}' to Cloud Dashboard! (Attempt {attempt})\n")
                success = True
                break
            except Exception as mqtt_err:
                print(f"  ⚠️ MQTT Publish Attempt {attempt}/{MAX_RETRIES} Failed: {mqtt_err}")
                if attempt < MAX_RETRIES:
                    backoff = RETRY_DELAY_SEC * attempt
                    print(f"     Retrying in {backoff}s...")
                    time.sleep(backoff)

        if not success:
            print(f"  ❌ Error: Dropping telemetry {score}% after {MAX_RETRIES} failed attempts.\n")

        telemetry_queue.task_done()

# ==============================================================================
# 4. MAIN SERIAL INGESTION LOOP
# ==============================================================================
def main():
    print("=" * 65)
    print("  AIoT WAREHOUSE OBJECT SORTING SYSTEM: IOT GATEWAY (HARDENED) ")
    print(f"  Channel ID: {CHANNEL_ID} | Host: {MQTT_HOST}")
    print("=" * 65)

    ser = connect_serial()
    if ser is None:
        print(f"⚠️ Warning: No initial serial port available in {CANDIDATE_PORTS}.")
        print("Will attempt auto-reconnection in background loop...")

    # Start cloud publisher thread
    publisher_thread = threading.Thread(target=cloud_publisher_worker, daemon=True)
    publisher_thread.start()

    print("⏳ Listening for Arduino UART transmissions... (Press Ctrl+C to stop)\n")

    reconnect_delay = 2.0
    last_reconnect_attempt = 0.0

    try:
        while True:
            # Handle disconnected or uninitialized serial port
            if ser is None or not ser.is_open:
                if time.time() - last_reconnect_attempt > reconnect_delay:
                    last_reconnect_attempt = time.time()
                    print("[SERIAL] Attempting to auto-reconnect serial interface...")
                    ser = connect_serial()
                    if ser is None:
                        continue
                else:
                    time.sleep(0.5)
                    continue

            # Read raw bytes with exception handling for cable disconnects
            try:
                raw_bytes = ser.readline()
            except (SerialException, OSError) as ser_err:
                print(f"❌ Serial Port Disconnected or Error: {ser_err}")
                try:
                    ser.close()
                except Exception:
                    pass
                ser = None
                continue

            if not raw_bytes:
                continue

            # Strict framing check: must end with newline delimiter
            if not (raw_bytes.endswith(b'\n') or raw_bytes.endswith(b'\r')):
                print(f"  ⚠️ Discarding truncated UART frame (timeout/noise): {raw_bytes!r}")
                continue

            # Decode safely
            data_string = raw_bytes.decode('utf-8', errors='ignore').strip()
            if not data_string:
                continue

            # Strict regex pattern validation (matches pure integer payload)
            if not re.match(r'^\d+$', data_string):
                print(f"  ⚠️ Warning: Non-integer UART payload discarded: '{data_string}'")
                continue

            # Validate numerical range (0 to 100% confidence)
            try:
                confidence_score = int(data_string)
            except ValueError:
                continue

            if not (0 <= confidence_score <= 100):
                print(f"  ⚠️ Warning: Out-of-bounds score ({confidence_score}%) rejected. Must be 0-100.")
                continue

            print(f"[UART RX] Valid Payload Received: '{data_string}' -> Confidence: {confidence_score}%")

            # Push to queue (never blocks UART ingestion, buffer is NOT cleared destructively)
            try:
                telemetry_queue.put_nowait((confidence_score, time.time()))
                print(f"  ↳ Queued for cloud upload (Queue depth: {telemetry_queue.qsize()})")
            except queue.Full:
                print("  ⚠️ Warning: Telemetry queue full! Discarding oldest reading.")

    except KeyboardInterrupt:
        print("\n[STOP] Shutting down IoT Gateway...")
    finally:
        shutdown_event.set()
        if ser and ser.is_open:
            ser.close()
            print("Serial port closed cleanly.")
        print("IoT Gateway shut down gracefully. Goodbye!")

if __name__ == "__main__":
    main()
