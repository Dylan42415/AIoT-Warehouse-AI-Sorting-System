# Theory: TensorFlow Lite and TFL-Micro

This document synthesizes Chapter 6 (`Introduction to TensorFlow Lite and TFL-Micro`, Prof. Marcelo Rovai, UNIFEI), explaining the compilation, conversion, quantization, and runtime execution pipeline for edge intelligence.

---

## 1. The TensorFlow Ecosystem Hierarchy

```
  +-----------------------------------------------------------+
  |  Full TensorFlow (Python)                                 |
  |  - Server, Workstation, Cloud GPUs                        |
  |  - Training, Backpropagation, Gigabytes of RAM            |
  +-----------------------------+-----------------------------+
                                | TFLite Converter
                                v
  +-----------------------------------------------------------+
  |  TensorFlow Lite (C++ / Mobile)                           |
  |  - Android, iOS, Raspberry Pi SBC                         |
  |  - Hardware acceleration (GPU, NPU, NEON), Megabytes RAM  |
  +-----------------------------+-----------------------------+
                                | Flattening & Stripping
                                v
  +-----------------------------------------------------------+
  |  TensorFlow Lite for Microcontrollers (TFLM)             |
  |  - Bare-metal microcontrollers (Arm Cortex-M4F, ESP32)    |
  |  - Zero OS dependency, Zero dynamic heap, Kilobytes RAM   |
  +-----------------------------------------------------------+
```

---

## 2. Model Conversion & The FlatBuffers Format

Standard TensorFlow models saved as `.h5` or SavedModel bundles are bloated with training metadata, gradients, and optimizer states. The **TensorFlow Lite Converter** strips this data:
1. **Operator Fusion:** Merges consecutive operations (e.g., Convolution + Bias Add + ReLU activation) into a single optimized mathematical kernel.
2. **Serialization to FlatBuffers:** Formats the computational graph and weights into a `.tflite` binary file based on Google FlatBuffers.
3. **C++ Byte Array Generation (`xxd`):**
   ```bash
   xxd -i model_quantized.tflite > model_data.cc
   ```
   Converts the binary file into an array:
   ```cpp
   const unsigned char g_model_data[] = { 0x1c, 0x00, 0x00, 0x00, ... };
   const int g_model_data_len = 18712;
   ```
   This array is placed directly into microcontroller **Flash ROM**, consuming zero SRAM at boot.

---

## 3. Post-Training Quantization (PTQ)

Microcontrollers lack fast 64-bit/32-bit floating point hardware units. **Quantization** maps 32-bit floating point weights and activations ($	ext{float32}$) to 8-bit signed integers ($	ext{int8}$):

### Mathematical Mapping Formula:
$$r = S 	imes (q - Z)$$
$$q = 	ext{round}\left(rac{r}{S}ight) + Z$$
Where:
- $r$ = Real-world continuous floating-point number.
- $q$ = Quantized 8-bit integer ($\in [-128, 127]$).
- $S$ = Scale factor (positive real floating-point multiplier).
- $Z$ = Zero-point offset (integer representing the quantized value of real zero $0.0$).

### Benefits for Embedded Hardware:
- **$4	imes$ Reduction in Memory Footprint:** 32-bit floats require 4 bytes; 8-bit integers require 1 byte.
- **Dramatically Faster Execution:** Integer SIMD instructions on the Arm Cortex-M4F execute in a fraction of the clock cycles required for software or hardware floating point operations.
- **Lower Power Consumption:** Memory fetches from Flash consume up to 80% of inference energy; reducing model size directly extends battery life.

---

## 4. TFLM Runtime Architecture on Microcontrollers

```
  +--------------------------------------------------------------+
  |                   TensorArena Memory Buffer                  |
  |  [ Model Scratchpad ] [ Intermediate Tensor Activations ]    |
  +--------------------------------------------------------------+
                                 ^
                                 | Static Allocation
  +------------------------------+-------------------------------+
  |  TFLM Runtime Engine:                                        |
  |  1. tflite::MicroMutableOpResolver<N>                        |
  |     - Registers ONLY the specific math kernels needed.       |
  |  2. tflite::MicroInterpreter                                 |
  |     - Orchestrates graph execution sequentially.             |
  +--------------------------------------------------------------+
```

1. **Selective Operator Registration (`MicroMutableOpResolver`):** Instead of linking all 140+ TensorFlow operators, only the exact operators used by the model (e.g., `AddConv2D`, `AddMaxPool2D`, `AddReshape`) are compiled into Flash, saving hundreds of kilobytes.
2. **Static TensorArena:** Dynamic memory allocation (`malloc`) causes memory fragmentation and unpredictable crashes in embedded systems. TFLM mandates pre-allocating a static byte array:
   ```cpp
   constexpr int kTensorArenaSize = 73 * 1024; // 73 KB
   uint8_t tensor_arena[kTensorArenaSize];
   ```
   All input, output, and intermediate activation tensors share this memory space.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 6_Introduction to TensorFlow Lite and TFL-Micro.pdf` (Pages 1-36)
> 
> "16. Introduction to TensorFlow Lite and TFL-Micro. Prof. Marcelo Rovai, UNIFEI... TensorFlow Lite is an open-source deep learning framework for on-device inference... The conversion process takes a SavedModel and generates a FlatBuffer .tflite file. Quantization converts 32-bit floats to 8-bit integers using scale and zero-point parameters, reducing model size by 4x and speeding up inference... TFLM runs on bare-metal systems using a static memory pool called TensorArena."
