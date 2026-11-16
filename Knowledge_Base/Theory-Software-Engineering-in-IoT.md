# Theory: Software Engineering in IoT

This document synthesizes Chapter 2 of the AIoT module (`AI53003FP`), covering the embedded software stack, programming paradigms, data serialization formats, and Application Programming Interfaces (APIs).

---

## 1. The Embedded and IoT Software Stack

Software execution in IoT spans across the hierarchical layers:
1. **Perception / Edge Firmware:** Bare-metal or RTOS firmware written in C/C++ running on [[Arduino-Nano-33-BLE-Sense|microcontrollers]], managing hardware peripherals, ADC sampling, and [[TensorFlow-Lite-for-Microcontrollers-TFLM|TinyML model execution]].
2. **Gateway / Fog Layer Software:** Python scripts and services executing on Linux-based [[Single-Board-Computer-Raspberry-Pi-Gateway|Single Board Computers (SBCs)]], performing protocol translation, buffering, and upstream routing.
3. **Cloud Application Software:** Web servers, time-series databases, and REST APIs managing data dashboards ([[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak]]).

---

## 2. Programming Languages: C/C++ vs. Python

| Attribute | C / C++ (Embedded) | Python (Gateway & Cloud) |
| :--- | :--- | :--- |
| **Execution Model** | Ahead-of-time compiled to native machine instructions | Bytecode compiled, interpreted by CPython runtime |
| **Memory Management**| Manual or static memory allocation; zero GC pauses | Automatic garbage collection; dynamic heap |
| **Execution Speed** | Ultra-fast execution with predictable latency | Slower execution; higher overhead |
| **Memory Footprint** | Extremely low (tens of kilobytes) | High (requires megabytes of RAM) |
| **Typical Role in AIoT**| Running sensor drivers and [[TensorFlow-Lite-for-Microcontrollers-TFLM|TFLM inference]] on Nano 33 BLE | Orchestrating MQTT, HTTP REST, and data processing on SBC |

---

## 3. Data Serialization Protocols: JSON vs. Binary

Telemetry transmitted across network boundaries must be serialized into standardized payloads:

### JavaScript Object Notation (JSON):
- Text-based, human-readable data interchange format.
- Consists of key-value pairs, objects (`{}`), and arrays (`[]`).
- Example JSON payload transmitted to IoT clouds:
  ```json
  {
    "device_id": "arduino_nano_01",
    "timestamp": 1686000000,
    "sensors": {
      "proximity": 15,
      "temperature": 24.5
    },
    "inference": {
      "person_detected": true,
      "confidence": 0.89
    }
  }
  ```

### FlatBuffers & Protocol Buffers (Binary Serialization):
- Schema-based binary formats designed for zero-copy deserialization.
- Crucial in TinyML: TensorFlow Lite models are serialized into `.tflite` FlatBuffers files, allowing execution directly from Flash without deserialization overhead.

---

## 4. Representational State Transfer (REST) APIs

REST is an architectural style for distributed hypermedia systems communicating over HTTP:
- **Stateless:** Each client request contains all necessary context for the server to process it.
- **Standard HTTP Methods:**
  - `GET`: Retrieve representation of a resource (used in [[Lab-A-How-to-Send-Data-from-Arduino-to-Cloud|Lab A]] to update ThingSpeak channels via URL query strings).
  - `POST`: Create a new resource or append telemetry.
  - `PUT`: Update/replace an existing resource.
  - `DELETE`: Remove a resource.
- **HTTP Status Codes:**
  - `200 OK`: Request succeeded.
  - `400 Bad Request`: Invalid parameters or malformed payload.
  - `401 / 403`: Unauthorized or forbidden (invalid API key).
  - `500 Internal Server Error`: Server failure.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 2_Software is Everywhere.pdf` (Pages 1-61)
> 
> "Software is everywhere in IoT. Programming languages such as Python and C/C++ allow developers to write code for microcontrollers, edge devices, and cloud servers... Data is exchanged using serialization standards like JSON, which provides a lightweight, human-readable format for transmitting structured data between devices and web APIs... RESTful APIs use standard HTTP verbs (GET, POST) to communicate telemetry data to cloud dashboards."
