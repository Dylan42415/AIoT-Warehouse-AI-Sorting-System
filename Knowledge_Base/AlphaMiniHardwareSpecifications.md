# Alpha Mini Hardware Specifications

This document catalogs the complete, uncompressed technical, electrical, mechanical, and radio frequency specifications of the **Alpha Mini Robot** manufactured by UBTECH Robotics Corp Ltd.

---

## Mechanical & Dimensional Parameters

| Parameter | Specification | Engineering Notes |
| :--- | :--- | :--- |
| **Product Name** | AlphaMini Robot | Humanoid companion and educational robot |
| **Product Model** | Alpha Mini | Model designation in firmware and compliance documentation |
| **Physical Dimensions** | Height: 245 mm<br>Width: 149 mm<br>Thickness: 112 mm | Actual size may vary slightly depending on manufacturing tolerances |
| **Total Weight** | Approximately 700 g | Including internal battery pack and servos |
| **Degrees of Freedom (Joints)** | 14 Precision Digital Servos Total | • Head: 3 joints (pan, tilt, nod)<br>• Arms: 4 joints (2 per shoulder/arm assembly)<br>• Waist: 1 joint<br>• Legs: 6 joints (hip, knee, ankle configurations) |
| **Chassis Materials** | High-impact ABS / Polycarbonate structural shell | Features backpack carry handle / handy grips |

---

## Computing, Memory & Operating System

| Component | Specification | Technical Details |
| :--- | :--- | :--- |
| **Processor Platform** | Embedded ARM Multi-core SoC | Runs customized embedded Android OS |
| **Operating System** | Android Embedded System (Firmware > v1.4.0, e.g. V1.6.0.50) | Houses internal daemon services for vision, ASR, locomotion, and socket servers |
| **System Memory (RAM)** | 2 GB LPDDR | Shared between Android runtime and computer vision pipelines |
| **Internal Storage (ROM)** | 16 GB eMMC Flash | Note: Android OS and system partitions occupy storage; user storage is less |
| **Firmware Requirement** | Must be > v1.4.0 | Required for third-party Python SDK socket binding and programming mode |

---

## Power, Battery & Electrical Subsystem

| Parameter | Specification | Verbatim Operational Threshold |
| :--- | :--- | :--- |
| **Battery Chemistry** | Lithium-ion Polymer (Li-Polymer) | Integrated rechargeable battery pack |
| **Rated Voltage** | 3.85 V DC nominal | Standard high-density lithium polymer operational range |
| **Battery Capacity** | 4060 mAh (Typical) | High-capacity internal cell |
| **Battery Failure Threshold** | Fail to reach 80% after repeated charging, OR 100% exhaustion in < 15 min | Criteria defining battery replacement / factory servicing |
| **Power Adapter Model** | A138A-120150U-EU4 (EU) / A138A-120150U-UK4 (UK) | Manufactured by Jiangxi Jian Aohai Technology Co., Ltd. |
| **Adapter Input** | AC 100 V – 240 V ~ 50/60 Hz, 0.5 A Max | Universal mains compatibility |
| **Adapter Output** | DC 5 V ⎓ 3 A / 9 V ⎓ 2 A / 12 V ⎓ 1.5 A | Multi-voltage fast charge negotiation over Type-C |
| **Charging Interface** | USB Type-C | Positioned at bottom base of backpack unit |
| **Charging Operating Pose** | Kneeling / sitting forward pose | Required when charging while powered off or in sleep mode |

---

## Vision, Audio & Display Subsystems

| Subsystem | Component Details | Functional Parameters |
| :--- | :--- | :--- |
| **Camera** | 13.0 Megapixels (13 MP) sensor | Forehead-mounted; autofocus, face detection, face tracking, object recognition |
| **Display Screens (Eyes)** | Dual 1.22-inch TFT LCD Displays | 240 × 240 pixels per screen, 262K colors; emotive animation rendering |
| **Mouth Indicator** | Tricolor RGB LED | White, Red, Green illumination; constant, flickering, or breathing modes |
| **Microphone Subsystem** | 4-Microphone Beamforming Array + 1 Communication Mic | Head-mounted 4-mic geometry for acoustic source localization; 1 dedicated comms mic |
| **Speakers** | 2-Channel Stereo Speakers, 0.8 W × 2 | Located laterally on torso / rib flanks |
| **Proximity Sensor** | Infrared Time-of-Flight (ToF) Sensor | Located centrally on chest; distance measurement in millimeters |
| **Capacitive Touch** | Top Head Touch Sensor | Supports single tap (interrupt action) and double tap (check Wi-Fi status) |
| **Motion Sensing** | 6-Axis IMU (Accelerometer + Gyroscope) | Internal fall detection, tilt detection, posture identification |

