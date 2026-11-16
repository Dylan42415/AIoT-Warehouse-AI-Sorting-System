# Nano 33 BLE Sense Pinout Architecture

The **Arduino Nano 33 BLE Sense** features 30 physical DIP/castellated pins arranged in two 15-pin dual in-line headers (JP1 / Left Header and JP2 / Right Header). Every pin maps directly to internal ports on the **Nordic Semiconductor nRF52840** microcontroller.

---

## 1. Complete Physical Pin Mapping Table

| Physical Pin # | Board Label | Default Function | nRF52840 Port Pin | Alternate / Peripheral Functions | Electrical Notes |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **JP1-1** | `D13` | Digital I/O | `P0.13` | SPI SCK, PWM, Onboard Orange LED | Max 15mA output |
| **JP1-2** | `+3V3` | Power Output | N/A | Regulated 3.3V DC output from MPM3610 | Max current shared with VDD |
| **JP1-3** | `AREF` | Analog Ref | `P0.02` / AREF | Analog Reference Input | Max 3.3V |
| **JP1-4** | `A0` | Analog In | `P0.04` | AIN0, GPIO, PWM | 12-bit ADC |
| **JP1-5** | `A1` | Analog In | `P0.05` | AIN1, GPIO, PWM | 12-bit ADC |
| **JP1-6** | `A2` | Analog In | `P0.30` | AIN2, GPIO, PWM | 12-bit ADC |
| **JP1-7** | `A3` | Analog In | `P0.29` | AIN3, GPIO, PWM | 12-bit ADC |
| **JP1-8** | `A4` | Analog / I2C | `P0.31` | AIN4, External I2C SDA (`Wire`) | External I2C data |
| **JP1-9** | `A5` | Analog / I2C | `P0.02` | AIN5, External I2C SCL (`Wire`) | External I2C clock |
| **JP1-10**| `A6` | Analog In | `P0.28` | AIN6, GPIO, PWM, Software UART TX | Mbed UART TX |
| **JP1-11**| `A7` | Analog In | `P0.03` | AIN7, GPIO, PWM, Software UART RX | Mbed UART RX |
| **JP1-12**| `VUSB` | Power Pin | N/A | 5V USB Input (Direct from VBUS) | Default disconnected; requires solder bridge |
| **JP1-13**| `RESET` | Active-Low Reset | N/A | nRF52 Hardware Reset Pin | Pull to GND to reset; double-tap for bootloader |
| **JP1-14**| `GND` | Ground | N/A | Common System Ground | 0V reference |
| **JP1-15**| `VIN` | Power Input | N/A | External DC Input Voltage (4.5V to 21.0V) | Powers onboard buck regulator |
| **JP2-1** | `TX` / `D1` | Serial TX | `P0.23` | Hardware UART `Serial1` TX, PWM | Connects to SBC RX |
| **JP2-2** | `RX` / `D0` | Serial RX | `P1.09` | Hardware UART `Serial1` RX, PWM | Connects to SBC TX |
| **JP2-3** | `RESET` | Active-Low Reset | N/A | nRF52 Hardware Reset Pin | Pull to GND to reset |
| **JP2-4** | `GND` | Ground | N/A | Common System Ground | 0V reference |
| **JP2-5** | `D2` | Digital I/O | `P1.03` | GPIO, PWM, External Interrupt | High-speed GPIO |
| **JP2-6** | `D3` | Digital I/O | `P1.10` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-7** | `D4` | Digital I/O | `P1.11` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-8** | `D5` | Digital I/O | `P1.12` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-9** | `D6` | Digital I/O | `P1.15` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-10**| `D7` | Digital I/O | `P1.13` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-11**| `D8` | Digital I/O | `P1.14` | GPIO, PWM, External Interrupt | Timer/Counter |
| **JP2-12**| `D9` | Digital I/O | `P0.27` | GPIO, PWM, External Interrupt | Used for [[PWM-Servo-Motor-Control|Servo Control]] |
| **JP2-13**| `D10` | Digital I/O | `P1.02` | GPIO, PWM, SPI CS | Chip select |
| **JP2-14**| `D11` | Digital / SPI | `P1.01` | SPI MOSI (COPI), GPIO, PWM | Connects to Carrier J2 |
| **JP2-15**| `D12` | Digital / SPI | `P1.08` | SPI MISO (CIPO), GPIO, PWM | Connects to Carrier J3 & Button PB1 |

---

## 2. Dedicated Internal Peripheral & Sensor Port Mapping

The onboard sensors are connected to dedicated internal GPIO ports on the nRF52840 that do **NOT** conflict with the physical header pins:

- **Internal I2C Bus (`Wire1` / `I2C1`):**
  - `SDA1`: Port `P0.14` (Connected to [[LSM9DS1-9-Axis-IMU|LSM9DS1]], [[APDS-9960-Digital-Sensor|APDS-9960]], [[HTS221-Humidity-Temperature-Sensor|HTS221]], [[LPS22HB-Barometric-Pressure-Sensor|LPS22HB]])
  - `SCL1`: Port `P0.15`
- **APDS-9960 Interrupt:** Port `P0.19` (Active-low interrupt line)
- **Digital Microphone ([[MP34DT05-A-MEMS-Microphone|MP34DT05-A]]):**
  - PDM CLK: Port `P0.26`
  - PDM DOUT: Port `P0.25`
  - Microphone Power (VDD): Port `P0.17`
- **Onboard RGB LED:**
  - Red LED: Port `P0.24` (Active-Low)
  - Green LED: Port `P0.16` (Active-Low)
  - Blue LED: Port `P0.06` (Active-Low)
- **Built-in Yellow/Orange LED:** Port `P0.13` (`LED_BUILTIN`, Active-High)

---

## 3. Electrical Operational Boundaries

- **Pin Voltage Tolerance:** All input pins operate at **3.3V**. Supplying 5.0V directly to any GPIO or analog pin will permanently destroy the nRF52840 SoC.
- **Maximum Output Current:** 15 mA per single GPIO pin.
- **Maximum Combined Current:** 25 mA across all simultaneously active GPIOs.
- **Serial Designation Standard:** CIPO (Controller In, Peripheral Out) replaces MISO; COPI (Controller Out, Peripheral In) replaces MOSI.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Pinout-NANOsense_latest.pdf` (Pages 1-2)
> 
> "MAXIMUM input current per pin is 5mA. MAXIMUM external current is 25mA for the sum of all GPIO currents and the current being drawn from VDD. MAXIMUM output current per pin is 15mA. VIN 5-21 V input to the board. NOTE: CIPO/COPI have previously been referred to as MISO/MOSI."
>
> "Internal Port assignments: LSM9DS1: SDA1 P0.14, SCL1 P0.15; APDS9960: SDA1 P0.14, SCL1 P0.15, INT P0.19; HTS221: SDA1 P0.14, SCL1 P0.15; LSP22: SDA1 P0.14, SCL1 P0.15; MP34DT06JTR: CLK P0.26, DOUT P0.25, VDD P0.17."
