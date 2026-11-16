# Lab 12: Integrating Proximity Sensor and Servo Motor Control

This document provides the exhaustive laboratory execution protocol for **Lab 12: Integrating Proximity Sensor and Servo Motor Control** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Design and build a closed-loop cyber-physical system integrating a sensor ([[APDS-9960-Digital-Sensor|APDS-9960 Proximity]]) and an actuator ([[PWM-Servo-Motor-Control|SG90 Servo]]).
2. Implement threshold-based event logic simulating an automated smart gate or barrier.
3. Manage concurrent sensor sampling and non-blocking servo positioning.

---

## 2. Cyber-Physical Control Logic Flowchart

```
  +-------------------------------------+
  | Read Proximity from APDS-9960       |
  +------------------+------------------+
                     |
                     v
             /-----------------\
            /  Proximity < 50   \  (Object Approached / Detected)
            \   Threshold?      /
             \-----------------/
              |               |
        YES   |               | NO
              v               v
  +--------------------+  +--------------------+
  | Open Gate:         |  | Close Gate:        |
  | servo.write(90)    |  | servo.write(0)     |
  | delay(3000) (Hold) |  +--------------------+
  +--------------------+
```

---

## 3. Laboratory Sketch Code

```cpp
#include <Arduino_APDS9960.h>
#include <Servo.h>

Servo gateServo;
const int servoPin = 9;
const int proximityThreshold = 50; // Threshold for detection

void setup() {
  Serial.begin(9600);
  while (!Serial);

  gateServo.attach(servoPin);
  gateServo.write(0); // Start in closed position (0 degrees)

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
    while (1);
  }
  Serial.println("Smart Gate System Initialized.");
}

void loop() {
  if (APDS.proximityAvailable()) {
    int prox = APDS.readProximity();
    Serial.print("Proximity: ");
    Serial.println(prox);

    if (prox < proximityThreshold) {
      Serial.println("Object Approaching -> OPENING GATE (90 deg)");
      gateServo.write(90);
      delay(3000); // Keep gate open for 3 seconds
      Serial.println("CLOSING GATE (0 deg)");
      gateServo.write(0);
    }
  }
  delay(100);
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab12_Integrating Proximity sensor and Servo Motor Control-v1.pdf` (Pages 1-6)
> 
> "Integrating proximity sensor and servo motor control... When an object approaches the APDS-9960 sensor (proximity reading drops below threshold), the program actuates the servo motor to rotate to 90 degrees, delays, and returns to 0 degrees to simulate an automated barrier."
