# Code: Python SBC 03 BLE ThingSpeak

This document provides the complete annotated source code and architectural analysis for `03_BLE_thingSpeak_Basic.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/03_BLE_thingSpeak_Basic.py`).

---

## 1. Architectural Overview

This script establishes a wireless **Bluetooth Low Energy (BLE)** bridge. Running asynchronously on the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]], it uses `bleak.BleakClient` to connect to the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]], reads the target GATT characteristic, and forwards the data payload to [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak]] over MQTT.

---

## 2. Complete Verbatim Source Code

```python
#! /usr/bin/python3
#import time library
import time
import asyncio
from bleak import BleakClient

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

mqtt_client_ID = "YOUR_CLIENT_ID"
mqtt_username  = "YOUR_USERNAME"
mqtt_password  = "YOUR_PASSWORD"

auth_dict = {
    'client_id': mqtt_client_ID,
    'username': mqtt_username,
    'password': mqtt_password
}

address = "E4:F4:02:13:5F:72" # Target Arduino Nano 33 BLE MAC Address
MODEL_NBR_UUID = "7299a27d-8110-4eb7-b67b-ad03cec64c02" # Characteristic UUID

async def main(address):
    async with BleakClient(address) as client:
        model_number = await client.read_gatt_char(MODEL_NBR_UUID)
        val = int.from_bytes(model_number, byteorder='little')
        print("Read from BLE: {0}".format(val))
        t_payload = "field1=" + str(val) + "&status=MQTTPUBLISH"
        try:
            publish.single(
                topic,
                payload=t_payload,
                hostname=hostname,
                port=port,
                auth=auth_dict,
                qos=0
            )
            print("Successfully published BLE telemetry to ThingSpeak!")
        except Exception as e:
            print(f"MQTT publish failed: {e}")

while True:
    asyncio.run(main(address))
    time.sleep(15)
```

---

## 3. Technical Breakdown

1. **MAC Address Targeting:** `address = "E4:F4:02:13:5F:72"` targets the specific Bluetooth MAC address discovered via [[Code-Python-SBC-BLE-Scanner|discover_py.py]].
2. **Asynchronous Context Manager:** `async with BleakClient(address) as client` manages connection handshake and automatic disconnection.
3. **Byte Decoding:** `int.from_bytes(model_number, byteorder='little')` converts the raw little-endian binary byte received over BLE into a Python integer.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/03_BLE_thingSpeak_Basic.py`
> 
> "from bleak import BleakClient... address = 'E4:F4:02:13:5F:72', MODEL_NBR_UUID = '7299a27d-8110-4eb7-b67b-ad03cec64c02'... model_number = await client.read_gatt_char(MODEL_NBR_UUID)... publish.single(topic, payload=t_payload, hostname=hostname, port=port, auth=auth_dict, qos=0)."
