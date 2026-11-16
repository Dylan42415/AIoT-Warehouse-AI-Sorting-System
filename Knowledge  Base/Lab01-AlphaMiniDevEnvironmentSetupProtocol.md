# Lab 01: Setup AlphaMini Development Environment Protocol

This document provides the exhaustive lab manual and execution protocol for **Labsheet 01: Setup AlphaMini Development Environment**.

---

## Lab Objectives

In this lab, students learn how to:
1. Check the physical and network connection status of the Alpha Mini robot.
2. Find the unique hardware serial number identifier for the robot.
3. Configure the robot's Wi-Fi connection using the mobile application.
4. Download the official demo sample code repository for Alpha Mini.
5. Provision a dedicated Python virtual environment using Anaconda.
6. Install the AlphaMini Python SDK (`alphamini`).
7. Modify and execute the `test_connect.py` sample code to connect a PC to the robot over Wi-Fi.

---

## Tools, Equipment and Materials

- Personal Computer with Internet access (Windows 64-bit)
- Anaconda Distribution
- UBTECH Alpha Mini Mobile Application (iOS / Android)
- GitHub repository for AlphaMini sample codes (`https://github.com/marklogg/mini_demo`)
- Alpha Mini Robot
- Mobile Hotspot (or dedicated 2.4 GHz Wi-Fi router)
- USB Type-C Cable and Power Adapter (Jiangxi Jian Aohai Technology Co.)

---

## Step-by-Step Procedure

### Phase 1: Robot Inspection & Hardware Preparation
1. **Unboxing:** Carefully remove the robot from its custom Styrofoam box. Observe handling precautions (never pull or twist joints).
2. **Serial Number Extraction:** Flip the robot over to locate the serial number sticker placed **under (not on)** the battery backpack chassis.
   - Note the string of characters below the barcode.
   - **Record the last 5 digits** (e.g., `00879` or `0090`).
3. **Powering On:** Place the robot on a flat table with one knee bent forward and the other bent backward.
   - Press and hold the power button on the left side of the backpack until the mouth LED turns on.
   - Wait ~40 seconds for the robot to boot and announce *"I am ready"*.
   - If the battery is low, connect the provided USB-C cable and adapter to the mains.
4. **Verifying Wi-Fi Hardware Status:** Gently double-tap the top of the head touch sensor. The robot's dual LCD eyes will render its current Wi-Fi SSID and assigned IP address.

### Phase 2: Wi-Fi Provisioning via Mobile App
1. Download the **AlphaMini App** from App Store or Google Play.
2. Launch the app and sign in with the assigned robot account credentials. Grant Bluetooth permissions.
3. Tap **"Start Binding"** (or **"Set up internet"**). Place the smartphone adjacent to the robot.
4. The app communicates with Alpha Mini via BLE. Select the target 2.4 GHz SSID (e.g. Mobile Hotspot or `"ITE Lab1"`) and enter the network password.
5. Alpha Mini announces its connection progress and verbally confirms when online.
6. Verify connection by double-tapping the head sensor again to observe the SSID and IP on the LCD eyes.

### Phase 3: Workstation Workspace Setup
1. Create a workspace folder on the Desktop: `C:\Users\<Username>\OneDrive\Desktop\testAI`.
2. Open a web browser and navigate to `https://github.com/marklogg/mini_demo`.
3. Click the **Code** dropdown button, select **Download ZIP**, and save `mini_demo-master.zip` to the `testAI` folder.
4. Extract the ZIP archive into `Desktop/testAI/mini_demo-master`. Ensure files are unnested so that `Desktop/testAI/mini_demo-master/test/__init__.py` exists.

### Phase 4: Anaconda Environment Creation
1. Launch **Anaconda Prompt**.
2. Update the base Conda package manager:
```shell
conda update -n base -c defaults conda
```
3. Create the isolated virtual environment:
```shell
conda create --name testAI python=3.7 anaconda tensorflow=2.0
```
4. Enter `y` at the prompt `Proceed ([y]/n)?` to confirm installation.
5. Activate the new virtual environment:
```shell
conda activate testAI
```
*(The terminal prompt changes from `(base)` to `(testAI)`).*
6. Install the AlphaMini Python SDK:
```shell
pip install alphamini
```

### Phase 5: Code Modification & Connection Execution
1. In the Anaconda Prompt, navigate to the sample directory:
```shell
cd C:\Users\<Username>\OneDrive\Desktop\testAI\mini_demo-master\test
```
2. Open VS Code:
```shell
code .
```
3. In the Explorer panel, open `test_connect.py`.
4. Locate line 18 in `test_get_device_by_name()`:
```python
result: WiFiDevice = await MiniSdk.get_device_by_name("00879", 10)
```
5. Replace `"00879"` with the **last 5 digits** of your robot's serial number recorded in Step 2.
6. Verify robot type declaration: Ensure `MiniSdk.set_robot_type(MiniSdk.RobotType.DEDU)` or `RobotType.EDU` matches your robot variant.
7. Save the file (`Ctrl + S`).
8. Return to the Anaconda Prompt and run the script:
```shell
python test_connect.py
```
9. **Verification of Output:**
   - A blue/info log block prints the resolved `WiFiDevice` object (showing name, IP address, and port 8800).
   - The robot establishes socket connectivity, enters programming mode, and cleanly executes `shutdown()`.
   - Record the displayed IP address and Port for lab submission.
10. **Repacking:** Turn off the robot (hold power button 3 seconds) and carefully repack into the Styrofoam box with all cables and accessories.

---

## Inter-Node Connectivity
- Environment Overview: [[AlphaMiniDevelopmentEnvironmentSetup]]
- Network Pairing: [[AlphaMiniWiFiAndNetworkConfiguration]]
- Connection Code: [[AlphaMiniConnectionLifecycle]]
- Lab 2 Follow-up: [[Lab02-PythonAsyncioProgrammingProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 01 (`Labsheet 01 - Setup AlphaMini Development Environment.docx`):**
> *"Objective(s): In this lab, students will learn how to: Check the connection status of the robot; Find the identifier for the robot; Connect the robot to computer via USB to setup its Wi-Fi connection; Download the demo sample codes for AlphaMini; Setup Python virtual environment; Install AlphaMini Python SDK; Modify the test_connect.py sample code to connect computer to robot via Wi-Fi.*
> *conda create --name testAI python=3.7 anaconda tensorflow=2.0*
> *pip install alphamini*
> *Locate the line that says: result: WiFiDevice = await MiniSdk.get_device_by_name('00879', 10)*
> *Replace the number in '00879' with the last 5-digit from step 5, keeping the quotes.*
> *The output in blue box is due to the print command in line 21. It is the WiFiDevice object returned by the get_device_by_name(). The output in orange box is due to the shutdown(). Please note down your IP address and Port."*
