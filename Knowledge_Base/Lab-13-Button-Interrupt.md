# Lab 13: Button Interrupt

This document provides the exhaustive laboratory execution protocol for **Lab 13: Button Interrupt** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the electrical and software differences between polling and [[Hardware-Interrupts-and-ISR-Programming|hardware interrupts]].
2. Wire a physical pushbutton (`PB1`) to digital input pin `D12` (or `D13`).
3. Configure hardware interrupt triggering using `attachInterrupt()`.
4. Apply the `volatile` keyword to shared variables.
5. Implement software debouncing using `millis()` to filter mechanical contact bounce.

---

## 2. Laboratory Sketch Implementation

```cpp
const int buttonPin = 12; // Pin connected to pushbutton PB1
const int ledPin    = LED_BUILTIN;

volatile uint btnCount = 0;
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // 200 ms lockout

void button_ISR() {
  unsigned long currentTime = millis();
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    btnCount++;
    lastDebounceTime = currentTime;
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Internal pullup resistor

  // Attach interrupt on falling edge (button pressed pulls to GND)
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_ISR, FALLING);

  Serial.println("Hardware Interrupt Setup Complete.");
}

void loop() {
  static uint lastCount = 0;
  if (btnCount != lastCount) {
    Serial.print("Button Pressed! Total Count: ");
    Serial.println(btnCount);
    digitalWrite(ledPin, (btnCount % 2 == 1) ? HIGH : LOW);
    lastCount = btnCount;
  }
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab13_Button_Interrupt.pdf` (Pages 1-5) & `AIoT_Arduino/Button_Interrupt/Button_Interrupt/Button_Interrupt.ino`
> 
> "Using hardware interrupts with a pushbutton to toggle states... attachInterrupt(digitalPinToInterrupt(buttonPin), ISR, FALLING)... Variables modified inside ISR must use volatile qualifier... Debounce filtering with millis() eliminates false triggers caused by mechanical contact bounce."
