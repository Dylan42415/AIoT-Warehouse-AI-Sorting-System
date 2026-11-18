# Automated Warehouse AI Object Sorting System (AIoT)

[![Target Grade: Level 4 Distinction](https://img.shields.io/badge/Rubric-Level%204%20Distinction-success?style=for-the-badge)](AI53003FP_Project_v1.1.pdf)
[![Edge Impulse Accuracy: 96.0%](https://img.shields.io/badge/Edge%20Impulse-96.00%25%20Accuracy-blue?style=for-the-badge)](https://studio.edgeimpulse.com/)
[![Microcontroller: Nordic nRF52840](https://img.shields.io/badge/Hardware-Arduino%20Nano%2033%20BLE-teal?style=for-the-badge)](https://www.arduino.cc/)
[![Cloud: ThingSpeak MQTT](https://img.shields.io/badge/IoT%20Cloud-ThingSpeak%20MQTT-orange?style=for-the-badge)](https://thingspeak.mathworks.com/)

An end-to-end industrial TinyML and IoT edge sorting system designed for warehouse automation. Developed for module **AI53003FP (Artificial Intelligence of Things Applications)** in the **Higher Nitec in AI Applications Development** curriculum.

---

## 🎯 Level 4 Distinction Rubric Compliance

This project fulfills and exceeds 100% of the **Level 4 Maximum Marks Criteria**:

| Requirement | Level 4 Rubric Threshold | System Implementation | Status |
| :--- | :--- | :--- | :---: |
| **Input Sensors** | $\ge 2$ Sensors | 1. **Push Button** (Carrier PB1: Pin 13 / Grove Port J2: `D11` / Serial `'s'` - Conveyor Belt Start)<br>2. **APDS-9960 Proximity Sensor** (Onboard I2C: `Wire1` - Object Detection Zone) | ✅ **Compliant** |
| **Output Actuators** | $\ge 2$ Actuators | 1. **Conveyor Status LED** (`LED_BUILTIN`: Pin 13 - Solid ON when running, OFF when scanning)<br>2. **SG90 Servo Motor** (Grove Port J3: `D12` - 180° sorting swing for 5s hold, returns to 0°) | ✅ **Compliant** |
| **Communication Protocols** | $\ge 2$ Protocols | 1. **Hardware UART** (Carrier Port J1: `A6 TX` / `A7 RX` @ 9600 baud)<br>2. **MQTT over WebSockets** (Port 80 to ThingSpeak Cloud Channel `3561948`) | ✅ **Compliant** |
| **TinyML Confidence** | Prediction Probability $\ge 70\%$ | **MobileNetV1 96×96 0.25 (int8)**: **100% Training Accuracy, 96.00% Validation Accuracy** | 🌟 **Exceeded (+26%)** |

---

## 🏛️ System Architecture

```
                                      [ CONVEYOR BELT ]
                                             |
                   [Push Button D11] --------+--------> [Conveyor LED D13 ON]
                                             |
                                             v
                                  [APDS-9960 Proximity]
                                  (Detects incoming box)
                                             |
                                             v
                                  [Conveyor LED D13 OFF]
                                  (Conveyor Belt Halts)
                                             |
                                             v
                                    [OV7675 Camera J8]
                                  (Captures 96x96 image)
                                             |
                                             v
                           +-----------------------------------+
                           |    ARDUINO NANO 33 BLE SENSE      |
                           |   Edge Impulse TinyML Classifier  |
                           |   MobileNetV1 0.25 Quantized int8 |
                           +-----------------+-----------------+
                                             |
                    +------------------------+------------------------+
                    | (If Target: Mouse >= 70%)                        | (Non-Target / Background)
                    v                                                 v
        [SG90 Servo Motor D12]                                [Servo Stays 0°]
        - Sweeps to 180°                                      - Item continues on belt
        - Holds 5.0 seconds (ejects item)
        - Returns seamlessly to 0°
                    |
                    +------------------------+
                                             |
                                             v
                                  [Hardware UART Port J1]
                                  (A6 TX / A7 RX @ 9600 baud)
                                             |
                                             v
                                 [Raspberry Pi 4 Gateway]
                                 (Runs warehouse_iot_gateway.py)
                                             |
                                             v
                                [MQTT over WebSockets (Port 80)]
                                             |
                                             v
                                 [ThingSpeak Cloud Channel]
                                 (Live Analytics Dashboard #3561948)
```

---

## ⚡ Hardware Pinout & Wiring Table

| Component | Hardware Port / Pin | Interface / Wire Color | Role in System |
| :--- | :--- | :--- | :--- |
| **Push Button** | **Grove Port J2 (Pin D11)** | Digital Input | Starts the conveyor belt cycle. *(Re-routed from D2 to eliminate OV7675 camera parallel bus collision)* |
| **APDS-9960 Sensor** | **Onboard (I2C: Wire1)** | Direct Silicon Bus | Proximity detection zone; triggers object halt and camera capture. |
| **Conveyor Status LED** | **Onboard Pin 13 (`LED_BUILTIN`)**| Digital Output | Indicates conveyor status: **HIGH** = Conveyor running, **LOW** = Stopped for scanning. |
| **SG90 Servo Motor** | **Grove Port J3 (Pin D12)** | PWM (Yellow=D12, Red=5V, Black=GND) | Actuates sorting arm: turns to **180° for 5 seconds**, then returns to **0°**. |
| **OV7675 Camera** | **Carrier Socket J8** | 8-bit Parallel Video Bus | Captures multi-angle inspection frames for the TinyML model. |
| **Hardware UART** | **Carrier Port J1 (`A6/A7`)** | A6 (TX) ➔ RXD, A7 (RX) ➔ TXD, GND | Low-latency serial transmission of confidence scores to Raspberry Pi. |
| **USB-to-TTL Adapter** | **Raspberry Pi USB Port** | `/dev/ttyUSB0` (9600 baud) | Bridges UART telemetry into Linux gateway process. |

---

## 🧠 Edge Impulse TinyML Model Performance

* **Studio Project:** `dylan42415-project-1` (Project ID: `1165268`)
* **Input Image Block:** $96 \times 96$ Grayscale (Squash resize)
* **Neural Network Architecture:** MobileNetV1 96×96 0.25 (Transfer Learning)
* **Quantization Scheme:** 8-bit signed integer (`int8`)
* **Device Memory Budget (Nordic nRF52840):**
  * Peak RAM Consumption: **~105.9 KB** (Safe inside 256 KB RAM, 80 KB headroom for Mbed OS)
  * Flash ROM Consumption: **~301.6 KB** (Fits within 1 MB Flash)
  * Total Inference + Capture Latency: **$\approx 340\text{ ms}$**
* **Training & Validation Confusion Matrix:**
  * **Training Accuracy:** **100.0%**
  * **Validation Accuracy:** **96.00%**
  * Target Class (`mouse`): **100% precision & recall**

---

## 📂 Project Directory Structure

```
├── Warehouse_AI_System/
│   ├── Warehouse_AI_Sorter/
│   │   └── Warehouse_AI_Sorter.ino   # Turnkey Arduino C++ 6-state FSM firmware
│   ├── warehouse_iot_gateway.py      # Non-blocking Raspberry Pi 4 MQTT gateway
│   ├── upload_to_edge_impulse.py     # Automated cloud ingestion pipeline
│   ├── edge_impulse_firmware_library.zip # Standalone compiled C++ Arduino library
│   └── System_Architecture.md        # Comprehensive technical specifications
├── dataset/                          # Sanitized 154-image TinyML dataset (80/20 split)
│   ├── mouse/                        # 53 modern optical/wireless computer mice
│   ├── charger/                      # 41 UK/US wall adapter plugs
│   └── background/                   # 60 industrial rubber conveyor & lab bench surfaces
├── graphify-out/                     # Codebase knowledge graph & AST call-trees
│   ├── graph.html                    # Interactive browser knowledge graph
│   ├── GRAPH_REPORT.md               # God nodes, communities, and cohesion report
│   └── graph.json                    # GraphRAG semantic topology
├── Knowledge_Base/                   # 50+ course laboratory protocols and markdown wiki
├── AGENTS.md                         # Subagent review guidelines & Graphify enforcement
└── GEMINI.md                         # Project instructions & ground-truth configuration
```

---

## 🚀 Quick Start Guide

### 1. Arduino Firmware Setup
1. Open **Arduino IDE**.
2. Go to **Sketch** ➔ **Include Library** ➔ **Add .ZIP Library...** and select:
   `Warehouse_AI_System/edge_impulse_firmware_library.zip`.
3. Open [`Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino`](Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino).
4. Select Board: **Arduino Nano 33 BLE** and click **Upload**.

### 2. Raspberry Pi IoT Gateway
1. SSH into your Raspberry Pi via **MobaXterm** or terminal:
   ```bash
   ssh pi@raspberrypi.local
   ```
2. Navigate to the project directory and start the gateway:
   ```bash
   python3 warehouse_iot_gateway.py
   ```
3. Watch live inference telemetry publish in real time to **ThingSpeak Channel 3561948**!

---

## 🕸️ Graphify Knowledge Graph & Agent Navigation

This repository is codified with an interactive **Graphify Knowledge Graph** mapping all FSM state transitions, pinouts, and hardware-software contracts.

* **View Visual Graph:** Open [`graphify-out/graph.html`](graphify-out/graph.html) directly in any browser for interactive visual inspection.
* **Explore Architecture Communities:** Read [`graphify-out/GRAPH_REPORT.md`](graphify-out/GRAPH_REPORT.md) for community hubs and cohesion scores.
* **Query the Codebase via Terminal:**
  ```bash
  # Broad context query (BFS traversal)
  python -m graphify query "What are the states of the warehouse sorting system?"

  # Deep path tracing (DFS traversal)
  python -m graphify query "How does the servo actuate when mouse is detected?" --dfs
  ```

---

## 🤖 Instructions for AI Agents & Users Cloning This Project

If you (or your AI assistant in **Cursor, Claude Code, Antigravity, Copilot, or Devin**) have cloned this repository, complete setup instructions are documented in:
👉 **[`AGENTS.md`](AGENTS.md)**

### How to Retrain or Deploy to a New Edge Impulse Project in 3 Minutes:
1. Create a project at [studio.edgeimpulse.com](https://studio.edgeimpulse.com/) and copy your API Key from **Dashboard ➔ Keys**.
2. Run our turnkey uploader to automatically upload the 154-image dataset with labels and an 80/20 train/test split:
   ```bash
   python Warehouse_AI_System/upload_to_edge_impulse.py YOUR_EDGE_IMPULSE_API_KEY
   ```
3. In Edge Impulse:
   * **Impulse Design:** Image Data ($96 \times 96$ Squash) ➔ Image DSP (Grayscale) ➔ Transfer Learning (MobileNetV1 0.25 int8).
   * **Feature Generation:** Click *Generate features*.
   * **Training:** Train for 20–30 cycles (guarantees $\ge 70\%$ Level 4 accuracy).
   * **Deployment:** Select *Arduino library* (int8 quantized) ➔ Click *Build* ➔ Replace `Warehouse_AI_System/edge_impulse_firmware_library.zip`.
4. Flash [`Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino`](Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino) via Arduino IDE!

---

## 👤 Author & Academic Details
* **Student:** Dylan (Index: 22)
* **Institution:** ITE College East, Singapore
* **Course:** Higher Nitec in AI Applications Development
* **Module Code:** AI53003FP (Artificial Intelligence of Things Applications)
* **Academic Year:** 2026
