# AIoT Project Guidelines & Subagent Review Policy

## 🕸️ Mandatory Codebase Review Rule: Use Graphify First
All autonomous subagents, researchers, code reviewers, and assistant turns reviewing this codebase **MUST** use the project's **Graphify Knowledge Graph** as their primary lens of inspection:

1. **Querying the Graph First:**
   * Before reading files one by one or guessing call trees, run `python -m graphify query "<query>"` to perform BFS/DFS traversal over the codified system graph (`graphify-out/graph.json`).
   * Read [GRAPH_REPORT.md](file:///c:/Users/Dfault/Downloads/AIOT_Project/graphify-out/GRAPH_REPORT.md) for community clusters, god nodes, and architectural cohesion.
2. **Inspection Hierarchy:**
   * **Level 1:** Query `graphify-out/graph.json` via `python -m graphify query "<question>"`.
   * **Level 2:** Read [GRAPH_REPORT.md](file:///c:/Users/Dfault/Downloads/AIOT_Project/graphify-out/GRAPH_REPORT.md) to inspect God Nodes (`SystemState`, `upload_dataset`) and community hubs.
   * **Level 3:** Target specific source files directly at their exact line markers discovered by Graphify.

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
* **Channel ID:** `3561948` | **Client ID/User:** `FCwmDyoGDTwwDDIABy0DASQ`
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
