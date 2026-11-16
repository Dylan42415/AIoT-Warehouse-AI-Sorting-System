#!/usr/bin/env python3
"""
==============================================================================
 AIoT Final Course Project: Automated Warehouse AI Object Sorting System
 Raspberry Pi 4 IoT Gateway Script
 Student: Dylan (Index: 22)
 
 Functionality:
 1. Reads UART telemetry from Arduino Nano 33 BLE via /dev/ttyUSB0 (9600 baud).
 2. Parses product detection confidence and sorting decision.
 3. Publishes live telemetry to ThingSpeak Cloud Platform using MQTT over WebSockets.
==============================================================================
"""

import time
import serial
import paho.mqtt.publish as publish
import sys

# ==============================================================================
# 1. CONFIGURATION & CREDENTIALS
# ==============================================================================
SERIAL_PORT = '/dev/ttyUSB0'
BAUD_RATE   = 9600
TIMEOUT_SEC = 2

# ThingSpeak MQTT Cloud Settings
CHANNEL_ID     = "3561948"
MQTT_HOST      = "mqtt3.thingspeak.com"
MQTT_CLIENT_ID = "FCwmDyoGDTwwDDIABy0DASQ"
MQTT_USERNAME  = "FCwmDyoGDTwwDDIABy0DASQ"
MQTT_PASSWORD  = "L+2fafL1vu+9vTjiUNB6/wri"

TRANSPORT_MODE = "websockets"
PORT_NUMBER    = 80
TOPIC_STRING   = f"channels/{CHANNEL_ID}/publish"

# ==============================================================================
# 2. INITIALIZE SERIAL CONNECTION
# ==============================================================================
print("=" * 65)
print("  AIoT WAREHOUSE OBJECT SORTING SYSTEM: IOT GATEWAY  ")
print(f"  Channel ID: {CHANNEL_ID} | Host: {MQTT_HOST}")
print("=" * 65)

# Candidate ports for Raspberry Pi (USB adapter, ACM, or native UART)
CANDIDATE_PORTS = [SERIAL_PORT, '/dev/ttyUSB1', '/dev/ttyACM0', '/dev/serial0']
ser = None

for port in CANDIDATE_PORTS:
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=TIMEOUT_SEC)
        print(f"✅ Successfully opened serial port: {port} @ {BAUD_RATE} baud.")
        break
    except Exception:
        continue

if ser is None:
    print(f"❌ Error: Could not open any serial port in {CANDIDATE_PORTS}")
    print("Please verify the Grove-to-USB cable is plugged into the Raspberry Pi.")
    sys.exit(1)

print("⏳ Listening for Arduino UART transmissions... (Press Ctrl+C to stop)\n")

# Rate limit tracking (ThingSpeak free tier requires >= 15s between channel updates)
RATE_LIMIT_SEC    = 15.0
last_publish_time = 0.0

# ==============================================================================
# 3. MAIN GATEWAY LOOP
# ==============================================================================
try:
    while True:
        try:
            # Read line from Arduino
            raw_bytes = ser.readline()
            if not raw_bytes:
                continue

            # Decode and clean string
            data_string = raw_bytes.decode('utf-8', errors='ignore').strip()
            
            if data_string:
                print(f"[UART RX] Raw Payload from Arduino: '{data_string}'")
                
                # Parse integer score with strict validation
                try:
                    confidence_score = int(data_string)
                except ValueError:
                    print(f"  ⚠️ Warning: Non-numeric UART payload discarded: '{data_string}'")
                    continue
                
                print(f"  ↳ Parsed Confidence Score: {confidence_score}%")
                
                # Check ThingSpeak 15-second rate limit non-blockingly
                current_time = time.time()
                elapsed = current_time - last_publish_time
                if elapsed < RATE_LIMIT_SEC:
                    wait_time = int(RATE_LIMIT_SEC - elapsed)
                    print(f"  ⏳ Rate limit guard: {wait_time}s remaining before next allowed cloud upload.")
                    print("  Holding telemetry until window opens...\n")
                    time.sleep(wait_time + 0.5)

                # Build ThingSpeak MQTT Payload
                # field1 = Product Score / Confidence Level
                payload = f"field1={confidence_score}"
                
                print(f"[MQTT TX] Publishing to ThingSpeak Topic '{TOPIC_STRING}'...")
                
                try:
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
                    print(f"  ⭐ Successfully uploaded payload '{payload}' to Cloud Dashboard!\n")
                    
                    # Flush old buffered bytes to guarantee next reading is real-time
                    ser.reset_input_buffer()

                except Exception as mqtt_err:
                    print(f"  ⚠️ MQTT Publish Error: {mqtt_err}\n")

        except Exception as loop_err:
            print(f"Loop warning: {loop_err}")
            time.sleep(1)

except KeyboardInterrupt:
    print("\n[STOP] Shutting down IoT Gateway...")
finally:
    ser.close()
    print("Serial port closed. Goodbye!")
