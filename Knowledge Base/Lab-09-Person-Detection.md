# Lab 09: Person Detection

This document provides the exhaustive laboratory execution protocol for **Lab 09: Person Detection** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Assemble the [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier]] with the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] and [[OV7675-Camera-Module|OV7675 camera]].
2. Load and compile the `person_detection` sketch from the `Harvard_TinyMLx` library.
3. Configure the camera downsampling pipeline ($96 \times 96$ grayscale).
4. Run live image classification on the quantized MobileNet model inside [[TensorFlow-Lite-for-Microcontrollers-TFLM|TFLM]].
5. Actuate onboard RGB LEDs and serial indicators based on classification scores.

---

## 2. Hardware Assembly & Verification

1. Insert the Nano 33 BLE Sense into headers `JANALOG1` and `JDIGITAL1` on the carrier.
2. Insert the OV7675 camera into socket `J8`.
3. Connect to PC via Micro-USB.
4. Manually adjust the camera focus ring to ensure crisp focus at 30–50 cm.

---

## 3. Sketch Workflow & Output Logic

1. The sketch initializes the camera via SCCB (`0x21` / `0x42`) and starts capturing video frames.
2. Each frame is downsampled to $96 \times 96$ pixels in 8-bit grayscale ($9,216\text{ bytes}$).
3. The image is loaded into the interpreter input tensor `input->data.int8`.
4. `interpreter->Invoke()` evaluates the convolutional model.
5. Classification outputs:
   - `person_score`
   - `no_person_score`
6. Output Actuation:
   - **Person Detected (`person_score > no_person_score`):** Green LED turns ON; Red LED turns OFF.
   - **No Person (`person_score <= no_person_score`):** Red LED turns ON; Green LED turns OFF.

Detailed in [[Person-Detection-TinyML-Pipeline|Person Detection TinyML Pipeline]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab09_Person_Detection.pdf` (Pages 1-6)
> 
> "Person detection example from Harvard_TinyMLx library... Uses OV7675 camera mounted on TinyML carrier to capture 96x96 grayscale images... MobileNet model runs in TensorArena to classify person vs no-person, lighting green LED when person is detected and red LED when no person is present."
