# Alpha Mini Wi-Fi and Network Configuration

The **Alpha Mini Robot** communicates with host computers, cloud services, and mobile controllers using IP networking over 2.4 GHz Wi-Fi or 4G LTE cellular data.

---

## Network Provisioning Architecture

```
+--------------------------------------------------------------------+
|                       Mobile Phone Application                     |
|                                                                    |
|  1. BLE Pairing ("Start Binding")                                  |
|  2. Sends Wi-Fi SSID & Password via BLE to Alpha Mini              |
+---------------------------------+----------------------------------+
                                  | Bluetooth Low Energy (2.4 GHz)
                                  v
+--------------------------------------------------------------------+
|                       Alpha Mini Robot (Embedded)                  |
|                                                                    |
|  - Connects to 2.4GHz WLAN (WPA2-PSK)                              |
|  - Obtains Local IP via DHCP                                       |
|  - Advertises Service via mDNS / DNS-SD (Port 8800)                |
|  - Displays SSID + IP across Dual LCD Eyes on Head Double-Tap      |
+---------------------------------+----------------------------------+
                                  |
                                  | Local Subnet (LAN / Hotspot)
                                  v
+--------------------------------------------------------------------+
|                       Host PC (Python SDK)                         |
|                                                                    |
|  - Scans mDNS broadcast packets for Robot Serial Suffix            |
|  - Connects via WebSocket / TCP socket directly to Robot IP:Port   |
+--------------------------------------------------------------------+
```

---

## Network Requirements & Constraints

1. **2.4 GHz Wi-Fi Only:** Alpha Mini only supports **2.4 GHz WLAN (IEEE 802.11 b/g/n)** with WPA/WPA2-PSK encryption. It cannot connect to 5.0 GHz Wi-Fi bands or enterprise networks requiring 802.1X PEAP certificates.
2. **Subnet Isolation Warning (CRITICAL):** The host PC running the Python SDK and the Alpha Mini **must reside on the exact same Layer-2 subnet**. If the school or corporate network implements client isolation (AP isolation) or blocks mDNS/multicast traffic, device discovery will fail.
   > **Operational Rule from Labsheet 01:** *"Using the Alpha Mini app, switch your robot to the mobile hotspot SSID. Same for your computer. Otherwise, your python code will not be able to reach the robot."*
3. **4G LTE Cellular Data Alternative:**
   - Alpha Mini features a Nano-SIM (4FF) card slot located under the left armpit.
   - Used for standalone operation, voice AI cloud access, and outdoor navigation. Note that local Python SDK socket control still requires a shared Wi-Fi/hotspot network.

---

## Step-by-Step Wi-Fi Provisioning Protocol

### Phase 1: Mobile App Setup & Bluetooth Pairing
1. Download the **UBTECH AlphaMini App** from Google Play Store or Apple App Store.
2. Enable Bluetooth on the mobile device.
3. Sign in to the designated UBTECH account.
4. Locate the robot's serial number on the barcode label situated under the backpack. Record the trailing 5 digits.
5. Place the phone adjacent to the robot and tap **"Start Binding"** (or **"Set up internet"**).
6. The app establishes a BLE handshake with Alpha Mini.

### Phase 2: Wi-Fi Credentials Transmission
1. Select the target 2.4 GHz Wi-Fi SSID (or Mobile Hotspot SSID) in the mobile app.
2. Enter the Wi-Fi password and submit.
3. Alpha Mini audibly announces its network connection progress. Once completed, it announces its online status.

### Phase 3: Hardware Verification via Head Double-Tap
1. Gently **double-tap the top capacitive touch sensor** on the robot's head.
2. **Visual Eye Feedback:**
   - The left and right LCD eyes will illuminate and render the connected Wi-Fi SSID.
   - On updated firmware (v1.4.0+), the assigned local IPv4 address (e.g. `192.168.43.120`) is displayed directly beneath the SSID.
3. **LED Mouth Feedback:**
   - If offline, the mouth LED flashes red 3 times periodically.
   - If connected, the mouth LED flickers white or stays illuminated.

### Phase 4: Reconfiguring Wi-Fi
If switching to another access point:
1. Open the AlphaMini app.
2. Tap the **"Me"** icon on the bottom navigation toolbar.
3. Select **"Wi-Fi settings"** -> **"Change Wi-Fi"**.
4. Re-enter new SSID credentials over BLE.

---

## Inter-Node Connectivity
- Environment Setup: [[AlphaMiniDevelopmentEnvironmentSetup]]
- Connection Verification: [[AlphaMiniConnectionLifecycle]]
- Buttons & LED Matrix: [[AlphaMiniButtonsAndLEDIndicators]]
- Firmware Requirements: [[AlphaMiniFirmwareAndMobileApp]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 01 & Slide 15, `SRA Lesson 2 - v1.0.pptx`:**
> *"Double tap on the head and the eyes will reveal its Wi-Fi connection status.*
> *Launch the AlphaMini app and sign-in using the given account and password for your robot. The app will ask for your permission to enable Bluetooth. Your robot should complain that it is not connected to the internet.*
> *Click on button Start Binding or the text that says, 'Set up internet'. This will instruct you to put your phone next to the robot. Using Bluetooth, it will communicate with the app to run the Wi-Fi selection. Choose SSID and enter the corresponding password. Throughout, the robot will announce its status and once connected to the internet will tell you so.*
> *[WARNING] Using the Alpha Mini app, switch your robot to the mobile hotspot SSID. Same for your computer. Otherwise, your python code will not be able to reach the robot."*
