# TensorFlow Lite for Microcontrollers (TFLM)

**TensorFlow Lite for Microcontrollers (TFLM)** is an ultra-lightweight open-source C++ inference engine designed by Google to execute deep learning models on bare-metal Arm Cortex-M and ESP32 microcontrollers.

---

## 1. TFLM Architectural Components

```
  +--------------------------------------------------------------+
  |                   Microcontroller Flash ROM                  |
  |  [ g_model_data[]: Flattened TFLite FlatBuffers Byte Array ] |
  +------------------------------+-------------------------------+
                                 |
                                 v Reads Model Graph
  +--------------------------------------------------------------+
  |                   tflite::MicroInterpreter                   |
  |  - tflite::MicroErrorReporter                                |
  |  - tflite::MicroMutableOpResolver<kNumberOps>                |
  +------------------------------+-------------------------------+
                                 |
                                 v Allocates Execution Memory
  +--------------------------------------------------------------+
  |              TensorArena (Contiguous uint8_t Array)          |
  |  [ Input Tensor ] [ Intermediate Activations ] [ Output ]    |
  +--------------------------------------------------------------+
```

### Core Engine Classes:
1. **`tflite::Model`:** C++ representation of the model FlatBuffers schema loaded via `tflite::GetModel(g_model_data)`.
2. **`tflite::MicroOpResolver`:** Resolves mathematical operation kernels. Using `MicroMutableOpResolver<N>` allows manually registering only the required operators:
   ```cpp
   static tflite::MicroMutableOpResolver<5> micro_op_resolver;
   micro_op_resolver.AddConv2D();
   micro_op_resolver.AddDepthwiseConv2D();
   micro_op_resolver.AddReshape();
   micro_op_resolver.AddFullyConnected();
   micro_op_resolver.AddSoftmax();
   ```
3. **`tflite::TensorArena`:** A statically pre-allocated byte array in SRAM:
   ```cpp
   constexpr int kTensorArenaSize = 73 * 1024; // 73 KB
   uint8_t tensor_arena[kTensorArenaSize];
   ```
   The interpreter automatically calculates activation lifetimes and overlaps memory buffers to minimize RAM usage.
4. **`tflite::MicroInterpreter`:** The execution controller instantiated with the model, resolver, arena pointer, and size.

---

## 2. Standard TFLM Inference Execution Loop

```cpp
#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

// 1. Model pointer & interpreter declarations
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 73 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

void setup() {
  // 2. Load model from Flash
  model = tflite::GetModel(g_model_data);

  // 3. Register operators
  static tflite::AllOpsResolver resolver;

  // 4. Instantiate interpreter
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // 5. Allocate tensors within TensorArena
  interpreter->AllocateTensors();

  // 6. Obtain input & output tensor pointers
  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  // 7. Load quantized sensor values into input->data.int8
  // ... (e.g. 96x96 grayscale image or audio spectrogram)

  // 8. Execute inference
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    // Handle error
    return;
  }

  // 9. Read predictions from output->data.int8
  int8_t score_class_0 = output->data.int8[0];
  int8_t score_class_1 = output->data.int8[1];
}
```

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 6_Introduction to TensorFlow Lite and TFL-Micro.pdf` (Pages 15-36)
> 
> "TFLM is designed specifically for microcontrollers without OS support or dynamic memory allocation... The MicroInterpreter executes models using static memory allocated in the TensorArena... OpResolvers allow loading only necessary kernels to minimize Flash usage."
