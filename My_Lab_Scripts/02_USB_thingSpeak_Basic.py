#!/usr/bin/env python3
"""
Lab 07 / Lab 08: Arduino to SBC Communication via USB Cable & Cloud Push
Reads Arduino Serial Data and Publishes to ThingSpeak MQTT
"""
import time
import serial
import paho.mqtt.publish as publish

# --- Configuration ---
channel_ID = "3520288"
mqtt_host  = "mqtt3.thingspeak.com"

# MQTT Device Credentials
mqtt_client_ID = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_username  = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_password  = "itEEIw2lk2UEz6sgPURuVfq1"

t_transport = "websockets"
t_port = 80
topic = f"channels/{channel_ID}/publish"

# Connect to Arduino via USB Serial port
# On Raspberry Pi, the Arduino is usually /dev/ttyACM0 or /dev/ttyUSB0
serial_port = '/dev/ttyACM0'
baud_rate = 9600

print(f"Connecting to Arduino on {serial_port} at {baud_rate} baud...")
try:
    serial1 = serial.Serial(serial_port, baud_rate, timeout=2)
    print("Serial port connected successfully!\n")
except Exception as e:
    print(f"Could not open {serial_port}, trying /dev/ttyUSB0...")
    try:
        serial_port = '/dev/ttyUSB0'
        serial1 = serial.Serial(serial_port, baud_rate, timeout=2)
        print("Serial port connected on /dev/ttyUSB0!\n")
    except Exception as e2:
        print(f"Error opening serial port: {e2}")
        serial1 = None

while True:
    if serial1 and serial1.in_waiting > 0:
        try:
            raw_line = serial1.readline().decode('utf-8', errors='ignore').strip()
            if raw_line:
                sensor_score = int(raw_line)
                print(f"Received from Arduino: {sensor_score}")
                
                # Check threshold (e.g. if > 50, send to cloud)
                if sensor_score > 50:
                    payload = f"field1={sensor_score}"
                    print(f"Publishing to ThingSpeak: '{payload}'...")
                    
                    publish.single(
                        topic,
                        payload=payload,
                        hostname=mqtt_host,
                        transport=t_transport,
                        port=t_port,
                        client_id=mqtt_client_ID,
                        auth={'username': mqtt_username, 'password': mqtt_password}
                    )
                    print("  -> Published successfully!\n")
                    time.sleep(15)  # Rate limit for cloud
        except ValueError:
            pass
        except Exception as err:
            print(f"Publish error: {err}")
    time.sleep(1)
