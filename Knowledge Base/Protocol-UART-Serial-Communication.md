# Protocol: UART Serial Communication

The **Universal Asynchronous Receiver-Transmitter (UART)** is a hardware communication interface that provides full-duplex asynchronous serial data transmission between embedded microcontrollers ([[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]]) and gateway computers ([[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]]).

---

## 1. Physical Layer & Framing Protocol

UART does not share a synchronous clock line between devices; both endpoints must be configured with an identical **Baud Rate** (symbol rate in bits per second).

```
  Idle (HIGH)  +--+   +---+---+---+---+---+---+---+---+   +-----+ Idle (HIGH)
  -------------+  |   | D0| D1| D2| D3| D4| D5| D6| D7|   |     +------------
               |  |   |   |   |   |   |   |   |   |   |   |     |
               +--+---+---+---+---+---+---+---+---+---+---+-----+
              Start Bit             8 Data Bits (LSB First)   Stop Bit
               (LOW)                                           (HIGH)
```

### Standard 8N1 Frame Structure:
1. **Idle State:** Signal line is held at logic `HIGH` (3.3V).
2. **Start Bit:** Active `LOW` transition for 1 bit period signaling the arrival of a character.
3. **Data Payload:** 8 data bits transmitted Least Significant Bit (LSB) first.
4. **Parity Bit:** None (`N`), omitted for maximum throughput.
5. **Stop Bit:** Active `HIGH` level for 1 bit period (`1`), returning the line to idle.

---

## 2. Hardware Pin Connections & Signal Crossing

UART requires two transmission lines and a shared reference ground:
- **TX (Transmit):** Drives outgoing serial bits.
- **RX (Receive):** Samples incoming serial bits.

```
  Arduino Nano 33 BLE Sense                 Raspberry Pi SBC (Gateway)
  +-------------------------+               +-------------------------+
  |  TX Pin (D1 / P0.23)    | ------------> |  RX Pin (GPIO 15/Pin 10)|
  |  RX Pin (D0 / P1.09)    | <------------ |  TX Pin (GPIO 14/Pin 8) |
  |  GND Pin                | ------------- |  GND Pin (Pin 6/9/14)   |
  +-------------------------+               +-------------------------+
```

> [!IMPORTANT] RX/TX Crossover Rule
> Transmit (`TX`) on the microcontroller must connect to Receive (`RX`) on the receiver, and vice versa. Connecting TX to TX or omitting the common Ground line will result in complete communication failure or corrupted framing errors.

---

## 3. Microcontroller Implementation (`Serial` vs. `Serial1` vs. Mbed `UART`)

On the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]]:
1. **`Serial`:** Emulates a USB CDC-ACM virtual serial port over the Micro-USB connector. Communicates with the host PC Serial Monitor or SBC via `/dev/ttyACM0`.
2. **`Serial1`:** Controls the hardware UART connected to physical header pins **`TX` (`D1`)** and **`RX` (`D0`)**.
3. **Mbed Dynamic UART (`UART` Object):** Under the Arduino Mbed OS core, secondary software/hardware UART instances can be routed to arbitrary pins:
   ```cpp
   UART mySerial(A6, A7, NC, NC); // TX on A6, RX on A7
   ```
   Utilized in [[Code-Arduino-UART-Basic|Code Arduino UART Basic]] and connected via Carrier header `J1`.

---

## 4. SBC Host Interface (Python `pyserial`)

On the Linux gateway SBC:
```python
import serial
import time

# Open USB virtual serial port at 9600 baud, 8N1, 1-second timeout
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

while True:
    if ser.in_waiting > 0:
        raw_line = ser.readline()
        # Decode ASCII / UTF-8 string and strip whitespace
        decoded_line = raw_line.decode('utf-8', errors='ignore').strip()
        print(f"Received from Arduino: {decoded_line}")
    time.sleep(0.01)
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab07_Arduino_SBC_Communication.pdf` (Pages 1-4) & `03_Lab Sheets/Code Example for students/AIoT_Arduino/UART_Basic/UART_Basic.ino`
> 
> "Serial communication is the process of sending data one bit at a time, sequentially, over a communication channel or computer bus... We connect the Arduino Nano 33 BLE Sense to the SBC using either the USB cable (/dev/ttyACM0) or using hardware UART TX/RX pins... Ensure both ends use identical baud rate (9600 bps)... In Python, we use the serial module (pyserial) to open the port, read lines, and decode strings."
