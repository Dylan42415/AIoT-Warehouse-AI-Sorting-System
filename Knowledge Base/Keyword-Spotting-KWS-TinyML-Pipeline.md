# Keyword Spotting (KWS) TinyML Pipeline

The **Keyword Spotting (KWS) TinyML Pipeline** is an embedded voice command recognition system running on the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]], classifying acoustic words such as `"yes"` and `"no"`.

---

## 1. End-to-End Speech Recognition Flow

```
  [ MP34DT05 MEMS Microphone ]
        |
        v 1-bit PDM stream @ 1.024 MHz
  [ PDM Decimation Filter ]
        |
        v 16-bit PCM Audio @ 16 kHz (Ring Buffer)
  [ Audio Feature Provider (FFT / Spectrogram) ]
        |
        v Generates 2D MFCC Spectrogram Tensor
  [ 2D CNN Speech Classifier (TFLM) ]
        |
        v Outputs 4 Logits: [silence, unknown, "yes", "no"]
  [ Temporal Moving-Average Filter ]
        |
        v Suppresses transient false positives
  [ Recognized Command Action ]
        - Toggles LED state / Transmits command via UART to SBC
```

---

## 2. Recognized Vocabulary & Output Logits

The pre-trained model evaluates 4 distinct categorical output classes:
1. `silence`: Ambient background acoustic floor without human vocalization.
2. `unknown`: Spoken words other than target vocabulary (e.g., "dog", "hello", "cat").
3. `yes`: Target activation keyword 1.
4. `no`: Target activation keyword 2.

---

## 3. Post-Processing & Temporal Filtering

Raw inference scores from single audio frames can suffer from spurious noise spikes. The post-processor tracks scores across a sliding time window (typically 3 to 5 consecutive inferences):
- A keyword is only confirmed if its average probability exceeds **0.80** ($80\%$) and remains the dominant prediction.
- Detailed in [[Lab-10-TinyML-Keyword-Spotting|Lab 10 Keyword Spotting]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab10_TinyML_Keyword_Spotting.pdf` (Pages 1-5) & `Chapter 9_KeyWord Spotting (KWS) Introduction.pdf`
> 
> "TinyML Keyword Spotting detects specific speech keywords like 'yes' and 'no' using the onboard digital microphone... Audio is digitized at 16 kHz, transformed into a spectrogram, and classified by a CNN into silence, unknown, yes, or no... Temporal smoothing prevents false triggers."
