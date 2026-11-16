# Theory: TinyML Kit Hardware and Software

This document synthesizes Chapter 7 (`TinyML Kit Overview - HW and SW installation & Test`, Prof. Marcelo Rovai, UNIFEI) and the Harvard SciTinyML Kit documentation, detailing the integrated physical kit and development environment.

---

## 1. Physical Hardware Kit Components

The educational TinyML Kit is composed of three tightly integrated hardware modules:

```
                  +--------------------------------+
                  |  1. Arduino Nano 33 BLE Sense  |
                  |     (Nordic nRF52840 MCU)      |
                  +---------------+----------------+
                                  | DIP Header
                                  v
                  +--------------------------------+
                  |  2. Machine Learning Carrier   |
                  |     (V1.0 Expansion Board)     |
                  +---------------+----------------+
                                  | J8 Socket
                                  v
                  +--------------------------------+
                  |  3. OV7675 Camera Module       |
                  |     (CMOS VGA Image Sensor)    |
                  +--------------------------------+
```

1. **Core Compute Node:** [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] containing the 64 MHz Arm Cortex-M4F MCU and full MEMS sensor suite.
2. **Expansion Baseboard:** [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier V1.0]] providing Grove connectors (`J1` to `J6`), screw terminal power (`J7`), pushbutton (`PB1`), and camera socket (`J8`).
3. **Vision Sensor:** [[OV7675-Camera-Module|OmniVision OV7675 Camera]] providing real-time 8-bit parallel digital video for vision tasks.

---

## 2. Software Development Environment

### 1. Arduino IDE Setup:
- **IDE Version:** Arduino IDE 2.3.x (or 1.8.x legacy).
- **Core Package:** `Arduino Mbed OS Nano Boards` (version 2.8.0 or later installed via Boards Manager).

### 2. Core Library Dependencies:
- `Arduino_TensorFlowLite` / `Harvard_TinyMLx`: Contains pre-compiled TFLM kernels and standard model implementations ([[Lab-09-Person-Detection|Person Detection]], [[Lab-10-TinyML-Keyword-Spotting|Micro Speech]]).
- `Arduino_APDS9960`: Interfacing driver for proximity, gesture, and color sensing.
- `Arduino_LSM9DS1`: Driver for the 9-axis inertial measurement unit.
- `Arduino_HTS221` & `Arduino_LPS22HB`: Drivers for environmental telemetry.
- `PDM`: Core library for digitizing audio from the MEMS microphone.
- `ArduinoBLE`: Bluetooth Low Energy protocol stack.

---

## 3. Hardware Assembly & Bring-up Verification Protocol

1. **Carrier Board Mating:** Align the pin headers of the Nano 33 BLE Sense with the female headers `JANALOG1` and `JDIGITAL1` on the carrier. Ensure the Micro-USB port faces outward. Press firmly until seated.
2. **Camera Insertion:** Insert the OV7675 camera module into the 20-pin header `J8`. Ensure the camera lens faces outward away from the board.
3. **Connection to Host:** Connect via Micro-USB to PC.
4. **Diagnostic Verification:**
   - Run the Blink sketch to confirm board detection and USB communication.
   - Run sensor diagnostic tests to confirm I2C device discovery (`0x39` for APDS-9960, `0x6B` for LSM9DS1, `0x5F` for HTS221, `0x5C` for LPS22HB).

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 7_TinyML Kit Overview - HW and SW.pdf` (Pages 1-32) & `04_Hardware_Documents/TinyML_Introduction.pdf` (Pages 1-27)
> 
> "17. TinyML Kit Overview - HW and SW installation & Test. Prof. Marcelo Rovai, UNIFEI... The TinyML Kit contains: Arduino Nano 33 BLE Sense, Tiny Machine Learning Shield / Carrier, and an OV7675 camera module. Software prerequisites include the Arduino IDE, Arduino Mbed OS Nano Boards package, and Harvard_TinyMLx library... Verification steps require slotting the Nano board into the carrier, connecting the camera to socket J8, and uploading test programs."
