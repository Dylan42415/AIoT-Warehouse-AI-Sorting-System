# Lab 10: TinyML Keyword Spotting

This document provides the exhaustive laboratory execution protocol for **Lab 10: TinyML Keyword Spotting** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Interface with the onboard [[MP34DT05-A-MEMS-Microphone|MP34DT05-A MEMS microphone]] via the `PDM` library.
2. Compile and flash the `micro_speech` sketch from the `Harvard_TinyMLx` library.
3. Understand the real-time audio feature extraction pipeline ([[Audio-Feature-Extraction-and-Spectrograms|Spectrogram / MFCC]]).
4. Detect spoken commands (`"yes"` and `"no"`) and apply temporal filtering to prevent false positives.

---

## 2. Sketch Architecture & Execution Protocol

1. Open `File` > `Examples` > `Harvard_TinyMLx` > `micro_speech`.
2. Verify board selection: `Arduino Nano 33 BLE`.
3. The sketch configures the PDM peripheral to sample mono audio at **16 kHz** into a double-buffered ring buffer.
4. The audio preprocessor slides a 30 ms window with 20 ms stride across the audio stream, calculating 2D Spectrogram slices.
5. The TFLM interpreter invokes the 2D CNN model, outputting probability scores for 4 classes:
   - `silence`
   - `unknown`
   - `yes`
   - `no`
6. When confidence for `"yes"` or `"no"` exceeds $0.80$ over multiple frames, the recognition event triggers an onboard LED toggle and serial log message.

Detailed in [[Keyword-Spotting-KWS-TinyML-Pipeline|Keyword Spotting KWS Pipeline]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab10_TinyML_Keyword_Spotting.pdf` (Pages 1-5)
> 
> "Keyword spotting detects words like 'yes' and 'no' using the onboard MP34DT05 microphone... Open micro_speech example from Harvard_TinyMLx... Audio is sampled at 16 kHz using PDM library and processed into spectrograms for CNN classification."
