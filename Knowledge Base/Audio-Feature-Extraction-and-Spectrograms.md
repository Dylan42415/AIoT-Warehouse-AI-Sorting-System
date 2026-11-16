# Audio Feature Extraction and Spectrograms

This document details the signal processing pipeline that converts continuous time-domain acoustic waveforms captured by the [[MP34DT05-A-MEMS-Microphone|MP34DT05-A MEMS microphone]] into 2D time-frequency images for [[Keyword-Spotting-KWS-TinyML-Pipeline|Keyword Spotting]].

---

## 1. Time-Domain to Frequency-Domain Transformation

1. **Audio Framing:** A 1-second 16 kHz audio buffer contains 16,000 discrete samples. This buffer is partitioned into overlapping windows of **30 ms** duration ($480	ext{ samples}$) with a **20 ms** step size ($320	ext{ samples}$).
2. **Hamming Window:** To minimize spectral leakage caused by finite signal truncation:
   $$w(n) = 0.54 - 0.46 \cos\left(rac{2\pi n}{N - 1}ight)$$
3. **Fast Fourier Transform (FFT):** Converts the windowed samples into frequency spectrum magnitude coefficients:
   $$X[k] = \sum_{n=0}^{N-1} x[n] e^{-j 2\pi k n / N}$$

---

## 2. Mel Filterbanks & MFCC Generation

1. **Mel Filterbank:** Triangular bandpass filters spaced along the logarithmic Mel scale are applied to the FFT power spectrum:
   $$m = 2595 \log_{10}\left(1 + rac{f}{700}ight)$$
2. **Log Energy Calculation:** Taking the logarithm of the energy in each Mel band compresses the dynamic range, matching human auditory response.
3. **Discrete Cosine Transform (DCT):** Decorrelates the filterbank energies, producing **Mel-Frequency Cepstral Coefficients (MFCCs)**.
4. **Spectrogram Matrix:** Stacking sequentially computed MFCC vectors over time produces a $40 	imes 40$ or $49 	imes 40$ 2D matrix fed directly into the convolutional neural network.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 9_KeyWord Spotting (KWS) Introduction.pdf` (Pages 15-45)
> 
> "Audio feature extraction transforms raw audio waveforms into 2D spectrogram representations... Windowing with a Hamming window prevents spectral leakage, followed by FFT and Mel filterbank integration to extract MFCC features."
