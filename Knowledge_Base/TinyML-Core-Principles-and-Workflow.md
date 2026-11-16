# TinyML Core Principles and Workflow

**Tiny Machine Learning (TinyML)** represents the intersection of ultra-low-power embedded microcontrollers and machine learning algorithm design. It enables on-device inference directly on sensors without sending telemetry to cloud or edge servers.

---

## 1. Hardware Resource Boundaries

Microcontroller targets (such as the [[Arduino-Nano-33-BLE-Sense|Nordic nRF52840]] on the Arduino Nano 33 BLE Sense) impose strict operational boundaries:

```
  Traditional Deep Learning (Cloud Server)         TinyML (Embedded MCU)
  +---------------------------------------+        +---------------------+
  | RAM: 32 GB – 128 GB                   |  vs.   | RAM: 256 KB         |
  | Flash Storage: Terabytes NVMe         |        | Flash: 1 MB         |
  | Power: 250W – 500W                    |        | Power: < 50 mW      |
  | OS: Linux / CUDA / PyTorch            |        | OS: Bare-Metal/RTOS |
  +---------------------------------------+        +---------------------+
```

---

## 2. The 4 Golden Rules of TinyML Engineering

1. **Static Memory Allocation Only:** The runtime must not invoke dynamic heap allocation (`malloc` or `new`) during inference. Memory fragmentation leads to fatal heap exhaustion crashes in long-running embedded systems.
2. **Integer Arithmetic Dominance:** Floating-point operations ($	ext{float32}$) consume excessive cycles and power. Models must be quantized to 8-bit signed integers ($	ext{int8}$) via [[Model-Quantization-PTQ-Int8|Post-Training Quantization]].
3. **Operator Whitelisting:** Standard frameworks link hundreds of unused math kernels. TinyML engines only link the exact subset of kernels required by the target model architecture.
4. **Energy-Proportional Operation:** The microcontroller stays in deep sleep modes ($\mu	ext{A}$ draw) and wakes via hardware interrupts only when sensor thresholds are crossed.

---

## 3. End-to-End TinyML Deployment Pipeline

```
  1. Data Collection & Labeling (Sensors / Edge Impulse)
         |
         v
  2. Model Training & Validation (TensorFlow / Keras in FP32)
         |
         v
  3. Optimization & Quantization (TFLite Converter -> Int8 PTQ)
         |
         v
  4. FlatBuffers C++ Export (xxd -> const unsigned char g_model_data[])
         |
         v
  5. Bare-Metal Execution via TFLM Interpreter (TensorArena)
```

1. **Data Collection:** Continuous sensor capture via [[Lab-14-Edge-Impulse-Data-Collection|Edge Impulse]] or serial forwarders.
2. **Model Training:** Training compact CNN or dense architectures using desktop/cloud GPUs.
3. **Quantization:** Transforming weights and activation tensors from 32-bit floats to 8-bit integers.
4. **Byte Array Compilation:** Converting `.tflite` FlatBuffers into a static C++ array placed in Flash memory.
5. **Microcontroller Inference:** Evaluating the model using [[TensorFlow-Lite-for-Microcontrollers-TFLM|TFLM]] inside the allocated `TensorArena`.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 4_Simple Introduction of tinyML.pdf` & `Chapter 5_ML Applications Overview.pdf`
> 
> "TinyML brings deep learning to microcontrollers with sub-milliwatt power draw and less than 1MB of storage... The workflow moves from data collection and model design to quantization and bare-metal compilation without dynamic memory allocation."
