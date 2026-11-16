# AIoT Knowledge Base: Master Knowledge Graph & Index

Welcome to the **Comprehensive AIoT Knowledge Base** for Module **`AI53003FP: Artificial Intelligence of Things`**. This knowledge graph is structured as a fully interconnected, zero-summarization, Obsidian-compatible local context base. Every document, hardware specification, theory concept, communication protocol, step-by-step laboratory sheet, student code example, and reference guide from the source curriculum has been ingested with complete technical fidelity and verbatim traceability.

---

## 1. High-Level AIoT System Architecture

The module curriculum spans the cyber-physical computing continuum, integrating ultra-low-power edge sensing, on-device machine learning inference, fog gateway protocol conversion, and cloud analytics:

```
+-------------------------------------------------------------------------------------------------------+
|                                        AIOT SYSTEM CONTINUUM                                          |
+-------------------------------------------------------------------------------------------------------+

  [ PERCEPTION / EDGE NODE ]                   [ FOG GATEWAY NODE ]               [ CLOUD ANALYTICS ]
  [[Arduino-Nano-33-BLE-Sense]]                [[Single-Board-Computer-Raspberry-Pi-Gateway]]                [[Cloud-ThingSpeak-IoT-Analytics-Platform]]             Analytics-Platform]]
  - Onboard MEMS Sensors Suite:                - Linux OS Runtime                  - 8-Field Telemetry Ingestion
    * [[APDS-9960-Digital-Sensor]] (Proximity) - Protocol Translation Engine       - Live Visual Dashboard Charts
    * [[LSM9DS1-9-Axis-IMU]] (Motion)          - Local Cache / Persistence         - REST API & MQTT Ingestion
    * [[MP34DT05-A-MEMS-Microphone]] (Audio)   - Remote Admin: SSH & VNC           - MATLAB Analytics Scripts
    * [[HTS221-Humidity-Temperature-Sensor]]     * [[Lab-04-Accessing-SBC-Remotely]]
    * [[LPS22HB-Barometric-Pressure-Sensor]]
    * [[ATECC608A-Crypto-Authentication-Element]]
  - Hardware Expansion Baseboard:
    * [[Machine-Learning-Carrier-V1.0]]
    * [[OV7675-Camera-Module]] (DVP Video)
  - Real-Time TinyML Inference Engines:
    * [[TensorFlow-Lite-for-Microcontrollers-TFLM]]
    * [[Person-Detection-TinyML-Pipeline]]
    * [[Keyword-Spotting-KWS-TinyML-Pipeline]]
    * [[Edge-Impulse-Platform-and-Deployment]]
  - Actuation & Closed-Loop Control:
    * [[PWM-Servo-Motor-Control]]
    * [[Hardware-Interrupts-and-ISR-Programming]]

                |                                         |                                  |
                +============= PHYSICAL INTERCONNECTS =====+==================================+
                |                                         |
                |-- [[Protocol-UART-Serial-Communication]]|-- [[Protocol-MQTT-Message-Queuing-Telemetry]]
                |   * USB Virtual Serial (/dev/ttyACM0)   |   * Broker: mqtt3.thingspeak.com:1883
                |   * Hardware UART Pins (TX/RX)          |   * Topic: channels/<ID>/publish
                |-- [[Protocol-Bluetooth-Low-Energy-GATT]]|-- [[Theory-Software-Engineering-in-IoT|HTTP REST]]
                |   * GAP Peripheral / GATT Server        |   * GET: https://api.thingspeak.com/update
                |   * Custom 128-bit Service & Char UUIDs |
```

---

## 2. Master Navigation Index by Domain

