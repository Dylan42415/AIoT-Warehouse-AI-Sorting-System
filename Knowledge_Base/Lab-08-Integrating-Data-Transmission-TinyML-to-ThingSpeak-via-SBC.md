# Lab 08: Integrating Data Transmission from TinyML to ThingSpeak via SBC

This document provides the exhaustive laboratory execution protocol for **Lab 08: Integrating Data Transmission from TinyML to ThingSpeak via SBC** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Architect the complete end-to-end AIoT integration pipeline:
   $$\text{Nano 33 BLE (TinyML)} \xrightarrow{\text{UART/USB}} \text{SBC Gateway} \xrightarrow{\text{MQTT}} \text{ThingSpeak Cloud}$$
2. Ingest real-time inference scores from edge ML models.
3. Parse and buffer telemetry payloads on the SBC.
4. Publish inference telemetry to the cloud for real-time visualization.

---

## 2. Integrated System Architecture

```
  [ Edge Inference Node ]                 [ Fog Gateway Node ]              [ Cloud Dashboard ]
  Arduino Nano 33 BLE Sense               Raspberry Pi SBC                  ThingSpeak Cloud
  - Executes Person Detection / APDS      - Executes 02_USB_thingSpeak      - Ingests MQTT stream
  - Serial.println(inferenceScore) =====> - Ingests /dev/ttyACM0 ======>   - Visualizes live chart
    (over USB / UART at 9600 baud)          (Publishes to MQTT broker)        (Alerts & Storage)
```

---

## 3. Gateway Integration Script (`02_USB_thingSpeak_Basic.py`)

```python
#! /usr/bin/python3
import time
import serial
import paho.mqtt.publish as publish

# Serial setup
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
channel_ID = "1927000"
hostname   = "mqtt3.thingspeak.com"
topic      = f"channels/{channel_ID}/publish"

auth_dict = {
    'client_id': 'YOUR_CLIENT_ID',
    'username': 'YOUR_USERNAME',
    'password': 'YOUR_PASSWORD'
}

while True:
    line = ser.readline().decode('utf-8', errors='ignore').strip()
    if line:
        print(f"Ingested Edge Inference: {line}")
        payload = f"field1={line}&status=MQTTPUBLISH"
        publish.single(
            topic,
            payload=payload,
            hostname=hostname,
            port=1883,
            auth=auth_dict,
            qos=0
        )
        print("Forwarded to ThingSpeak Cloud!")
        time.sleep(15) # Maintain cloud rate limit
```

Detailed in [[Code-Python-SBC-02-USB-ThingSpeak|Code Python SBC USB ThingSpeak]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab08_Integrating Data Transmission from TinyML to ThingSpeak via SBC.pdf` (Pages 1-2)
> 
> "Integrate edge inference data transmission from Arduino TinyML to ThingSpeak cloud via the SBC gateway... The Arduino outputs inference classifications over serial; the SBC reads the serial port and publishes the telemetry to ThingSpeak using MQTT."
