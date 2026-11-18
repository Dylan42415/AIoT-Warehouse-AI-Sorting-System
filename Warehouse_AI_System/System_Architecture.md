# Project: Automated Warehouse AI Object Sorting System Architecture

**Module:** `AI53003FP: Artificial Intelligence of Things Applications`  
**Scenario:** Warehouse Assistant System Engineer designing an Automated Conveyor Belt Sorting System  
**Target Performance:** **Level 4 (Maximum Distinction: 2 Sensors, 2 Actuators, 2 Protocols, Confidence $\ge 70\%$)**

---

## 1. System Overview & Architecture

The Integrated Product Sorting System automates warehouse package handling by simulating an industrial conveyor belt equipped with edge intelligence (TinyML) and cloud IoT connectivity:

```
+----------------------------------------------------------------------------------------------------------+
|                                    WAREHOUSE AI OBJECT SORTING CONTINUUM                                 |
+----------------------------------------------------------------------------------------------------------+

  [ PERCEPTION & ACTUATION ]                  [ FOG GATEWAY (SBC) ]              [ CLOUD ANALYTICS (IoT) ]
  Arduino Nano 33 BLE Sense + Carrier          Raspberry Pi 4 Model B             ThingSpeak Platform
  
  SENSORS (Inputs):                           - Linux OS Runtime                  - 8-Field Telemetry Channel
  1. Push Button (D11 / Grove J2)             - USB Serial /dev/ttyUSB0           - Channel ID: 3561948
     -> Starts Conveyor (avoids D2 camera bus)  - Python MQTT Gateway Script        - MQTT: mqtt3.thingspeak.com
  2. APDS-9960 Proximity Sensor (I2C)            * warehouse_iot_gateway.py        - Visual Time-Series Dashboard
     -> Detects arrival in scan zone          - Protocol Translation
  ACTUATORS (Outputs):
  1. Conveyor LED (Pin 13 / LED_BUILTIN)
     -> Indicates conveyor motion
  2. Servo Motor FT90B / SG90 (D12 / Grove J3)
     -> Pushes item at 180° for 5 sec

  EDGE AI (TinyML):
  - OV7675 DVP Camera Sensor
  - MobileNetV1 Transfer Learning Model
    * Trained on Edge Impulse (Quantized int8)
    * Classes: mouse, charger, background
    * Target Object: Computer Mouse (>= 70%)

             |                                          |                                    |
             +================ PHYSICAL INTERCONNECTS ==+====================================+
             |                                          |
             |-- Hardware UART Serial (A6/A7 -> J1)     |-- MQTT over WebSockets (Port 80)
             |   Baud: 9600, 8-N-1                      |   Topic: channels/3561948/publish
```

---

## 2. Hardware Pin Allocation & Wiring Table

| Hardware Component | Component Type | Physical Pin on Carrier / Board | Functionality in Warehouse |
| :--- | :--- | :--- | :--- |
| **Push Button** | Input Sensor 1 | **Grove Port `J2` (Pin `D11`)** | Starts the simulated conveyor belt. *(Aviods D2 camera parallel bus collision)* |
| **APDS-9960** | Input Sensor 2 | **Onboard (Internal I2C `Wire1`)** | Detects an object entering the scanning zone. |
| **LED Indicator** | Output Actuator 1 | **Onboard Pin 13 (`LED_BUILTIN`)** | Lights up when conveyor moves; turns off when stopped. |
| **Servo Motor** | Output Actuator 2 | **Grove Port `J3` (Pin `D12`)** | Pushes sorted object off belt: turns to **$180^\circ$ for 5s**, returns to $0^\circ$. |
| **OV7675 Camera** | Vision Sensor | **Carrier 24-Pin DVP Ribbon Slot `J8`** | Captures $96 \times 96$ image frames for inference. *(Uses D0–D9)* |
| **Grove-to-USB Cable** | Hardware UART | **Carrier Port `J1` (`A6` TX / `A7` RX)** | Sends prediction score to Raspberry Pi USB port (`/dev/ttyUSB0`). |

---

## 3. Finite State Machine (FSM) Control Flow

