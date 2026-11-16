# Lab 15: Edge Impulse Image Classification Protocol

This document provides the exhaustive laboratory execution protocol for **Lab 15: Edge Impulse Image Classification** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Design an image classification Impulse in Edge Impulse Studio.
2. Ingest visual datasets from the [[OV7675-Camera-Module|OV7675 camera]].
3. Configure the **Image Processing Block** ($96 \times 96$ grayscale) and **Transfer Learning Block** (MobileNetV2).
4. Train, evaluate, and quantize the model to 8-bit signed integer (`int8`).
5. Export the trained model as a standalone C++ Arduino library (`.zip`) and flash it onto the Nano 33 BLE Sense.

---

## 2. Step-by-Step Impulse Design & Export

1. **Impulse Design:**
   - Input Block: Image data, $96 \times 96$ pixels, Grayscale.
   - Processing Block: **Image** (extracts normalized pixel tensor).
   - Learning Block: **Transfer Learning (Images)** (MobileNetV2 $96 \times 96$ 0.1 / 0.35).
2. **Model Training:** Train for 20–50 epochs. Review accuracy, confusion matrix, and on-device memory estimates (<256 KB RAM).
3. **Deployment Tab:**
   - Select **Arduino library**.
   - Enable **int8 quantization**.
   - Click **Build** to download the customized ZIP library.
4. **Arduino IDE Import:**
   - Navigate to `Sketch` > `Include Library` > `Add .ZIP Library...` and select the downloaded ZIP file.
   - Open the generated inferencing example under `File` > `Examples` > `[Project_Name]_inferencing` > `nano_ble33_sense_camera`.
   - Compile and upload to the board.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab15_Edge_Impulse_Image_Classification.pdf` (Pages 1-6)
> 
> "Design and deploy a computer vision image classifier using Edge Impulse... Set image size to 96x96 grayscale... Train a MobileNet transfer learning model, quantize to int8, and export as an Arduino C++ library... Include the library in Arduino IDE and run live image inference on the Nano 33 BLE Sense."