### Domain I: Hardware Specifications & Pinout Architecture
Exhaustive hardware reference manuals, pin multiplexing, electrical limits, and schematics:
- [[Arduino-Nano-33-BLE-Sense]]: Core microcontroller SoC specifications, nRF52840, MPM3610 power regulator, operating boundaries.
- [[Nano-33-BLE-Sense-Pinout-Architecture]]: Complete 30-pin table, physical header pins JP1/JP2, internal nRF52 port mappings (`P0.xx`/`P1.xx`), power limits.
- [[Machine-Learning-Carrier-V1.0]]: Hardware schematic breakdown, headers `J1`-`J8`, button `PB1`, solder jumpers `SJ1`/`SJ2`.
- [[OV7675-Camera-Module]]: OmniVision VGA sensor, SCCB register control, DVP 8-bit parallel video bus, $96 \times 96$ grayscale subsampling.
- [[APDS-9960-Digital-Sensor]]: Proximity, ambient light, RGB color, and gesture sensor; I2C address `0x39`, interrupt line `P0.19`.
- [[LSM9DS1-9-Axis-IMU]]: 3-axis accelerometer, 3-axis gyroscope, 3-axis magnetometer; internal bus `Wire1`.
- [[MP34DT05-A-MEMS-Microphone]]: Digital omnidirectional PDM microphone, 1.024 MHz clock, 16 kHz 16-bit PCM decimation.
- [[HTS221-Humidity-Temperature-Sensor]]: Relative humidity and temperature sensor; factory calibration interpolation, I2C `0x5F`.
- [[LPS22HB-Barometric-Pressure-Sensor]]: 260–1260 hPa piezoresistive barometer; 24-bit resolution, hypsometric altitude formula, I2C `0x5C`.
- [[ATECC608A-Crypto-Authentication-Element]]: Hardware cryptographic co-processor; ECDSA, SHA-256, root of trust, I2C `0x60`.
- [[PWM-Servo-Motor-Control]]: TowerPro SG90 servo motor; 50 Hz PWM timing, 1.0ms–2.0ms pulse widths, power separation.
- [[Single-Board-Computer-Raspberry-Pi-Gateway]]: Raspberry Pi architecture, Linux OS environment, USB/UART ports, role as fog gateway.

---

### Domain II: Theory Notes & Core Architectural Concepts
Curriculum lecture notes (`02_Theory Notes`, Chapters 1–9):
- [[Theory-Sensors-Actuators-and-Microcontrollers]]: (Chapter 1) Electronics terms, Ohm's law, sensor/actuator taxonomy, MCU vs MPU architectures.
- [[Theory-Software-Engineering-in-IoT]]: (Chapter 2) Software stack, C/C++ vs Python, data serialization (JSON vs FlatBuffers), REST APIs.
- [[Theory-Network-Fog-and-Cloud-Computing]]: (Chapter 3) Network scopes (PAN, LAN, WAN), topologies, IPv4/IPv6, Cloud vs Fog vs Edge trade-offs.
- [[Theory-Introduction-to-TinyML]]: (Chapter 4) The 4 pillars of edge intelligence (Latency, Bandwidth, Privacy, Power), embedded resource limits.
- [[Theory-Machine-Learning-Applications-and-Workflow]]: (Chapter 5) ML paradigms, end-to-end ML lifecycle, Confusion Matrix, Accuracy, Precision, Recall, F1.
- [[Theory-TensorFlow-Lite-and-TFL-Micro]]: (Chapter 6) TensorFlow ecosystem, TFLite Converter, FlatBuffers format, `TensorArena`, `MicroInterpreter`.
- [[Theory-TinyML-Kit-Hardware-and-Software]]: (Chapter 7) Kit mating, board packages, library installation, hardware bring-up diagnostics.
- [[Theory-Computer-Vision-and-Image-Classification]]: (Chapter 8) Digital image tensors, 2D convolutions, pooling, MobileNet, depthwise separable convolutions.
- [[Theory-Audio-Processing-and-Keyword-Spotting]]: (Chapter 9) Acoustic signal physics, sampling theorem, FFT, STFT, Spectrograms, Mel scale, MFCC, 2D CNNs.

---

### Domain III: Communication Protocols & Cloud Services
Networking protocols, hardware interrupts, and cloud platform architecture:
- [[Protocol-UART-Serial-Communication]]: Framing format (8N1), baud rate matching, physical crossover wiring, `Serial` vs `Serial1` vs Mbed `UART`.
- [[Protocol-Bluetooth-Low-Energy-GATT]]: BLE physical layer, GAP advertising, GATT client-server hierarchy, 128-bit UUIDs, characteristic properties.
- [[Protocol-MQTT-Message-Queuing-Telemetry]]: Publish-subscribe model, QoS 0/1/2, packet structure, ThingSpeak MQTT broker authentication.
- [[Cloud-ThingSpeak-IoT-Analytics-Platform]]: Channel architecture, field allocations 1–8, Write/Read API keys, 15-second rate limits, REST GET/POST.
- [[Hardware-Interrupts-and-ISR-Programming]]: NVIC interrupt controller, ISR coding rules, `volatile` qualifier, switch contact bounce, `millis()` debouncing.

