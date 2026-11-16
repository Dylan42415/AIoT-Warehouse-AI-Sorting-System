# Lab 03: Proximity Sensor Protocol

This document provides the exhaustive laboratory execution protocol for **Lab 03: Proximity Sensor** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the operating physics of the integrated [[APDS-9960-Digital-Sensor|APDS-9960]] digital sensor.
2. Initialize and configure the sensor using the `Arduino_APDS9960` library over internal bus `Wire1`.
3. Sample real-time proximity readings and understand distance mapping.
4. Implement threshold-based event triggers (e.g., lighting an LED when an object is detected).

---

## 2. Sensor Operation & Value Interpretation

- **IR Emission & Reflection:** The APDS-9960 emits pulsed 950 nm infrared light and measures the reflected photon intensity.
- **Library Value Scale:**
  - `0`: Maximum proximity (object is directly touching or extremely close to the sensor aperture).
  - `255`: Minimum proximity (no object detected, or target is out of detection range).
  - Intermediate values ($10 \text{ to } 200$) indicate physical distances between $\approx 10\text{ mm}$ and $200\text{ mm}$.

---

## 3. Laboratory Sketch Implementation

```cpp
#include <Arduino_APDS9960.h>

const int ledPin = LED_BUILTIN;
const int proximityThreshold = 50; // Threshold for object detection

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT);

  // Initialize APDS-9960 sensor on internal Wire1 bus
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
    while (1); // Halt if sensor not detected
  }
  Serial.println("APDS-9960 Proximity Sensor Initialized.");
}

void loop() {
  // Check if a new proximity reading is available
  if (APDS.proximityAvailable()) {
    int proximity = APDS.readProximity();
    Serial.print("Proximity Value: ");
    Serial.println(proximity);

    // If object is closer than threshold (value < threshold)
    if (proximity < proximityThreshold) {
      digitalWrite(ledPin, HIGH); // Turn LED ON
      Serial.println("--> Object DETECTED!");
    } else {
      digitalWrite(ledPin, LOW);  // Turn LED OFF
    }
  }
  delay(100); // Polling delay
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab03_Proximity_Sensor.pdf` (Pages 1-5)
> 
> "Module Code: AI53003FP. Duration: 3 hours... The APDS-9960 sensor has an integrated IR LED and detector. In the Arduino_APDS9960 library, readProximity() returns 0 when the object is closest to the sensor, and 255 when the object is far away or not detected... Use APDS.begin() to start the sensor, APDS.proximityAvailable() to check for data, and APDS.readProximity() to read values."
