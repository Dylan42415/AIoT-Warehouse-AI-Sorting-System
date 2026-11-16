#!/usr/bin/env python3
"""
Lab 06: Update ThingSpeak Channel from SBC via MQTT
Configured with Student Credentials
Channel ID: 3520288
"""
import time
import paho.mqtt.publish as publish

# --- Configuration ---
channel_ID = "3520288"
mqtt_host  = "mqtt3.thingspeak.com"

# MQTT Device Credentials
mqtt_client_ID = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_username  = "LAIyFCQEOjoLEigXAS0EMjM"
mqtt_password  = "itEEIw2lk2UEz6sgPURuVfq1"

# Using WebSockets on port 80 (Bypasses school firewall restrictions)
t_transport = "websockets"
t_port = 80

# Topic to publish data
topic = f"channels/{channel_ID}/publish"

print("====================================================")
print(f"Starting ThingSpeak MQTT Publisher...")
print(f"Target Channel ID: {channel_ID}")
print(f"MQTT Broker: {mqtt_host} on Port {t_port} ({t_transport})")
print("====================================================\n")

sample_val = 123

while True:
    payload = f"field1={sample_val}"
    
    print(f"Publishing Payload: '{payload}' to {topic}...")
    try:
        publish.single(
            topic,
            payload=payload,
            hostname=mqtt_host,
            transport=t_transport,
            port=t_port,
            client_id=mqtt_client_ID,
            auth={'username': mqtt_username, 'password': mqtt_password}
        )
        print("  -> Success! Data sent to ThingSpeak.\n")
    except Exception as e:
        print(f"  -> Error publishing: {e}\n")
    
    # ThingSpeak free accounts allow updates every 15-20 seconds
    time.sleep(20)
