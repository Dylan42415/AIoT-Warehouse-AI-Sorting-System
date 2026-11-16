# Code: Arduino UART Basic

This document provides the complete annotated source code and architectural analysis for `UART_Basic.ino` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_Arduino/UART_Basic/UART_Basic.ino`).

---

## 1. Architectural Overview

This sketch configures an auxiliary hardware UART instance on the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] using the Arduino Mbed OS `UART` class. Signals are routed to header pins **`A6` (TX)** and **`A7` (RX)** (corresponding to header `J1` on the [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier]]). It transmits periodic telemetry test packets over the auxiliary serial line while simultaneously maintaining host USB communication on `Serial`.

---

## 2. Complete Verbatim Source Code

```cpp
UART mySerial(A6, A7, NC, NC);   // A6 (Nano TX), A7 (Nao RX)
//String testData = "123";
int mmm=123;
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.println("123");
  Serial.println("Send:123");
  delay(1000);
}
```

---

## 3. Technical Breakdown & Hardware Routing

1. **`UART mySerial(A6, A7, NC, NC)` Constructor:**
   - Parameter 1: `tx_pin` $\rightarrow$ `A6` (`P0.28` on nRF52840).
   - Parameter 2: `rx_pin` $\rightarrow$ `A7` (`P0.03` on nRF52840).
   - Parameter 3: `rts_pin` $\rightarrow$ `NC` (No flow control).
   - Parameter 4: `cts_pin` $\rightarrow$ `NC` (No flow control).
2. **Carrier Connection:** Header `J1` on the Machine Learning Carrier exposes `A6` on Pin 1, `A7` on Pin 2, `+3V3` on Pin 3, and `GND` on Pin 4. This connector can be cabled directly to the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]] hardware UART pins (`GPIO 14` / `15`).

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_Arduino/UART_Basic/UART_Basic.ino`
> 
> "UART mySerial(A6, A7, NC, NC); // A6 (Nano TX), A7 (Nao RX)... mySerial.begin(9600); Serial.begin(9600); mySerial.println('123'); Serial.println('Send:123'); delay(1000);"
