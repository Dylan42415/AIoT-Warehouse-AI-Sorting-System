# Lab 06: Update ThingSpeak Channel from SBC by MQTT

This document provides the exhaustive laboratory execution protocol for **Lab 06: Update ThingSpeak Channel from SBC by MQTT** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the [[Protocol-MQTT-Message-Queuing-Telemetry|MQTT protocol]] publish/subscribe paradigm.
2. Configure a ThingSpeak MQTT Device and retrieve MQTT credentials.
3. Install the `paho-mqtt` and `psutil` Python packages on the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]].
4. Implement a Python script to sample SBC system metrics and publish them to the ThingSpeak MQTT broker.

---

## 2. Setting Up ThingSpeak MQTT Device

1. In ThingSpeak, navigate to **Devices** > **MQTT Devices** > click **Add a new device**.
2. Assign a device name (e.g., `RaspberryPi_Gateway`).
3. Authorize the device for your target Channel ID.
4. Click **Add Device** and save the generated credentials:
   - **Client ID** (24-character hex string)
   - **Username** (24-character hex string)
   - **Password** (Secret key)

---

## 3. SBC Python Implementation

Install dependencies:
```bash
pip3 install paho-mqtt psutil
```

Execute Python script (`01_thingSpeak_Basic.py`):
```python
#! /usr/bin/python3
import time
import paho.mqtt.publish as publish
import psutil

# Configuration
channel_ID = "1927000"
hostname   = "mqtt3.thingspeak.com"
topic      = f"channels/{channel_ID}/publish"

mqtt_client_ID = "YOUR_CLIENT_ID"
mqtt_username  = "YOUR_USERNAME"
mqtt_password  = "YOUR_PASSWORD"

auth_dict = {
    'client_id': mqtt_client_ID,
    'username': mqtt_username,
    'password': mqtt_password
}

while True:
    cpu_percent = psutil.cpu_percent()
    payload = f"field1={cpu_percent}&status=MQTTPUBLISH"
    
    print(f"Publishing CPU Load: {cpu_percent}%")
    publish.single(
        topic,
        payload=payload,
        hostname=hostname,
        port=1883,
        auth=auth_dict,
        qos=0
    )
    time.sleep(20) # 20 second interval (respecting 15s limit)
```

Detailed in [[Code-Python-SBC-01-ThingSpeak-Basic|Code Python SBC ThingSpeak Basic]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab06_Update ThingSpeak channel from SBC by MQTT.pdf` (Pages 1-3) & `AIoT_SBC/01_thingSpeak_Basic.py`
> 
> "Update ThingSpeak channel from SBC by MQTT... Configure MQTT device in ThingSpeak to obtain Client ID, Username, and Password... Use paho.mqtt.publish to send telemetry payloads to mqtt3.thingspeak.com on port 1883 with topic channels/<channelID>/publish."
