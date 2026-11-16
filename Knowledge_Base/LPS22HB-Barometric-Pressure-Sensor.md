# LPS22HB Barometric Pressure Sensor

The **STMicroelectronics LPS22HB** is an ultra-compact piezoresistive absolute barometer that functions as a digital output altimeter. It is integrated onto the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] on the internal I2C bus `Wire1`.

---

## 1. Technical Specifications

| Parameter | Specification |
| :--- | :--- |
| **Pressure Range** | 260 to 1260 hPa (mbar) |
| **Absolute Accuracy** | ±0.1 hPa |
| **Output Resolution** | 24-bit pressure output (4096 LSB/hPa) |
| **ODR (Output Data Rate)** | 1 Hz to 75 Hz |
| **RMS Noise** | 0.75 Pa (low noise mode) |
| **Temperature Range** | -40°C to +85°C |
| **I2C Address** | `0x5C` |
| **Bus Connection** | Internal I2C `Wire1` (`P0.14` SDA1, `P0.15` SCL1) |

---

## 2. Altitude Calculation (Hypsometric Formula)

Atmospheric pressure decreases logarithmically with increasing altitude:
$$h = 44330 	imes \left(1 - \left(rac{P}{P_0}
ight)^{rac{1}{5.255}}
ight)$$
Where:
- $h$ = Altitude in meters above sea level
- $P$ = Measured atmospheric pressure from LPS22HB (in kPa or hPa)
- $P_0$ = Sea-level reference pressure (standard $1013.25	ext{ hPa}$)

---

## 3. Arduino Software API (`Arduino_LPS22HB.h`)

```cpp
#include <Arduino_LPS22HB.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Failed to initialize LPS22HB barometer!");
    while (1);
  }
}

void loop() {
  float pressure = BARO.readPressure(); // Pressure in kPa
  Serial.print("Pressure = ");
  Serial.print(pressure * 10.0); // Convert kPa to hPa
  Serial.println(" hPa");
  delay(1000);
}
```

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Arduino_Nao_datasheet.pdf` (Page 4, Section 2.3)
> 
> "2.3 Barometric Pressure Sensor: The LPS22HB is an ultra-compact piezoresistive absolute barometer which functions as a digital output barometer. The device comprises a sensing element and an IC interface which communicates through I2C from the sensing element to the application. Pressure range: 260 to 1260 hPa. Connected on internal bus Wire1 at address 0x5C."
