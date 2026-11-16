# HTS221 Humidity & Temperature Sensor

The **STMicroelectronics HTS221** is an ultra-compact digital relative humidity and temperature sensor. It integrates a planar capacitor sensing element and an analog-to-digital front end, communicating with the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] over the internal I2C bus `Wire1`.

---

## 1. Technical Specifications

| Parameter | Relative Humidity (% rH) | Temperature (°C) |
| :--- | :--- | :--- |
| **Measurement Range** | 0% to 100% rH | -40°C to +120°C |
| **Accuracy** | ±3.5% rH (20% to 80% rH) | ±0.5°C (15°C to 40°C) |
| **Resolution** | 16-bit | 16-bit |
| **I2C Device Address** | `0x5F` (7-bit address) | `0x5F` |
| **Internal Bus** | `Wire1` (`P0.14` SDA1, `P0.15` SCL1) | `Wire1` |
| **Response Time** | $	au_{63\%} pprox 10	ext{ seconds}$ | - |

---

## 2. Factory Calibration Interpolation

The HTS221 stores unique factory calibration coefficients in its internal non-volatile registers (`H0_rH_x2`, `H1_rH_x2`, `T0_degC_x8`, etc.). Raw ADC data must be linearly interpolated against these coefficients:
$$rH\% = rac{	ext{raw} - H_0\_T0\_OUT}{H_1\_T0\_OUT - H_0\_T0\_OUT} 	imes (H_1\_rH - H_0\_rH) + H_0\_rH$$
The `Arduino_HTS221` library handles these calculations automatically.

---

## 3. Arduino Software API (`Arduino_HTS221.h`)

```cpp
#include <Arduino_HTS221.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!HTS.begin()) {
    Serial.println("Failed to initialize HTS221 sensor!");
    while (1);
  }
}

void loop() {
  float temperature = HTS.readTemperature(); // Degrees Celsius
  float humidity    = HTS.readHumidity();    // Percentage rH

  Serial.print("Temp: "); Serial.print(temperature); Serial.print(" °C | ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  delay(1000);
}
```

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Arduino_Nao_datasheet.pdf` (Page 4, Section 2.2)
> 
> "2.2 Humidity and Temperature Sensor: The HTS221 is an ultra-compact sensor for relative humidity and temperature. It includes a sensing element and a mixed signal ASIC to provide the measurement information through digital serial interfaces. Humidity accuracy: ±3.5% rH, 20 to +80% rH; Temperature accuracy: ±0.5 °C, 15 to +40 °C. Connected on internal bus Wire1 at address 0x5F."