---

## Wireless Communication & Radio Frequency (RF) Specifications

| Communication Mode | Operating Frequency Range | Max Emission Power (dBm) | Protocol Standards |
| :--- | :--- | :--- | :--- |
| **Wi-Fi (WLAN)** | 2412 MHz – 2472 MHz (2.4 GHz band) | 16.80 dBm | IEEE 802.11 b/g/n |
| **Bluetooth Classic** | 2402 MHz – 2480 MHz | 9.532 dBm | Bluetooth 4.2 |
| **Bluetooth Low Energy (BLE)** | 2402 MHz – 2480 MHz | 1.146 dBm | BLE 4.2 (used for app binding) |
| **Cellular LTE Band 1** | Uplink: 1922.4 – 1977.6 MHz<br>Downlink: 2112.4 – 2167.6 MHz | 22.04 dBm | 4G FDD-LTE |
| **Cellular LTE Band 3** | Uplink: 1710.0 – 1785.0 MHz<br>Downlink: 1805.0 – 1880.0 MHz | 21.89 dBm | 4G FDD-LTE |
| **Cellular WCDMA Band 1** | Uplink: 1922.4 – 1977.6 MHz<br>Downlink: 2112.4 – 2167.6 MHz | 22.68 dBm | 3G UMTS / WCDMA |
| **Cellular WCDMA Band 8** | Uplink: 882.4 – 912.6 MHz<br>Downlink: 927.4 – 957.6 MHz | 22.16 dBm | 3G UMTS / WCDMA |
| **SIM Card Slot** | Nano-SIM (4FF form-factor), single slot | Under left armpit; requires ejector pin |
| **Satellite Positioning** | GPS Built-in receiver | Autonomous geolocation |

---

## Operating Conditions & Safety Boundaries

- **Operating Ambient Temperature:** 0 °C to 40 °C
- **Operating Clearance Zone:** Minimum 20 cm clear radius in all directions (front, rear, sides, above)
- **Control Methods:** Physical buttons (Power, Volume), Mobile Application (iOS/Android), Voice commands ("Hey Mini"), Python SDK over Wi-Fi socket connection.

---

## Inter-Node Connectivity
- Core Overview: [[AlphaMiniRobotOverview]]
- Physical Anatomy: [[AlphaMiniAnatomyAndStructure]]
- Indicators & Controls: [[AlphaMiniButtonsAndLEDIndicators]]
- Safety Protocols: [[AlphaMiniSafetyPrecautionsAndMaintenance]]
- SDK Integration: [[AlphaMiniPythonSDKArchitecture]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 8, `Alphamini_User_Manual.pptx`:**
> *"08 Product Specifications - User Manual*
> *Product name: AlphaMini Robot*
> *Product model: Alpha Mini*
> *Measurements: Height 245mm, Width 149mm, Thickness 112mm (The actual size may vary slightly depending on the configuration, manufacturing process, and measurement method)*
> *Weight: About 700g*
> *Joints: 14 joints in total (3 head joints, 4 arm joints, 1 waist joint, 6 leg joints)*
> *Network standard: 4G/3G*
> *Transmission function: WiFi 2.4G (WLAN 802.11 b/g/n), BT4.2, GPS*
> *User storage: RAM 2GB / ROM 16GB (As the Android system has taken up a certain storage, the user storage may be less than this value.)*
> *SIM card type: Nano SIM card (4FF, single card slot)*
> *Speakers: 2-channel stereo speakers, 0.8W\*2*
> *Microphones: 4-microphone array (used for voice interaction and source location), 1 communication microphone*
> *Display screen: 1.22 inch (actual visible area is slightly smaller), 262K color TFT display screen, 240\*240 pixels*
> *Camera: 13 million pixels*
> *Sensors: GPS, accelerometer, gyroscope, touch sensor, Infrared sensor*
> *Battery: Li-Polymer lithium polymer battery, 3.85V, 4060mAh (Typical)*
> *Power adapter: Manufacturer: Jiangxi Jian Aohai Technology Co., Ltd. Model: A138A-120150U-EU4 / A138A-120150U-UK4 Input AC 100V-240V~50/60Hz 0.5A; Output DC 5V 3A / 9V 2A / 12V 1.5A*
> *Operating temperatures: 0°C-40°C*
> *Control methods: Power button, volume button, App, voice control"*
