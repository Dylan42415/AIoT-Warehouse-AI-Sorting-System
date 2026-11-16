# Lab 11: Servo Motor Control Program

This document provides the exhaustive laboratory execution protocol for **Lab 11: Servo Motor Control Program** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the working principles of PWM servo actuators ([[PWM-Servo-Motor-Control|TowerPro SG90]]).
2. Wire the servo motor safely with external 5V power and shared ground.
3. Program positional angle sweeps ($0^\circ$ to $180^\circ$) using the `Servo.h` library.
4. Calibrate timing parameters and prevent mechanical binding.

---

## 2. Hardware Wiring Diagram

```
  Arduino Nano 33 BLE Sense                 TowerPro SG90 Servo
  +-------------------------+               +-------------------+
  |  Digital Pin 9 (PWM)    | ------------> |  Orange (Signal)  |
  |  GND Pin                | ---+--------> |  Brown (Ground)   |
  +-------------------------+    |          +-------------------+
                                 |                    ^
  External 5V Power Supply       |                    |
  +-------------------------+    |                    |
  |  +5V Terminal           | ---|--------------------+ Red (Power)
  |  GND Terminal           | ---+ (Shared Ground)
  +-------------------------+
```

---

## 3. Laboratory Sketch Code

```cpp
#include <Servo.h>

Servo myservo;
const int servoPin = 9;

void setup() {
  myservo.attach(servoPin); // Attach servo to pin 9
}

void loop() {
  // Sweep from 0 to 180 degrees in steps of 1 degree
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15); // Wait 15ms for servo to reach position
  }
  delay(1000);

  // Sweep from 180 down to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  delay(1000);
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab11_Servo_Motor_Control_Program.pdf` (Pages 1-6)
> 
> "A servo motor is an actuator that allows for precise control of angular position... Wire Brown to GND, Red to 5V, Orange to Pin 9... Use the Servo library attach() and write() functions to control angle from 0 to 180 degrees."
