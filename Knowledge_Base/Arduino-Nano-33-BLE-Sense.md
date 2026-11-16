# Arduino Nano 33 BLE Sense

The **Arduino Nano 33 BLE Sense** (SKU: `ABX00031`) is a compact 3.3V microcontroller board designed around the **Nordic Semiconductor nRF52840** SoC and the **u-blox NINA-B306** module. It serves as the core edge compute engine in the [[00-AIoT-Knowledge-Base-Master-Index|AIoT Knowledge Base]] and powers edge artificial intelligence workflows using [[TensorFlow-Lite-for-Microcontrollers-TFLM|TensorFlow Lite for Microcontrollers (TFLM)]] and [[Edge-Impulse-Platform-and-Deployment|Edge Impulse]].

---

## 1. Architectural Overview & System Specifications

| Parameter | Specification |
| :--- | :--- |
| **Microcontroller SoC** | Nordic Semiconductor nRF52840 (integrated within u-blox NINA-B306) |
| **CPU Architecture** | 32-bit Arm Cortex-M4F with hardware Floating Point Unit (FPU) |
| **CPU Clock Speed** | 64 MHz |
| **Internal Flash Memory** | 1 MB |
| **Internal SRAM** | 256 KB |
| **Operating Voltage (Logic Level)**| 3.3V DC (**Note:** Pins are NOT 5V tolerant) |
| **Input Voltage (VIN Terminal)** | 4.5V to 21.0V DC (Recommended max 21V) |
| **Input Voltage (USB)** | 5.0V DC via Micro-USB port |
| **DC Current per I/O Pin** | Maximum 15 mA (output); Maximum 5 mA (input) |
| **Total GPIO + VDD Current Limit** | Maximum 25 mA across all GPIO pins combined |
| **DC-DC Step-Down Regulator** | Monolithic Power Systems MPM3610 (Efficiency up to 92%) |
| **Operating Temperature Range** | -40°C to +85°C (-40°F to +185°F) |
| **Physical Dimensions** | 45 mm x 18 mm (Nano form factor) |
| **Weight** | ~5 grams |

---

## 2. Integrated Onboard Sensor Suite

The board integrates an array of micro-electro-mechanical systems (MEMS) sensors connected via dedicated internal and external buses:

1. **Inertial Measurement Unit (IMU):**
   - [[LSM9DS1-9-Axis-IMU|STMicroelectronics LSM9DS1]]: 9 degrees-of-freedom (3-axis accelerometer, 3-axis gyroscope, 3-axis magnetometer) connected via internal I2C bus `Wire1` (`I2C1`).
2. **Digital Proximity, Gesture, Color, and Light Sensor:**
   - [[APDS-9960-Digital-Sensor|Broadcom APDS-9960]]: Proximity, ambient light, RGB color, and gesture sensor connected via internal I2C (`Wire1`, address `0x39`) and hardware interrupt pin `P0.19`.
3. **Barometric Pressure & Altimeter Sensor:**
   - [[LPS22HB-Barometric-Pressure-Sensor|STMicroelectronics LPS22HB]]: 260 to 1260 hPa piezoresistive absolute barometer connected via internal I2C (`Wire1`).
4. **Relative Humidity & Temperature Sensor:**
   - [[HTS221-Humidity-Temperature-Sensor|STMicroelectronics HTS221]]: Capacitive digital relative humidity and temperature sensor connected via internal I2C (`Wire1`).
5. **Digital MEMS Microphone:**
   - [[MP34DT05-A-MEMS-Microphone|STMicroelectronics MP34DT05-A / MP34DT06JTR]]: Omnidirectional digital microphone utilizing Pulse Density Modulation (PDM) with dedicated clock (`P0.26`) and data (`P0.25`) lines.
6. **Hardware Cryptographic Co-Processor:**
   - [[ATECC608A-Crypto-Authentication-Element|Microchip ATECC608A]]: Secure element storing cryptographic keys, hardware SHA-256 accelerator, and ECDSA signature engine.

---

## 3. Power Architecture & Voltage Regulation

The power tree of the Arduino Nano 33 BLE Sense includes two primary power inputs:
- **USB Connector:** 5V supplied from host PC or USB wall adapter through an onboard Schottky diode.
- **VIN Pin:** External power input accepting 4.5V to 21V.

The board features the **MPM3610** step-down switching buck regulator that converts input voltage down to **3.3V** to power the nRF52840 SoC, NINA-B306 module, and onboard sensors.

### Power Pin Behavior & Solder Jumpers:
- **`+3V3` Pin:** Supplies regulated 3.3V output to external circuits (subject to current limits).
- **`VUSB` Pin:** Connected to the 5V USB input via a solder jumper (`VUSB` pad). By default, this jumper is open (floating). Soldering the bridge allows drawing 5V from the USB host to power 5V peripherals like [[PWM-Servo-Motor-Control|servo motors]].
- **Battery Operation:** Cutting the bottom battery solder jumper disconnects the onboard regulator from reverse current, enabling direct low-power battery supply.

---

## 4. Programming & Bootloader Recovery Protocol

The Nano 33 BLE Sense runs the **Arduino Mbed OS Nano Boards** core. Communication is handled via native USB virtual serial port (CDC ACM).

### Recovery from Hung / Brick State:
If an erroneous sketch locks the CPU or disables interrupts:
1. Quickly **double-tap** the physical reset button on the board.
2. The onboard orange LED (`LED_BUILTIN` / `P0.13`) will start pulsing slowly (fading in and out).
3. The board enters the native bootloader mode and exposes a recovery virtual COM port.
4. Select the recovery COM port in Arduino IDE and upload a known working sketch (e.g., [[Lab-02-Run-Your-First-Program|Blink]]).

---

## 5. Network & System Connections

- **Carrier Expansion:** Mates directly into the [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier V1.0]] for [[OV7675-Camera-Module|OV7675 camera]] integration.
- **Gateway Interfacing:** Transmits telemetry to [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]] via [[Protocol-UART-Serial-Communication|UART]] or [[Protocol-Bluetooth-Low-Energy-GATT|BLE]].
- **Pinout Details:** See [[Nano-33-BLE-Sense-Pinout-Architecture|Nano 33 BLE Sense Pinout Architecture]] for physical pin definitions.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Arduino_Nao_datasheet.pdf` (Pages 1-5, 10-14)
> 
> "Nano 33 BLE Sense is a miniature sized module containing a NINA B306 module, based on Nordic nRF52480 and containing a Cortex M4F microcontroller and an internal 9 axis IMU. The module can either be mounted as a DIP component (when mounting pin headers), or as a SMT component, directly soldering it via the castellated pads... Operating Voltage 3.3V, Input Voltage (limit) 21V, DC Current per I/O Pin 15 mA."
>
> "Ratings: -40 °C (-40 °F) to 85 °C (185 °F). Storage Temperature -40 °C to 85 °C. Maximum input current per pin is 5mA. Maximum external current is 25mA for the sum of all GPIO currents and the current being drawn from VDD."
