# Theory: Audio Processing and Keyword Spotting

This document synthesizes Chapter 9 (`KeyWord Spotting (KWS) Introduction`, Prof. Marcelo Rovai, UNIFEI), detailing acoustic signal physics, digital audio feature extraction, and convolutional speech recognition pipelines.

---

## 1. Acoustic Signals & Digital Audio Sampling

Sound is an analog acoustic pressure wave oscillating through a medium. Microphones (such as the [[MP34DT05-A-MEMS-Microphone|MP34DT05-A]] MEMS sensor) convert acoustic pressure into electrical signals:
- **Sampling Rate ($f_s$):** Number of discrete audio samples captured per second.
- **Nyquist-Shannon Sampling Theorem:** To prevent aliasing, the sampling frequency must be at least twice the highest audio frequency component:
  $$f_s \ge 2 	imes f_{	ext{max}}$$
  Standard human speech frequencies range up to $pprox 8	ext{ kHz}$, making **$16	ext{ kHz}$** the ideal sampling rate for TinyML speech recognition.
- **Bit Depth:** Number of bits per sample (16-bit signed PCM represents amplitudes from $-32,768$ to $+32,767$).

---

## 2. Audio Feature Extraction: Time Domain to Spectrogram

Raw 1D time-domain audio waveforms contain complex phase variations that are difficult for standard neural networks to classify directly. Feature engineering transforms audio into 2D time-frequency images:

```
  Raw 1D Audio Waveform (Time Domain)
  ~~/\~~\_/\~~/\~~~
          |
          v  Windowing (20-30 ms slices with 50% overlap)
  [ Fast Fourier Transform (FFT) ]
          |
          v  Computes frequency spectrum per time slice
  [ Spectrogram (2D Time vs Frequency Heatmap) ]
          |
          v  Apply Mel Filterbank (Logarithmic frequency binning)
  [ Mel Spectrogram / MFCC (Mel-Frequency Cepstral Coefficients) ]
```

1. **Framing & Windowing:** The continuous 16 kHz stream is divided into short overlapping frames (e.g., 30 ms duration with 20 ms stride). A Hamming or Hanning window is applied to eliminate boundary discontinuities.
2. **Fast Fourier Transform (FFT):** Converts each time-domain frame into a frequency-domain spectrum showing energy distribution across frequency bins.
3. **Spectrogram:** Stacking the FFT frequency spectra of sequential time windows horizontally creates a 2D matrix (time along the X-axis, frequency along the Y-axis, energy amplitude as pixel intensity).
4. **Mel Scale & Mel Filterbanks:** Human hearing perceives sound pitch logarithmically rather than linearly. The Mel scale warps linear frequencies:
   $$m = 2595 \log_{10}\left(1 + rac{f}{700}
ight)$$
5. **MFCC (Mel-Frequency Cepstral Coefficients):** Applying a Discrete Cosine Transform (DCT) to log-mel filterbank energies yields a compact 2D feature map that acts like a visual fingerprint of spoken phonemes.

---

## 3. Keyword Spotting (KWS) Neural Network Architecture

Once audio is converted into a 2D Spectrogram or MFCC tensor, speech recognition is solved as a **2D computer vision image classification problem**:

```
  Audio Input (1 second = 16,000 samples @ 16 kHz)
       |
       v
  [ Spectrogram Feature Generator (e.g., 40 time steps x 40 frequency bins) ]
       |
       v
  [ 2D Convolutional Neural Network (CNN) ]
       |
       v
  [ Softmax Output: "yes", "no", "unknown", "silence" ]
```

### Temporal Smoothing & Post-Processing:
Raw neural network predictions on continuous audio streams can fluctuate due to background noise. A temporal moving-average filter processes consecutive output probabilities:
- A keyword is only registered if its smoothed confidence exceeds a preset threshold (e.g., $> 0.80$) consistently for multiple successive frames.
- Implemented and evaluated in [[Lab-10-TinyML-Keyword-Spotting|Lab 10 Keyword Spotting]].

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 9_KeyWord Spotting (KWS) Introduction.pdf` (Pages 1-69)
> 
> "23. KeyWord Spotting (KWS) Introduction. Prof. Marcelo Rovai, UNIFEI... Speech recognition begins with digital sampling at 16 kHz... Feature extraction converts raw 1D time-domain audio waveforms into 2D Spectrograms and Mel-Frequency Cepstral Coefficients (MFCC) via FFT and filterbanks... A 2D CNN processes the spectrogram as an image to classify spoken keywords like 'yes' and 'no', applying thresholding to prevent false positives."
