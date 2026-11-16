# Alpha Mini Firmware and Mobile App

The **Alpha Mini Mobile Application** and on-robot **Firmware** govern the operational state, safety barriers, and developer capabilities of the robot.

---

## Alpha Mini Mobile Application Overview

The mobile companion application (UBTECH AlphaMini, package version `AlphaMini_V1.3.2`) provides:
1. **Robot Binding & Provisioning:** Initial onboarding, user account binding, and BLE-based Wi-Fi pairing.
2. **Firmware Version Inspection:** Reading installed firmware build numbers.
3. **Over-The-Air (OTA) Push Upgrades:** Automated downloading and flashing of system software.
4. **Skills & Voice Interaction Dashboard:** Triggering native dances, skills, educational modules, and encyclopedia queries.
5. **System Settings:** Volume control, Wi-Fi switching, and toggling **Privacy Mode** (which disables the camera and microphone array).

---

## Serial Number Identification

Every Alpha Mini unit carries a unique hardware serial number:
- **Physical Location:** Located **under (not on)** the battery backpack chassis, directly below the printed barcode.
- **Format:** An alphanumeric string ending in a distinct sequence of digits (e.g. `...00879` or `...0090`).
- **Developer Usage:** The **last 5 digits** (or last few characters) are required by the Python SDK method `MiniSdk.get_device_by_name("00879", 10)` to identify and resolve the robot over local mDNS broadcast.

---

## Firmware Version Requirements

- **Minimum Required Version:** Firmware must be **greater than v1.4.0** (e.g., standard production firmware `V1.6.0.50`).
- **Why Firmware > v1.4.0 is Required:**
  - Firmware versions prior to v1.4.0 lack the internal socket daemon server on port 8800.
  - Versions v1.4.0+ introduce the double-tap eye LCD display of the robot's local IP address.
  - Required for third-party developer programming mode (`MiniSdk.enter_program()`).
- **Upgrade Procedure:**
  1. Connect Alpha Mini to a stable Wi-Fi network with Internet access.
  2. Open the AlphaMini mobile app and connect to the robot.
  3. If an update is available, the app displays a firmware upgrade prompt.
  4. Keep the robot connected to mains power during the OTA flash process. Do not power off while updating.

---

## Inter-Node Connectivity
- Setup Guide: [[AlphaMiniDevelopmentEnvironmentSetup]]
- Network Pairing: [[AlphaMiniWiFiAndNetworkConfiguration]]
- Python SDK Connection: [[AlphaMiniConnectionLifecycle]]
- User Manual Specifications: [[AlphaMiniHardwareSpecifications]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 16, `SRA Lesson 2 - v1.0.pptx` & Labsheet 01:**
> *"Setup - Upgrade Alpha Mini*
> *Alpha Mini firmware version must be greater than v1.4.0*
> *Check version through Mobile App*
> *Please read here for instructions to connect robot to Mobile App*
> *Auto/push upgrade via the Mobile App*
> *Flip the robot over to find the serial number under (not on) the battery backpack. The serial number is a long string of alphabets and digits below the barcode, typically ending with series of number. Take note of the last 5 digits."*
