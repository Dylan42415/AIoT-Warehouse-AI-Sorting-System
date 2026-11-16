# Lab 07: Arduino to SBC Communication

This document provides the exhaustive laboratory execution protocol for **Lab 07: Arduino SBC Communication** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Connect the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] to the [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]] using USB serial cable and hardware UART.
2. Identify Linux serial device nodes (`/dev/ttyACM0`, `/dev/ttyUSB0`, `/dev/serial0`).
3. Implement a Python receiving script using `pyserial` on the SBC.
4. Establish bidirectional serial communication and verify data packet integrity.

---

## 2. Hardware Connection & Device Identification

1. Connect the Arduino Nano 33 BLE Sense to the SBC via USB cable.
2. In the SBC terminal, list active serial ports:
   ```bash
   ls -l /dev/ttyACM* /dev/ttyUSB*
   ```
   The device appears as `/dev/ttyACM0` (or `/dev/ttyUSB0`).
3. Grant dialout user permissions if required:
   ```bash
   sudo usermod -a -G dialout pi
   ```

---

## 3. Microcontroller Transmitter Sketch

```cpp
void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue); // Transmit formatted line
  delay(1000);
}
```

---

## 4. SBC Python Receiver Script

```python
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

print("Listening for Arduino serial stream...")

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        print(f"Received: {line}")
    time.sleep(0.05)
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab07_Arduino_SBC_Communication.pdf` (Pages 1-4)
> 
> "Connect the Arduino Nano 33 BLE Sense to the SBC using the USB cable. The device will be recognized as /dev/ttyACM0 or /dev/ttyUSB0... Use pyserial in Python to establish communication at 9600 baud rate and read data using ser.readline()."
