# MP34DT05-A MEMS Microphone

The **STMicroelectronics MP34DT05-A** (or updated `MP34DT06JTR`) is an ultra-compact, low-power, omnidirectional digital MEMS microphone with a capacitive sensing element and an integrated digital interface. On the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]], it captures environmental audio for [[Keyword-Spotting-KWS-TinyML-Pipeline|Keyword Spotting (KWS)]] and acoustic edge classification.

---

## 1. Technical Specifications

| Parameter | Specification |
| :--- | :--- |
| **Transducer Type** | Capacitive MEMS acoustic sensor |
| **Directivity** | Omnidirectional |
| **Output Interface** | Pulse Density Modulation (PDM) |
| **Signal-to-Noise Ratio (SNR)** | 64 dB |
| **Sensitivity** | -26 dBFS ±3 dB |
| **Acoustic Overload Point (AOP)**| 122.5 dBSPL |
| **Clock Frequency Range** | 1.2 MHz to 3.25 MHz |
| **Target Audio Sample Rate** | 16 kHz (Standard speech recognition rate) |
| **Audio Bit Depth** | 16-bit Pulse Code Modulation (PCM) after decimation |
| **Dedicated nRF52 Pins** | Clock: `P0.26`, Data: `P0.25`, Power: `P0.17` |

---

## 2. PDM to PCM Decimation Pipeline

```
  Acoustic Wave
       |
       v
  [ Capacitive MEMS ] ---> [ 1-bit PDM Stream @ 1.024 MHz ]
                                       |
                                       v  Decimation & Digital Filter (Cortex-M4F)
                           [ 16-bit PCM Audio @ 16 kHz ]
                                       |
                                       v
                           [ FFT / Spectrogram Engine ]
```

1. **PDM Stream Generation:** The microphone samples sound pressure waves into an oversampled high-frequency 1-bit PDM digital stream.
2. **Decimation Filtering:** The nRF52840 hardware PDM peripheral filters and decimates the 1-bit stream down to standard 16 kHz, 16-bit signed PCM integer audio samples.
3. **Double Buffering:** Audio frames are captured into ping-pong DMA buffers to prevent audio dropouts during neural network execution.

---

## 3. Arduino Software API (`PDM.h`)

The official `PDM` library manages the hardware interface:

```cpp
#include <PDM.h>

short sampleBuffer[256];
volatile int samplesRead;

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2; // 16-bit samples
}

void setup() {
  Serial.begin(9600);
  PDM.onReceive(onPDMdata);
  // Configure for 1 channel (mono) at 16 kHz
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM microphone!");
    while (1);
  }
}
```

---

## 4. Associated Labs & Edge Audio Workflows

- **[[Lab-10-TinyML-Keyword-Spotting|Lab 10 Keyword Spotting]]:** Capturing speech commands ("yes", "no") and classifying them in real time.
- **[[Audio-Feature-Extraction-and-Spectrograms|Audio Feature Extraction]]:** Converting PCM samples into Spectrograms and MFCCs.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/Arduino_Nao_datasheet.pdf` (Page 4, Section 2.5) & `Pinout-NANOsense_latest.pdf`
> 
> "The MP34DT05-A is an ultra-compact, low-power, omnidirectional, digital MEMS microphone built with a capacitive sensing element and an IC interface. It has an acoustic overload point of 122.5 dBSPL with a 64 dB signal-to-noise ratio. Connected to nRF52 pins P0.26 (CLK), P0.25 (DOUT), and P0.17 (VDD)."
