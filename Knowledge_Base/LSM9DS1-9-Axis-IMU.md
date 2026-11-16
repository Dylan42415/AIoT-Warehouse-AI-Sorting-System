# LSM9DS1 9-Axis IMU

The **STMicroelectronics LSM9DS1** is an ultra-compact system-in-package featuring a **3-axis digital accelerometer**, a **3-axis digital gyroscope**, and a **3-axis digital magnetometer**. It provides motion and orientation telemetry on the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]].

---

## 1. Hardware Architecture & Specifications

| Parameter | Accelerometer | Gyroscope | Magnetometer |
| :--- | :--- | :--- | :--- |
| **Measurement Axes** | X, Y, Z | X, Y, Z (Pitch, Roll, Yaw) | X, Y, Z |
| **Selectable Full Scales** | ±2g, ±4g, ±8g, ±16g | ±245 dps, ±500 dps, ±2000 dps | ±4, ±8, ±12, ±16 gauss |
| **Linear Sensitivity (at ±2g)** | 0.061 mg/LSB | - | - |
| **Angular Sensitivity (±245 dps)**| - | 8.75 mdps/LSB | - |
| **Magnetic Sensitivity (±4 gauss)**| - | - | 0.14 mgauss/LSB |
| **Interface Bus** | Internal I2C (`Wire1`) | Internal I2C (`Wire1`) | Internal I2C (`Wire1`) |
| **I2C Address** | `0x6B` (Linear / Gyro) | `0x6B` | `0x1E` (Magnetic) |
| **ADC Resolution** | 16-bit | 16-bit | 16-bit |

---

## 2. Arduino Software API (`Arduino_LSM9DS1.h`)

The official `Arduino_LSM9DS1` library handles register communication over `Wire1`:

```cpp
#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize LSM9DS1 IMU!");
    while (1);
  }
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z); // Acceleration in G's
    Serial.print("Acc: "); Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", "); Serial.println(z);
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z); // Angular velocity in degrees/second
  }

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z); // Magnetic field in uT (microteslas)
  }
}
```

---

## 3. TinyML Motion Applications

The LSM9DS1 is the primary data source for inertial ML models:
- **[[Lab-14-Edge-Impulse-Data-Collection|Lab 14 Edge Impulse Data Collection]]:** Sampling continuous 3-axis accelerometer data at 50 Hz to 100 Hz.
- **Gesture & Activity Recognition:** Classifying physical movements (e.g., waving, punching, idle, shaking) using time-series neural networks.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Arduino_Nao_datasheet.pdf` (Page 4, Section 2.4 IMU)
> 
> "2.4 IMU: The LSM9DS1 is a 9-axis motion sensor containing a 3D digital linear acceleration sensor, a 3D digital angular rate sensor, and a 3D digital magnetic sensor. It features linear acceleration full scales of ±2g/±4g/±8g/±16g, angular rate of ±245/±500/±2000 dps, and magnetic field full scale of ±4/±8/±12/±16 gauss. Connected via internal I2C bus Wire1."
