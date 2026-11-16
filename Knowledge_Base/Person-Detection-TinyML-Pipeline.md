# Person Detection TinyML Pipeline

The **Person Detection TinyML Pipeline** is a real-time visual presence detection system running on the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]], [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier]], and [[OV7675-Camera-Module|OV7675 camera]].

---

## 1. System Pipeline Architecture

```
  [ OV7675 Camera Sensor ]
        |
        v Captures VGA (640x480) YUV422 stream
  [ Center Crop & Downsample Engine ]
        |
        v Produces 96 x 96 Grayscale Image (9,216 bytes)
  [ Input Tensor Normalization (int8) ]
        |
        v Quantized range [-128, 127]
  [ MobileNet int8 CNN on TFLM ]
        |
        v Executes in static TensorArena (~73 KB)
  [ Classification Logits Output ]
        |
        v Class 0: no-person, Class 1: person
  [ Threshold & LED / UART Output ]
        - Green LED: Person Detected (Score > Threshold)
        - Red LED: No Person Detected
        - UART Serial: Transmits score to Raspberry Pi SBC
```

---

## 2. Model Specifications

| Parameter | Value |
| :--- | :--- |
| **Model Architecture** | MobileNet with Depthwise Separable Convolutions |
| **Input Tensor Shape** | `[1, 96, 96, 1]` (Batch 1, Height 96, Width 96, 1 Channel Grayscale) |
| **Input Data Type** | 8-bit signed integer (`int8_t`) |
| **Flash Memory Size** | $pprox 250	ext{ KB}$ |
| **RAM (TensorArena) Size** | $pprox 73	ext{ KB}$ |
| **Output Tensor Shape** | `[1, 2]` (`int8_t` logits for `[no_person, person]`) |
| **Inference Latency** | $pprox 200	ext{ ms} - 400	ext{ ms}$ on 64 MHz Cortex-M4F |

---

## 3. Code Implementation & Output Evaluation

In [[Lab-09-Person-Detection|Lab 09 Person Detection]]:

```cpp
// Evaluate model output
int8_t no_person_score = output->data.int8[kNoPersonIndex];
int8_t person_score    = output->data.int8[kPersonIndex];

// Convert quantized scores to approximate probabilities
float person_score_f = (person_score - output->params.zero_point) * output->params.scale;
float no_person_score_f = (no_person_score - output->params.zero_point) * output->params.scale;

if (person_score > no_person_score) {
  // Turn ON Green LED, turn OFF Red LED
  digitalWrite(LEDR, HIGH); // Active Low
  digitalWrite(LEDG, LOW);
  Serial.println("PERSON DETECTED!");
} else {
  // Turn ON Red LED, turn OFF Green LED
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  Serial.println("NO PERSON");
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab09_Person_Detection.pdf` (Pages 1-6) & `Reference_HandsOnVisionAudio.pdf`
> 
> "In person detection, the model takes a 96x96 grayscale image from the OV7675 camera and classifies whether a person is present or not... Output consists of two classes: person and no-person... TensorArena size is approximately 73 KB, executed on the nRF52840 processor using Harvard_TinyMLx library."
