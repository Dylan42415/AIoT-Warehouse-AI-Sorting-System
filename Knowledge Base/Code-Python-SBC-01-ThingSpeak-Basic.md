# Code: Python SBC 01 ThingSpeak Basic

This document provides the complete annotated source code and architectural analysis for `01_thingSpeak_Basic.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/01_thingSpeak_Basic.py`).

---

## 1. Architectural Overview

This Python script runs on the [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]] gateway. It queries system resource utilization metrics using `psutil` (specifically CPU usage percentage) and publishes the telemetry to the [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak cloud]] over [[Protocol-MQTT-Message-Queuing-Telemetry|MQTT]] using the `paho.mqtt.publish` client.

---

## 2. Complete Verbatim Source Code

```python
#! /usr/bin/python3
#import time library
import time
# Import important librarys for web service
import paho.mqtt.publish as publish
import psutil
import string

# The ThingSpeak Channel ID.
# Replace <YOUR-CHANNEL-ID> with your channel ID.
channel_ID = "1927000"
# The hostname of the ThingSpeak MQTT broker.
hostname = "mqtt3.thingspeak.com"
# The port of the ThingSpeak MQTT broker.
port = 1883
# The topic to publish to.
topic = "channels/" + channel_ID + "/publish"

# ThingSpeak MQTT Device Credentials
mqtt_client_ID = "<YOUR-CLIENT-ID>"
mqtt_username  = "<YOUR-USERNAME>"
mqtt_password  = "<YOUR-PASSWORD>"

# Create the MQTT authentication dictionary.
auth_dict = {
    'client_id': mqtt_client_ID,
    'username': mqtt_username,
    'password': mqtt_password
}

while True:
    # get the cpu usage
    cpu_percent = psutil.cpu_percent()
    # build the payload string
    t_payload = "field1=" + str(cpu_percent) + "&status=MQTTPUBLISH"
    # attempt to publish to ThingSpeak
    try:
        print("Publishing: " + str(cpu_percent))
        publish.single(
            topic,
            payload=t_payload,
            hostname=hostname,
            port=port,
            auth=auth_dict,
            qos=0
        )
        time.sleep(20) # 20 second sleep ensures compliance with ThingSpeak 15s rate limit
    except Exception as e:
        print(f"Error publishing to ThingSpeak: {e}")
        time.sleep(5)
```

---

## 3. Key Parameters & Configuration

- `channel_ID`: Set to the student's ThingSpeak Channel ID (e.g., `1927000`).
- `hostname`: Target broker `mqtt3.thingspeak.com` on unencrypted TCP port `1883`.
- `topic`: Constructed dynamically as `channels/<channel_ID>/publish`.
- `auth_dict`: Dictionary containing `client_id`, `username`, and `password` generated from ThingSpeak MQTT Device setup.
- `publish.single()`: Connects, publishes a single payload with `qos=0`, and cleanly terminates the TCP connection.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/01_thingSpeak_Basic.py`
> 
> "hostname = 'mqtt3.thingspeak.com', port = 1883, topic = 'channels/' + channel_ID + '/publish'... cpu_percent = psutil.cpu_percent()... t_payload = 'field1=' + str(cpu_percent) + '&status=MQTTPUBLISH'... publish.single(topic, payload=t_payload, hostname=hostname, port=port, auth=auth_dict, qos=0)."
