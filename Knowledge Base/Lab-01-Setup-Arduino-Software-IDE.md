# Lab 01: Setup Arduino Software (IDE)

This document provides the exhaustive, step-by-step laboratory execution protocol for **Lab 01: Setup Arduino Software (IDE)** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives & Learning Outcomes

By the completion of this lab, the student will be able to:
1. Download, install, and configure **Arduino IDE 2.3.x** on Windows 64-bit.
2. Install the target hardware board core package: **Arduino Mbed OS Nano Boards**.
3. Install required sensor and protocol libraries via the Library Manager.
4. Establish USB driver connectivity and successfully verify port detection for the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]].
5. Execute the double-tap bootloader recovery procedure when a device fails to expose a virtual COM port.

---

## 2. Software Installation Steps

### Step 1: Installing Arduino IDE 2.3.x
1. Run the installer `arduino-ide_2.3.10_Windows_64bit.exe` located in `01_Software Installation/`.
2. Follow the setup wizard, accepting the license agreement and installing USB drivers for virtual serial devices.
3. Launch the Arduino IDE.

### Step 2: Installing the Board Support Package
1. Open the **Boards Manager** icon on the left navigation bar (or navigate to `Tools` > `Board` > `Boards Manager...`).
2. In the search field, type: `Nano 33 BLE`.
3. Locate **Arduino Mbed OS Nano Boards** by Arduino.
4. Click **INSTALL** (select the latest stable release, e.g., version 2.8.0 or later).
5. Wait for the compilation toolchains (`arm-none-eabi-gcc`), OpenOCD, and Mbed core files to download and unpack.

### Step 3: Installing Core Library Dependencies
Open the **Library Manager** (`Tools` > `Manage Libraries...`) and install the following packages:
1. **`Arduino_APDS9960`:** Driver for the digital proximity, gesture, and RGB sensor.
2. **`Arduino_LSM9DS1`:** Driver for the 9-axis inertial measurement unit.
3. **`Arduino_HTS221`:** Driver for the relative humidity and temperature sensor.
4. **`Arduino_LPS22HB`:** Driver for the barometric pressure sensor.
5. **`ArduinoBLE`:** Bluetooth Low Energy 5.0 protocol stack.
6. **`Servo`:** PWM servo motor control library.
7. **`Harvard_TinyMLx`** / **`Arduino_TensorFlowLite`:** TensorFlow Lite for Microcontrollers runtime and examples.

---

## 3. Hardware Interfacing & Port Selection

1. Connect the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] to the host computer using a Micro-USB data cable.
2. Navigate to `Tools` > `Board` > `Arduino Mbed OS Nano Boards` > select **Arduino Nano 33 BLE**.
3. Navigate to `Tools` > `Port` > select the active COM port (e.g., `COM3 (Arduino Nano 33 BLE)`).

---

## 4. Bootloader Recovery Protocol (Double-Tap Reset)

If the Arduino IDE cannot detect the board or fails with `Upload error: Failed uploading: uploading error: exit status 1`:
1. Press the small physical reset button on the Nano 33 BLE board **twice quickly (double-tap)**.
2. Observe the built-in orange LED (`LED_BUILTIN` / `P0.13`): it will begin a slow, rhythmic **pulsing / fading** sequence.
3. Check the `Tools` > `Port` menu: a new COM port will appear (indicating the board is in native bootloader mode).
4. Re-upload a basic sketch (e.g., [[Lab-02-Run-Your-First-Program|Blink]]) to restore normal operation.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab01_Setup_Arduino_Software(IDE).pdf` (Pages 1-10)
> 
> "Module Code: AI53003FP. Module: Artificial Intelligence of Things. Duration: 3 hours... Download and install the Arduino IDE... Open the Boards Manager and install 'Arduino Mbed OS Nano Boards'... Install libraries: Arduino_APDS9960, Arduino_LSM9DS1, ArduinoBLE, Harvard_TinyMLx... If the board is not detected, double-click the reset button to put it in bootloader mode (orange LED pulses)."
