# AIoT Project Guidelines & Subagent Review Policy

## 🕸️ Mandatory Codebase Review Rule: Use Graphify First
All autonomous subagents, researchers, code reviewers, and assistant turns reviewing this codebase **MUST** use the project's **Graphify Knowledge Graph** as their primary lens of inspection:

1. **Querying the Graph First:**
   * Before reading files one by one or guessing call trees, run `python -m graphify query "<query>"` to perform BFS/DFS traversal over the codified system graph (`graphify-out/graph.json`).
   * Read [GRAPH_REPORT.md](graphify-out/GRAPH_REPORT.md) for community clusters, god nodes, and architectural cohesion.
2. **Inspection Hierarchy:**
   * **Level 1:** Query `graphify-out/graph.json` via `python -m graphify query "<question>"`.
   * **Level 2:** Read [GRAPH_REPORT.md](graphify-out/GRAPH_REPORT.md) to inspect God Nodes (`SystemState`, `upload_dataset`) and community hubs.
   * **Level 3:** Target specific source files directly at their exact line markers discovered by Graphify.

---

## 🤖 Instructions for AI Agents Assisting Users Who Cloned This Repository

If you are an AI coding assistant (Antigravity, Claude Code, Cursor, Copilot, Devin, Windsurf) working with a user who cloned this repository, follow this standardized runbook:

### Phase 1: Assess User Goal
Ask the user whether they want:
* **Option A (Instant Plug-and-Play):** Use the pre-trained, pre-compiled Arduino library ([`Warehouse_AI_System/edge_impulse_firmware_library.zip`](Warehouse_AI_System/edge_impulse_firmware_library.zip)) which already achieves **96.0% accuracy**.
* **Option B (Train a Fresh Edge Impulse Model):** Guide them through the cloud MLOps training process from scratch using the automated dataset ingestion pipeline.

---

### Phase 2: Guiding the User to Setup Edge Impulse (Option B)

Follow these exact steps to ensure the user gets $\ge 70\%$ validation accuracy (Level 4 Rubric):