---

### Domain IV: TinyML & Edge Machine Learning Deep Dive
Embedded deep learning pipelines and model optimization:
- [[TinyML-Core-Principles-and-Workflow]]: Zero dynamic allocation rule, integer arithmetic dominance, operator whitelisting, energy budgets.
- [[TensorFlow-Lite-for-Microcontrollers-TFLM]]: C++ runtime architecture, `MicroMutableOpResolver`, `MicroInterpreter`, static `TensorArena` memory layout.
- [[Model-Quantization-PTQ-Int8]]: Post-training quantization mathematics, Scale factor ($S$), Zero-point ($Z$), symmetric vs asymmetric quantization.
- [[Convolutional-Neural-Networks-for-Edge-Vision]]: 2D convolution kernel mathematics, depthwise separable convolution speedup ($8\times$–$9\times$).
- [[Person-Detection-TinyML-Pipeline]]: $96 \times 96$ grayscale pipeline, MobileNet int8 topology, output logits, LED indicator actuation.
- [[Audio-Feature-Extraction-and-Spectrograms]]: PDM decimation, Hamming windowing, FFT, Mel filterbanks, 2D MFCC spectrogram generation.
- [[Keyword-Spotting-KWS-TinyML-Pipeline]]: Micro speech pipeline, 16 kHz sampling, 2D CNN classification (`silence`, `unknown`, `yes`, `no`), temporal smoothing.
- [[Edge-Impulse-Platform-and-Deployment]]: Edge Impulse studio workflow, data forwarder CLI, Impulse blocks, C++ library export.

---

### Domain V: Laboratory Execution Protocols (Hands-On Labs)
Complete, step-by-step procedural guides for all 17 lab modules:
- [[Lab-01-Setup-Arduino-Software-IDE]]: Installing Arduino IDE 2.3.x, Mbed OS core, libraries, bootloader double-tap recovery.
- [[Lab-02-Run-Your-First-Program]]: Sketch anatomy (`setup` / `loop`), `LED_BUILTIN` blink, `Serial.begin(9600)`, Serial Monitor/Plotter.
- [[Lab-03-Proximity-Sensor-Protocol]]: APDS-9960 initialization on `Wire1`, reading proximity values (0=closest, 255=furthest), threshold trigger.
- [[Lab-04-Accessing-SBC-Remotely]]: Headless Raspberry Pi setup, SSH terminal access via MobaXterm (port 22), VNC desktop via RealVNC (port 5900).
- [[Lab-05-Setup-Cloud-ThingSpeak]]: Creating ThingSpeak channel, configuring fields 1–3, obtaining Write API Key, testing manual HTTP GET updates.
- [[Lab-06-Update-ThingSpeak-Channel-from-SBC-by-MQTT]]: Setting up MQTT Device, `paho-mqtt` on SBC, publishing CPU metrics to `channels/<ID>/publish`.
- [[Lab-07-Arduino-to-SBC-Communication]]: Wiring USB/UART serial between Nano 33 BLE and SBC, identifying `/dev/ttyACM0`, Python `pyserial` script.
- [[Lab-08-Integrating-Data-Transmission-TinyML-to-ThingSpeak-via-SBC]]: End-to-end integration: Nano 33 BLE TinyML $\rightarrow$ SBC gateway $\rightarrow$ ThingSpeak MQTT.
- [[Lab-09-Person-Detection]]: Hardware assembly on ML Carrier, OV7675 camera alignment, running Harvard TinyMLx Person Detection, LED indicators.
- [[Lab-10-TinyML-Keyword-Spotting]]: MP34DT05 microphone setup, running `micro_speech`, recognizing spoken keywords `"yes"` and `"no"`.
- [[Lab-11-Servo-Motor-Control-Program]]: Wiring TowerPro SG90 servo, 5V power separation, `Servo.h` library, programming 0°–180° sweeps.
- [[Lab-12-Integrating-Proximity-Sensor-and-Servo-Motor-Control]]: Closed-loop cyber-physical system: APDS-9960 proximity trigger actuates SG90 servo barrier.
- [[Lab-13-Button-Interrupt]]: Wiring pushbutton `PB1` to D12, configuring `attachInterrupt()` on `FALLING`, `volatile` counter, `millis()` debouncing.
- [[Lab-14-Edge-Impulse-Data-Collection]]: Installing Edge Impulse CLI, connecting `edge-impulse-data-forwarder`, sampling 3-axis accelerometer data at 50Hz.
- [[Lab-15-Edge-Impulse-Image-Classification-Protocol]]: Capturing camera dataset, designing MobileNetV2 impulse, int8 quantization, exporting Arduino zip library.
- [[Lab-16-Bluetooth-Communication]]: Configuring Nano 33 BLE as GATT server, custom Service and Characteristic UUIDs, wireless LED control.
- [[Lab-A-How-to-Send-Data-from-Arduino-to-Cloud]]: Kenneth Foo tutorial: Arduino APDS-9960 $\rightarrow$ USB serial $\rightarrow$ Python HTTP GET $\rightarrow$ ThingSpeak.

