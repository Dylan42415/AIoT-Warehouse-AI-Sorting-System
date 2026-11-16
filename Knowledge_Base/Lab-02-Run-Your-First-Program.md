# Lab 02: Run Your First Program

This document provides the exhaustive laboratory execution protocol for **Lab 02: Run Your First Program** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Master the foundational structure of an Arduino sketch: `setup()` and `loop()`.
2. Program digital output signals to blink the onboard LED (`LED_BUILTIN`).
3. Establish bidirectional USB serial communication with the host PC.
4. Utilize both the **Serial Monitor** and **Serial Plotter** for real-time debugging.

---

## 2. Anatomy of an Arduino Sketch

Every Arduino C/C++ program consists of two required functions:
- **`setup()`:** Executes once when the board powers up or resets. Used for initializing pin modes, communication peripherals, and libraries.
- **`loop()`:** Executes consecutively and infinitely after `setup()` completes. Houses core runtime logic, sensor reads, and actuation.

---

## 3. Step 1: Programming the Built-in LED (Blink)

### Sketch Code:
```cpp
void setup() {
  // Initialize digital pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (HIGH voltage level)
  delay(1000);                      // Wait for a second (1000 ms)
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage LOW
  delay(1000);                      // Wait for a second
}
```

### Technical Notes:
- `LED_BUILTIN` is a macro mapped to digital pin **`D13`** (`P0.13` on the nRF52840).
- `pinMode(pin, OUTPUT)` sets the pin's internal push-pull output driver.
- `digitalWrite(pin, HIGH)` supplies 3.3V; `LOW` pulls the pin to GND.
- `delay(ms)` halts execution for the specified milliseconds using hardware timer counters.

---

## 4. Step 2: Implementing Serial Communication

### Sketch Code:
```cpp
void setup() {
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  // Wait for native USB serial port to open (crucial on Nano 33 BLE)
  while (!Serial);
  
  Serial.println("System Initialized: Arduino Nano 33 BLE Sense");
}

void loop() {
  Serial.println("Hello AIoT World!");
  delay(1000);
}
```

> [!IMPORTANT] The `while (!Serial)` Requirement
> On native USB microcontrollers (like the nRF52840), `Serial.begin()` returns immediately before the host USB enumeration completes. The `while (!Serial)` loop blocks execution until the user opens the Serial Monitor, ensuring initialization messages are not missed.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab02_Run_Your_First_Program.pdf` (Pages 1-6)
> 
> "In this lab you will run your first program on the Arduino board... An Arduino program has two main functions: setup() which runs once at start, and loop() which runs continuously... The built-in LED is connected to pin 13... Serial.begin(9600) initializes communication. On native USB boards like Nano 33 BLE Sense, while (!Serial) waits for the Serial Monitor to be opened."
