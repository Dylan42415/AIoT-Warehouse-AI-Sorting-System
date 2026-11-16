# Code: Python SBC 02 USB ThingSpeak

This document provides the complete annotated source code and architectural analysis for `02_USB_thingSpeak_Basic.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/02_USB_thingSpeak_Basic.py`).

---

## 1. Architectural Overview

This script acts as the core gateway bridge between the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] and the [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak cloud]]. It opens the USB serial connection (`/dev/ttyUSB0` or `/dev/ttyACM0`), ingests sensor readings (such as [[APDS-9960-Digital-Sensor|proximity]] or [[Person-Detection-TinyML-Pipeline|person detection confidence]]), formats the reading into an MQTT payload, and uploads it to ThingSpeak.

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

serial1 = serial.Serial('/dev/ttyUSB0',9600)

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
    print("Received Serial Data: ", dataString)
    
    # build the payload string
    t_payload = "field1=" + str(dataString) + "&status=MQTTPUBLISH"
    # attempt to publish to ThingSpeak
    try:
        print("Publishing to ThingSpeak: " + str(dataString))
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

## 3. Hardware Port Configuration Notes

- **Linux Port Naming:** If using native Micro-USB on the Nano 33 BLE Sense, the port often instantiates as `/dev/ttyACM0` rather than `/dev/ttyUSB0`. If a `FileNotFoundError` occurs, modify the serial instantiation to:
  ```python
  serial1 = serial.Serial('/dev/ttyACM0', 9600)
  ```
- **String Cleaning:** `dataString.decode('utf-8').strip()` strips CR/LF (`\r\n`) characters to prevent invalid HTTP/MQTT query parameters.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/02_USB_thingSpeak_Basic.py`
> 
> "serial1 = serial.Serial('/dev/ttyUSB0', 9600)... dataString = serial1.readline().decode('utf-8').strip()... t_payload = 'field1=' + str(dataString) + '&status=MQTTPUBLISH'... publish.single(topic, payload=t_payload, hostname=hostname, port=port, auth=auth_dict, qos=0)."
