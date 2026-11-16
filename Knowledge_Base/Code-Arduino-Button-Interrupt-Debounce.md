# Code: Arduino Button Interrupt Debounce

This document provides the complete annotated source code, circuit wiring notes, and architectural analysis for `Button_Interrupt.ino` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_Arduino/Button_Interrupt/Button_Interrupt/`).

---

## 1. Architectural Overview

This implementation demonstrates hardware edge-triggered interrupt programming using `attachInterrupt()` to detect pushbutton actuations. It includes a non-blocking `millis()` debounce timer to eliminate false transitions caused by mechanical contact jitter. Furthermore, it demonstrates how the Mbed OS core instantiates a software UART instance (`UART mySerial(11, NC, NC, NC)`).

---

## 2. Complete Verbatim Source Code

```cpp
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;  // the number of the pushbutton pin
// variables will change:
uint btnCount = 0;
bool onceFlag = false;
UART mySerial(11, NC,NC,NC);

void setup() {
    delay(200);
    Serial.begin(9600);
    while (!Serial)
        ;
    mySerial.begin(9600);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), isr, FALLING);
}

void loop() {
    delay(200);
    //Serial.print("btnCount: ");
    //Serial.println(btnCount);
    if(onceFlag){
        onceFlag = false;
        Serial.print("btnCount: ");
        Serial.println(btnCount);
        mySerial.println(btnCount);
    }
}

void isr() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200) {
        btnCount++;
        onceFlag = true;
    }
    last_interrupt_time = interrupt_time;
}
```

---

## 3. Circuit Schematic & Hardware Notes

- **Source File `Button.txt`:**
  > "Use a pushbutton to control an LED."
- **Source Layout & Schematic:** (`layout.png`, `schematic.png`):
  - Pushbutton terminal 1 is connected to digital input pin `buttonPin`.
  - Pushbutton terminal 2 is connected directly to `GND`.
  - `pinMode(buttonPin, INPUT_PULLUP)` enables the internal pull-up resistor inside the nRF52840 SoC, holding the input at logic `HIGH` (3.3V) when unpressed.
  - When pressed, the switch shorts the input to `GND`, generating a clean `FALLING` electrical edge.

---

## 4. Software Debounce Algorithm Analysis

Inside the ISR `void isr()`:
1. `millis()` reads elapsed system runtime.
2. The condition `(interrupt_time - last_interrupt_time > 200)` enforces a **200-millisecond refractory lockout period**.
3. High-frequency chatter occurring within 200 ms of the initial press is discarded, incrementing `btnCount` exactly once per physical user press.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_Arduino/Button_Interrupt/Button_Interrupt/Button_Interrupt.ino` & `Button.txt`
> 
> "const int buttonPin = 13... attachInterrupt(digitalPinToInterrupt(buttonPin), isr, FALLING)... If interrupts come faster than 200ms, assume it's a bounce and ignore... UART mySerial(11, NC, NC, NC)..."
