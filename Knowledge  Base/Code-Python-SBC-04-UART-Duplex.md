# Code: Python SBC 04 UART Duplex

This document provides the complete annotated source code and architectural analysis for `04_UART_Duplex_Basic.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/04_UART_Duplex_Basic.py`).

---

## 1. Architectural Overview

This script demonstrates **bidirectional (full-duplex) serial communication** between the [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]] and the [[Arduino-Nano-33-BLE-Sense|Arduino]]. The SBC continuously reads incoming telemetry packets and transmits periodic response commands (`"Hello Arduino\n\r"`) back to the microcontroller.

---

## 2. Complete Verbatim Source Code

```python
#! /usr/bin/python3
#import time library
import time
import serial

serial1 = serial.Serial('/dev/ttyUSB0',9600)

while True:
  dataString = serial1.readline()
  print(dataString)
  time.sleep(3)
  serial1.write("Hello Arduino\n\r".encode())
ser.close()#end of while loop, close the serial interface
```

---

## 3. Technical Details & Communication Protocol

1. **`serial.Serial('/dev/ttyUSB0', 9600)`:** Initializes the serial port at 9600 baud.
2. **`serial1.readline()`:** Blocks until a newline character (`\n`) is received from the Arduino.
3. **`serial1.write("Hello Arduino\n\r".encode())`:** Encodes the Python string into UTF-8 bytes and transmits them across the serial TX line to the Arduino.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/04_UART_Duplex_Basic.py`
> 
> "serial1 = serial.Serial('/dev/ttyUSB0', 9600)... while True: dataString = serial1.readline()... time.sleep(3)... serial1.write('Hello Arduino\\n\\r'.encode())."
