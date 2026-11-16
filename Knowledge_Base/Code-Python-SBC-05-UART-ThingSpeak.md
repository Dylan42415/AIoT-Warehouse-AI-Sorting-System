# Code: Python SBC 05 UART ThingSpeak

This document provides the complete annotated source code and architectural analysis for `05_UART_thingSpeak_Basic.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/05_UART_thingSpeak_Basic.py`).

---

## 1. Architectural Overview

This script demonstrates connecting the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]] gateway to an Arduino device enumerated on `/dev/ttyACM1` (e.g., when multiple microcontrollers or serial dongles are attached simultaneously) and uploading telemetry to [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak]] over MQTT.

---

## 2. Complete Verbatim Source Code

```python
#! /usr/bin/python3
#import time library
import time
import serial
import time
# Import important librarys for web service
import paho.mqtt.publish as publish
import psutil
import string

serial1 = serial.Serial('/dev/ttyACM1',9600)

# The ThingSpeak Channel ID.
# Replace <YOUR-CHANNEL-ID> with your channel ID.
channel_ID = "1927000"
# The hostname of the ThingSpeak MQTT broker.
hostname = "mqtt3.thingspeak.com"
# The port of the ThingSpeak MQTT broker.
port = 1883
# The topic to publish to.
topic = "channels/" + channel_ID + "/publish"

mqtt_client_ID = "YOUR_CLIENT_ID"
mqtt_username  = "YOUR_USERNAME"
mqtt_password  = "YOUR_PASSWORD"

auth_dict = {
    'client_id': mqtt_client_ID,
    'username': mqtt_username,
    'password': mqtt_password
}

while True:
    dataString = serial1.readline()
    dataString = dataString.decode('utf-8')
    dataString = dataString.strip()
    print("Received from /dev/ttyACM1: ", dataString)
    
    # build the payload string
    t_payload = "field1=" + str(dataString) + "&status=MQTTPUBLISH"
    # attempt to publish to ThingSpeak
    try:
        print("Publishing: " + str(dataString))
        publish.single(
            topic,
            payload=t_payload,
            hostname=hostname,
            port=port,
            auth=auth_dict,
            qos=0
        )
        time.sleep(15)
    except Exception as e:
        print(f"Error publishing: {e}")
        time.sleep(5)
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/05_UART_thingSpeak_Basic.py`
> 
> "serial1 = serial.Serial('/dev/ttyACM1', 9600)... Reads data from secondary ACM port and forwards to ThingSpeak MQTT topic channels/<channel_ID>/publish."
