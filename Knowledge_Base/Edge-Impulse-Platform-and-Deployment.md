# Edge Impulse Platform and Deployment

**Edge Impulse** is a developer platform for building, training, and deploying embedded machine learning models to edge devices, simplifying the workflow from data collection to C++ deployment.

---

## 1. Development Workflow Architecture

```
  1. Data Ingestion:
     Arduino Nano 33 BLE ===(Serial Forwarder / WebUSB)===> Edge Impulse Studio
         |
         v
  2. Impulse Design:
     [ Time Series / Image Data ] --> [ Processing Block ] --> [ Learning Block ]
     (Raw Accelerometer / Camera)     (Spectral Analysis/Image) (NN Classifier/FOMO)
         |
         v
  3. Model Training & Validation:
     - Quantize to int8
     - Review Confusion Matrix, Precision, Recall, and Memory Estimates
         |
         v
  4. Deployment Export:
     - C++ Library Export (Self-contained Arduino .zip library)
     - Flashed directly to Arduino Nano 33 BLE Sense
```

---

## 2. Impulse Building Blocks

An **Impulse** in Edge Impulse combines:
1. **Input Data Block:** Sets sample frequency (e.g., 50 Hz for IMU, 96x96 for camera) and window size.
2. **Processing Block:** Extracts features:
   - **Spectral Analysis:** For IMU motion sensors (FFT, filterbanks).
   - **Image Processing:** Color-to-grayscale downsampling, normalization.
3. **Learning Block:**
   - **Classification (Keras/TensorFlow):** Fully connected or 2D CNN classifier.
   - **Anomaly Detection:** K-means clustering to detect novel abnormal patterns.

---

## 3. Associated Labs

- **[[Lab-14-Edge-Impulse-Data-Collection|Lab 14 Data Collection]]:** Using `edge-impulse-data-forwarder` to stream accelerometer data from Nano 33 BLE Sense.
- **[[Lab-15-Edge-Impulse-Image-Classification-Protocol|Lab 15 Image Classification]]:** Training a custom image classifier and deploying it as an Arduino library.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab14_Edge_Impulse_DataCollection.pdf` & `Lab15_Edge_Impulse_Image_Classification.pdf`
> 
> "Edge Impulse is a leading development platform for machine learning on edge devices... You collect data from the Arduino Nano 33 BLE Sense using the CLI data forwarder, design an impulse with processing and learning blocks, train the model, and export it as an Arduino C++ library."
