# APDS-9960 Digital Sensor

The **Broadcom (Avago) APDS-9960** is an integrated multi-functional digital sensor featuring **advanced proximity detection**, **ambient light sensing (ALS)**, **RGB color sensing**, and **four-directional gesture sensing**. On the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]], it is connected to the internal I2C bus `Wire1`.

---

## 1. Electrical & Sensor Specifications

| Parameter | Specification |
| :--- | :--- |
| **I2C Device Address** | `0x39` (7-bit address) |
| **Connected I2C Bus** | Internal `Wire1` (`SDA1` = `P0.14`, `SCL1` = `P0.15`) |
| **Interrupt Output Pin** | `P0.19` (Active-low open-drain output) |
| **Operating Voltage (VDD)** | 2.4V to 3.6V (Operates at 3.3V on Nano 33 BLE) |
| **Proximity Detection Range** | ~100 mm to 200 mm nominal sensing distance |
| **IR LED Pulse Source** | Built-in 950 nm infrared LED emitter |
| **Proximity ADC Resolution** | 8-bit unsigned integer (`0` to `255`) |
| **Color / Ambient Light ADC** | 16-bit resolution per channel (Clear, Red, Green, Blue) |
| **Gesture Sensing Method** | 4 directional photodiodes (Up, Down, Left, Right) detecting motion vectors |

---

## 2. Sensor Operation Principles

```
         IR LED (950nm)              Photodiode Array (R, G, B, Clear, IR)
              \                                /
               \                              /  Reflected Optical Energy
                \                            /
                 v                          /
              +-------------------------------+
              |   Target Obstacle / Hand     |
              +-------------------------------+
```

1. **Proximity Sensing:** The sensor drives pulsed IR current through its internal IR LED. The reflected IR light bounces back from nearby objects into the integrated photodiode array. The integrated analog front-end digitizes the amplitude into an 8-bit proximity reading.
2. **Value Inversion in Arduino Library:** In the official `Arduino_APDS9960` library:
   - Reading `0`: Target is closest (maximum proximity / reflection).
   - Reading `255`: Target is furthest or out of range.
3. **Gesture Engine:** The 4 directional photodiodes detect phase delays as a hand swipes across the sensor, generating directional events (`GESTURE_UP`, `GESTURE_DOWN`, `GESTURE_LEFT`, `GESTURE_RIGHT`).

---

## 3. Arduino Software API (`Arduino_APDS9960.h`)

### Core Initialization & Methods:
```cpp
#include <Arduino_APDS9960.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize sensor on internal Wire1 bus
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
    while (1);
  }
}

void loop() {
  // Check if proximity sample is ready
  if (APDS.proximityAvailable()) {
    // Read proximity value (0 = closest, 255 = furthest)
    int proximity = APDS.readProximity();
    Serial.print("Proximity: ");
    Serial.println(proximity);
  }
}
```

### Color Sensing Methods:
- `APDS.colorAvailable()`: Returns `true` when a new RGBC sample is available.
- `APDS.readColor(r, g, b)`: Reads 16-bit Red, Green, and Blue intensity values.

---

## 4. Associated Labs & Cyber-Physical Integration

- **[[Lab-03-Proximity-Sensor-Protocol|Lab 03 Proximity Sensor]]:** Initial testing and calibration of proximity values.
- **[[Lab-12-Integrating-Proximity-Sensor-and-Servo-Motor-Control|Lab 12 Proximity & Servo Control]]:** Triggering [[PWM-Servo-Motor-Control|servo actuation]] based on proximity thresholds.
- **[[Lab-A-How-to-Send-Data-from-Arduino-to-Cloud|Lab A Arduino to Cloud]]:** Streaming proximity data to [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab03_Proximity_Sensor.pdf` (Pages 1-5)
> 
> "The APDS-9960 sensor features advanced gesture detection, proximity detection, digital Ambient Light Sense (ALS) and Color Sense (RGBC). The proximity detection feature operates well from bright sunlight to dark rooms... When an object is close to the sensor, the proximity value returned is 0; when the object is far away, the reading increases towards 255."
>
> "Library functions: APDS.begin(), APDS.proximityAvailable(), APDS.readProximity(). Connected to internal I2C bus Wire1."