```mermaid
stateDiagram-v2
    [*] --> STATE_IDLE: System Boots
    
    STATE_IDLE --> STATE_CONVEYOR_RUNNING: Operator Presses Push Button (D11)
    note right of STATE_CONVEYOR_RUNNING: Conveyor LED Turns ON
    
    STATE_CONVEYOR_RUNNING --> STATE_OBJECT_DETECTED: APDS-9960 Proximity < Threshold
    note right of STATE_OBJECT_DETECTED: Conveyor Stops (LED Turns OFF)
    
    STATE_OBJECT_DETECTED --> STATE_INFERENCE: Camera Captures Frame
    
    STATE_INFERENCE --> STATE_SORTING_ACTION: Run TinyML MobileNet Classifier
    
    state STATE_SORTING_ACTION {
        [*] --> CheckTarget
        CheckTarget --> PushItem: Detected "mouse" & Confidence >= 70%
        PushItem: Servo to 180° for 5 seconds -> return to 0°
        CheckTarget --> IgnoreItem: Detected "charger" or "background"
        IgnoreItem: Servo stays at 0°
    }
    
    STATE_SORTING_ACTION --> STATE_TRANSMIT_CLOUD: Send Telemetry Packet over UART
    note right of STATE_TRANSMIT_CLOUD: Gateway publishes to ThingSpeak via MQTT
    
    STATE_TRANSMIT_CLOUD --> STATE_IDLE: Cycle Complete (Wait for Next Button)
```

---

## 4. Grading Rubric Compliance (Level 4 Verification)

| Evaluation Criterion | Minimum Level 1 | Target Level 4 | Our Implemented Architecture | Status |
| :--- | :---: | :---: | :--- | :---: |
| **Input Sensors** | 0 | **2** | 1. Push Button (Carrier PB1: Pin 13 / Grove J2: D11)<br>2. APDS-9960 Proximity Sensor (Wire1) | ✅ **Compliant (2 Sensors)** |
| **Output Actuators** | 0 | **2** | 1. Conveyor LED (Onboard RGB Pin 13/Green/Red)<br>2. Sorting Servo Motor (Grove J3: D12, 180° for 5s) | ✅ **Compliant (2 Actuators)** |
| **Comm Protocols** | 1 | **2** | 1. Hardware UART (A6 TX / A7 RX @ 9600)<br>2. MQTT over WebSockets (ThingSpeak Channel 3561948) | ✅ **Compliant (2 Protocols)** |
| **Prediction Confidence** | $< 50\%$ | **$\ge 70\%$** | MobileNetV1 96×96 0.25 Grayscale int8 ($96.00\%$ validation) | ✅ **Compliant ($\ge 70\%$)** |

---

## 5. Microcontroller Memory & Tensor Arena Optimization

| Model Configuration | Input Dimensions | Peak Tensor Arena | nRF52840 RAM Fit (256 KB) | Status |
| :--- | :---: | :---: | :---: | :---: |
| **RGB MobileNetV2 (Default)** | $96 \times 96 \times 3$ | **246,720 bytes (241 KB)** | ❌ Exceeds free heap (~215 KB) | Arena Alloc Error (`-6`) |
| **Grayscale MobileNetV1 0.25 (Production)** | $96 \times 96 \times 1$ | **~53,000 bytes (52 KB)** | ✅ Comfortably fits (>150 KB free) | **Instant live inference** |

---

## 6. Plug-and-Play File Locations

1. **Master Arduino Sketch:**  
   [Warehouse_AI_Sorter.ino](file:///c:/Users/Dfault/Downloads/AIOT_Project/Warehouse_AI_System/Warehouse_AI_Sorter/Warehouse_AI_Sorter.ino)
2. **Raspberry Pi Gateway Script:**  
   [warehouse_iot_gateway.py](file:///c:/Users/Dfault/Downloads/AIOT_Project/Warehouse_AI_System/warehouse_iot_gateway.py)
3. **Curated Clean Dataset:**  
   [C:\Users\Dfault\Downloads\AIOT_Project\dataset](file:///c:/Users/Dfault/Downloads/AIOT_Project/dataset) (`mouse`, `charger`, `background`)
