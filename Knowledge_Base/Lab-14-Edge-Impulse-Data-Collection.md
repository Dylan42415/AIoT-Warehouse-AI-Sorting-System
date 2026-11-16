# Lab 14: Edge Impulse Data Collection

This document provides the exhaustive laboratory execution protocol for **Lab 14: Edge Impulse Data Collection** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Create and configure an embedded machine learning project on **Edge Impulse**.
2. Install Node.js and the **Edge Impulse CLI** tools on the host computer.
3. Stream multi-axis accelerometer telemetry from the [[LSM9DS1-9-Axis-IMU|LSM9DS1 IMU]] to the cloud using the `edge-impulse-data-forwarder`.
4. Collect, label, and partition training and testing datasets for gesture recognition.

---

## 2. CLI Setup & Data Forwarder Execution

1. Install Edge Impulse CLI:
   ```bash
   npm install -g edge-impulse-cli
   ```
2. Upload the data forwarding firmware to the Nano 33 BLE Sense (prints comma-separated sensor readings over Serial):
   ```cpp
   Serial.print(accX); Serial.print(",");
   Serial.print(accY); Serial.print(",");
   Serial.println(accZ);
   ```
3. Run the forwarder:
   ```bash
   edge-impulse-data-forwarder
   ```
4. Authenticate with your Edge Impulse account and assign sensor axes: `accX`, `accY`, `accZ` at **50 Hz** or **100 Hz**.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab14_Edge_Impulse_DataCollection.pdf` (Pages 1-5)
> 
> "In this lab you will collect motion sensor data using Edge Impulse... Install Edge Impulse CLI using npm... Run edge-impulse-data-forwarder to connect your Arduino Nano 33 BLE Sense and stream 3-axis accelerometer data into the Edge Impulse studio for labeling."