#### Step 1: Account & Project Creation
Instruct the user to:
1. Log into [studio.edgeimpulse.com](https://studio.edgeimpulse.com/).
2. Click **Create new project** and name it (e.g., `Warehouse-AI-Sorting`).
3. Navigate to **Dashboard** (left sidebar) ➔ Click the **Keys** tab at the top.
4. Copy their **Project API Key** (starts with `ei_...`).

#### Step 2: Automated Dataset Ingestion
Instruct the user to run the automated uploader script located in `Warehouse_AI_System/`:
```bash
python Warehouse_AI_System/upload_to_edge_impulse.py <THEIR_API_KEY>
```
* **What this does:** Automatically scans `dataset/mouse` (53 images), `dataset/charger` (41 images), and `dataset/background` (60 images), attaches the exact labels, and splits them into an **80% Training / 20% Testing** distribution via Edge Impulse's `/api/split/files` endpoint. Zero manual dragging required.

#### Step 3: Configure the Impulse
Guide the user to configure these exact parameters:
1. **Impulse Design** ➔ **Create Impulse**:
   * **Image Data:** Width: `96`, Height: `96`, Resize mode: `Squash`.
   * **Add Processing Block:** Select **Image**.
   * **Add Learning Block:** Select **Transfer Learning (Images)**.
   * Click **Save Impulse**.
2. **Image Processing Block (Left Sidebar):**
   * Color Depth: Select **Grayscale** *(Crucial: fits inside the 256 KB RAM of the Nordic nRF52840)*.
   * Click **Save parameters** ➔ Click **Generate features**.
3. **Transfer Learning Block (Left Sidebar):**
   * Neural network architecture: **MobileNetV1 96×96 0.25** (or MobileNetV2 0.1/0.35).
   * Number of training cycles: `20` to `30`.
   * Learning rate: `0.0005`.
   * Click **Start training**.
   * Verify the model achieves **$\ge 70\%$ Validation Accuracy** (our curated dataset typically yields **92% – 96%**).

#### Step 4: Export Arduino C++ Library
1. Navigate to the **Deployment** tab (left sidebar).
2. Under "Search deployment options", select **Arduino library**.
3. Under "Quantization", select **Quantized (int8)**.
4. Click **Build** to download the compiled `.zip` library.
5. Instruct the user to save it into `Warehouse_AI_System/` as `edge_impulse_firmware_library.zip`.

---

### Phase 3: Hardware Verification Checklist

When assisting the user with hardware hookup:
1. **Camera Bus Safety Warning:**
   * **DO NOT** use Pin `D2` or `D4` for the push button or external LEDs!
   * The OV7675 camera parallel data bus hardwires pins `D0` through `D9`. Connecting an external button to D2 causes a direct electrical short and camera failure.
   * **Button MUST be connected to Grove Port J2 (Pin D11)**.
2. **Servo Motor Power Brownout:**
   * Power the SG90 Servo Motor from an **external 5V rail** (e.g. Raspberry Pi Pin 2 or 4 [5V]), with shared GND.
   * Do not power the servo from the Nano's 3.3V pin, as motor inrush current will brown out the microcontroller during the 180° swing.
3. **Flashing the Board:**
   * Open Arduino IDE ➔ **Sketch** ➔ **Include Library** ➔ **Add .ZIP Library...** ➔ Select `edge_impulse_firmware_library.zip`.
   * Open [`Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino`](Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino).
   * Select Board: **Arduino Nano 33 BLE** and click **Upload**.

---

## 🏛️ Ground-Truth System Architecture

### 1. Embedded Firmware (`Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino`)
* **Core FSM States:** `STATE_IDLE` ➔ `STATE_CONVEYOR_RUNNING` ➔ `STATE_OBJECT_DETECTED` ➔ `STATE_INFERENCE` ➔ `STATE_SORTING_ACTION` ➔ `STATE_TRANSMIT_CLOUD`
* **Pin Allocation (Tiny ML Carrier):**
  * Push Button (Input Sensor 1): **Grove Port J2 (Pin D11)** *(DO NOT use D2 due to OV7675 camera parallel bus collision)*
  * Proximity Sensor (Input Sensor 2): **Onboard APDS-9960** (I2C: `Wire1`)
  * Conveyor Status LED (Output Actuator 1): **Onboard `LED_BUILTIN` (Pin 13)**
  * Sorting Servo Motor (Output Actuator 2): **Grove Port J3 (Pin D12)** (Swings 180° for 5 seconds, returns to 0°)
  * Hardware UART: **Carrier Port J1 (Pin A6 TX / Pin A7 RX)** @ 9600 baud

### 2. Raspberry Pi IoT Gateway (`Warehouse_AI_System/warehouse_iot_gateway.py`)
* **Serial Link:** Dynamic auto-port detection (`/dev/ttyUSB0`, `/dev/ttyUSB1`, `/dev/ttyACM0`) @ 9600 baud.
* **Cloud Broker:** ThingSpeak via MQTT over WebSockets (Port 80)
* **Channel ID:** `3581016` | **Client ID/User:** `MB0iNjAUGR8CKAQkDykpNgA`
* **Buffer Safety:** Non-blocking rate-limiting + `ser.reset_input_buffer()` after publish to prevent stale telemetry.

### 3. Edge Impulse TinyML Model
* **Project ID:** `1165268` (`dylan42415-project-1`)
* **Input Block:** $96 \times 96$ Grayscale (Squash)
* **Model Type:** MobileNetV1 0.25 (Quantized int8)
* **Performance:** **100% Training Accuracy**, **96.00% Validation Accuracy** (surpasses Level 4 $\ge 70\%$ threshold)
* **Compiled Arduino Package:** `Warehouse_AI_System/edge_impulse_firmware_library.zip`

---

## 🎯 Level 4 Grading Rubric Verification
* $\ge 2$ Input Sensors: Push Button (`D11`) + APDS-9960 Proximity Sensor ✅
* $\ge 2$ Output Actuators: Conveyor Status LED (`Pin 13`) + SG90 Servo (`D12`, 180° for 5s) ✅
* $\ge 2$ Protocols: Hardware UART (A6/A7) + MQTT WebSockets over Wi-Fi (ThingSpeak) ✅
* Prediction Confidence: $96\% \ge 70\%$ ✅
