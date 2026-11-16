# Theory: Introduction to TinyML

This document synthesizes Chapter 4 (`TensorFlow Lite for microcontrollers` by Daniel Situnayake) and foundational TinyML principles, detailing why and how machine learning is deployed onto ultra-low-power microcontrollers.

---

## 1. What is TinyML?

**TinyML (Tiny Machine Learning)** is a subfield of machine learning that enables on-device sensor data analytics on ultra-low-power microcontrollers (typically consuming **under 1 milliwatt** of power). It brings deep learning inference to edge endpoints without relying on cloud servers or internet connectivity.

---

## 2. The Four Pillars of Edge Intelligence (Why TinyML?)

```
                       +-------------------+
                       |    TINYML CORE    |
                       |    ADVANTAGES     |
                       +---------+---------+
                                 |
         +----------------+------+-------+----------------+
         |                |              |                |
         v                v              v                v
    [ LATENCY ]      [ BANDWIDTH ]   [ PRIVACY ]     [ POWER & COST ]
   Zero network     Sends insights,  Raw biometric/  Milliwatt draw,
   round-trip lag   not raw streams  camera data     battery operable
                                     stays on-chip   for years
```

1. **Zero Latency (Instantaneous Inference):** Cloud round trips introduce 50 ms to several seconds of network latency. TinyML provides deterministic, local inference suitable for safety-critical systems and real-time triggers.
2. **Bandwidth Conservation:** Streaming raw video (e.g., 30 fps VGA = ~9 MB/s) or high-frequency multi-axis accelerometer streams from billions of sensors would choke global network bandwidth. TinyML processes raw data locally and transmits only high-level semantic insights (e.g., "Person Detected").
3. **Uncompromising Privacy & Security:** Sensitive raw data (e.g., in-home cameras, medical biometric sensors, voice audio) never leaves the physical microcontroller chip, eliminating data interception risks.
4. **Always-On Low Power Operation:** Battery-operated devices can run continuously for months or years on coin-cell batteries, waking larger systems only when a trigger event is classified.

---

## 3. Microcontroller Constraints vs. Cloud ML

| Metric | Cloud ML Server | Microcontroller ([[Arduino-Nano-33-BLE-Sense|nRF52840]]) | Factor Difference |
| :--- | :--- | :--- | :--- |
| **Power Budget** | 200W – 500W per GPU | $pprox 10	ext{ mW} - 50	ext{ mW}$ | $10,000	imes$ lower |
| **Memory (RAM)** | 32 GB – 128 GB | **256 KB** | $500,000	imes$ lower |
| **Storage (Flash)** | Terabytes (NVMe SSD) | **1 MB** | $1,000,000	imes$ lower |
| **Floating Point** | TeraFLOPS (FP32/FP16) | 64 MHz single-precision FPU | Highly constrained |
| **Dynamic Memory** | Dynamic heap allocation | **No dynamic heap**; static arena | Deterministic |

---

## 4. The TensorFlow Lite for Microcontrollers (TFLM) Paradigm

To execute complex neural networks inside 256 KB of RAM, Google designed **TensorFlow Lite for Microcontrollers (TFLM)**:
- Written in portable, dependency-free C++11.
- Requires no operating system, standard C libraries, or dynamic memory allocation (`malloc`/`free`).
- Interprets models compiled into serialized C++ byte arrays (`FlatBuffers`).
- Pre-allocates a single contiguous memory block called the [[TensorFlow-Lite-for-Microcontrollers-TFLM|TensorArena]] for all model weights, activations, and temporary scratchpads.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 4_Simple Introduction of tinyML.pdf` (Pages 1-25)
> 
> "TensorFlow Lite for microcontrollers: Deploy machine learning models on tiny devices. Daniel Situnayake (@dansitu). TensorFlow Lite is our production ready, cross-platform framework for deploying ML models on mobile and edge devices... TinyML allows inference on microcontrollers with less than 1MB of memory and milliwatt power budgets, preserving privacy, eliminating network latency, and drastically cutting bandwidth requirements."