---

### Domain VI: Student Code Implementations & Reference Scripts
Verbatim source code listings with architectural and line-by-line analysis:
- [[Code-Arduino-BLE-Callback-Basic]]: `BLE_Callback_Basic.ino` (GATT service, characteristic written event handlers, `BLE.poll()`).
- [[Code-Arduino-Button-Interrupt-Debounce]]: `Button_Interrupt.ino` (Hardware interrupt, volatile flag, 200ms debounce lockout, software UART).
- [[Code-Arduino-UART-Basic]]: `UART_Basic.ino` (Mbed auxiliary UART instance on pins A6/A7, bidirectional transmission).
- [[Code-Python-SBC-01-ThingSpeak-Basic]]: `01_thingSpeak_Basic.py` (Publishing `psutil` CPU metrics to ThingSpeak MQTT broker via `paho.mqtt.publish`).
- [[Code-Python-SBC-02-USB-ThingSpeak]]: `02_USB_thingSpeak_Basic.py` (Reading serial stream on `/dev/ttyUSB0` and forwarding to ThingSpeak MQTT).
- [[Code-Python-SBC-03-BLE-ThingSpeak]]: `03_BLE_thingSpeak_Basic.py` (Asynchronous BLE client using `bleak.BleakClient` forwarding to ThingSpeak MQTT).
- [[Code-Python-SBC-04-UART-Duplex]]: `04_UART_Duplex_Basic.py` (Full-duplex serial exchange on `/dev/ttyUSB0`).
- [[Code-Python-SBC-05-UART-ThingSpeak]]: `05_UART_thingSpeak_Basic.py` (Reading secondary serial port `/dev/ttyACM1` and publishing to ThingSpeak MQTT).
- [[Code-Python-SBC-BLE-Scanner]]: `discover_py.py` (Asynchronous BLE peripheral radio discovery using `bleak.discover()`).

---

### Domain VII: Reference Guides & Curated External Resources
Deep technical references, programming cheat sheets, and media libraries:
- [[Reference-Arduino-Programming-Notebook]]: Brian W. Evans Arduino Programming Notebook v1.1 exhaustive syntax, structure, data types, and I/O guide.
- [[Reference-Python-3-Cheat-Sheet]]: Laurent Pointal Python 3 Cheat Sheet exhaustive reference for types, containers, slicing, and exception handling.
- [[Reference-Curated-AIoT-Video-Index]]: Complete table of all 31 video lectures with exact YouTube URLs, descriptions, and syllabus mapping.
- [[Reference-Harvard-TinyMLx-Hands-On-Vision-Audio]]: Brian Plancher Harvard TinyMLx deep dive: OV7675 registers, PDM microphone pipeline, memory profiling.

---

### Domain VIII: Major Course Projects & Capstone Architectures
End-to-end turnkey project blueprints, Level 4 system designs, and integration protocols:
- [[Project-Warehouse-AI-Sorting-System-Architecture]]: Automated Warehouse AI Object Sorting System (Level 4 distinction: Push Button D2, APDS-9960 Proximity Sensor, Conveyor LED D4, Servo Motor D12 180° 5s, Edge Impulse MobileNet TinyML, Raspberry Pi 4 Gateway via UART A6/A7, ThingSpeak MQTT Cloud Telemetry).

---

## 3. Knowledge Graph Maintenance & Integrity Notes

- **Zero Summarization Standard:** All pin numbers, baud rates, register names, memory limits, and mathematical formulas are explicitly documented.
- **Traceability Guarantee:** Every wiki node contains an unambiguous `## Traceability & Source Excerpts` section with exact citations from the source repository.
- **Wikilink Density:** Every related hardware component, theory concept, communication protocol, and lab protocol is bidirectionally linked.
