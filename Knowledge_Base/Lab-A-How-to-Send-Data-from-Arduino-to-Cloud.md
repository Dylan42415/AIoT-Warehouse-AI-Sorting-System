# Lab A: How to Send Data from Arduino to Cloud

This document provides the exhaustive walkthrough for **Lab A: How to Send Data from Arduino to Cloud** (Author: Kenneth Foo, Student, ITE College West, South Korea Exchange Tutorial Notebook).

---

## 1. Architectural Overview & Tutorial Scope

This tutorial notebook outlines the foundational pipeline for reading sensor telemetry on an Arduino and pushing it to the ThingSpeak cloud using an intermediate Python gateway script running on a host computer or SBC:

$$\text{Arduino Nano 33 BLE Sense} \xrightarrow[\text{Baud: 9600}]{\text{USB Virtual Serial}} \text{Python Gateway Script} \xrightarrow[\text{Port 80 / 443}]{\text{HTTP GET REST}} \text{ThingSpeak Cloud}$$

---

## 2. Microcontroller Firmware Implementation

The Arduino reads the onboard [[APDS-9960-Digital-Sensor|APDS-9960]] sensor and transmits numeric values over the serial bus:

```cpp
#include <Arduino_APDS9960.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
    while (1);
  }
}

void loop() {
  if (APDS.proximityAvailable()) {
    int proximity = APDS.readProximity();
    Serial.println(proximity); // Send single integer value with newline
  }
  delay(1000);
}
```

---

## 3. Host Python Gateway Script (`urllib` / HTTP REST)

The host script opens the serial port, reads each line, and constructs an HTTP GET request to update the ThingSpeak channel:

```python
import serial
import time
import urllib.request

# Serial port configuration (COM port on Windows or /dev/ttyACM0 on Linux)
SERIAL_PORT = 'COM3' 
BAUD_RATE   = 9600

# ThingSpeak credentials
WRITE_API_KEY = "YOUR_THINGSPEAK_WRITE_API_KEY"
BASE_URL      = "https://api.thingspeak.com/update"

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
time.sleep(2) # Allow Arduino to reset upon serial connection

print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")

while True:
    try:
        raw_data = ser.readline().decode('utf-8').strip()
        if raw_data:
            print(f"Received from Arduino: {raw_data}")
            
            # Construct HTTP GET URL
            url = f"{BASE_URL}?api_key={WRITE_API_KEY}&field1={raw_data}"
            
            # Send HTTP request
            with urllib.request.urlopen(url) as response:
                html = response.read().decode('utf-8')
                print(f"ThingSpeak Response Entry ID: {html}")
                
            # Respect ThingSpeak 15-second rate limit
            time.sleep(15)
            
    except Exception as e:
        print(f"Error during execution: {e}")
        time.sleep(2)
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/A_How to send data from Arduino to Cloud.pdf` (Pages 1-13)
> 
> "How to send data from Arduino to cloud. Artificial Intelligence of Things. ITE South Korea Exchange. Kenneth Foo, Student, 4th June 2023. Tutorial Notebook... Step-by-step walkthrough of reading APDS-9960 proximity sensor, transmitting over USB serial at 9600 baud, and using a Python script with urllib.request to update a ThingSpeak channel using HTTP GET requests."
