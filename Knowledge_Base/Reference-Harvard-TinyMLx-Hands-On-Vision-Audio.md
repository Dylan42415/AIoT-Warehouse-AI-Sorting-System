# Reference: Harvard TinyMLx Hands-On Vision and Audio

This document provides a technical synthesis of the Harvard University TinyMLx curriculum reference slides (**"Hands on Embedded ML (Vision and Audio)"** by Prof. Brian Plancher, Harvard John A. Paulson School of Engineering and Applied Sciences, Source: `03_Lab Sheets/Reference_HandsOnVisionAudio.pdf` / `09_Additional Resources/Reference_HandsOnVisionAudio.pdf`).

---

## 1. Embedded Vision Architecture & OV7675 Sensor Mechanics

```
  +--------------------------------------------------------------+
  |              OV7675 Camera Sensor Architecture               |
  +--------------------------------------------------------------+
  |  - Active Array: 640 x 480 pixels (VGA)                      |
  |  - Control Protocol: SCCB (Serial Camera Control Bus, I2C)   |
  |  - Video Bus: 8-bit Parallel Digital Video Port (DVP)        |
  |  - Timing Lines: PCLK (Pixel Clock), HREF (Horizontal Ref),  |
  |                 VSYNC (Vertical Sync), XCLK (System Clock)   |
  +--------------------------------------------------------------+
```

### Camera Operation Sequence:
1. **Clock Generation:** The nRF52840 MCU drives a high-frequency system clock (`XCLK` at $\approx 16\text{ MHz}$) into the camera module.
2. **Configuration Registers:** Configuration bytes are written over the SCCB bus to set exposure, automatic gain control (AGC), auto white balance (AWB), and output format (YUV422).
3. **Synchronous Frame Reading:**
   - The camera asserts `VSYNC` high during frame blanking.
   - For each active scanline, `HREF` is asserted high.
   - For every pixel byte on data pins `D[7:0]`, the camera pulses `PCLK`. The microcontroller samples the byte on the rising edge of `PCLK`.
4. **Subsampling to $96 \times 96$ Grayscale:** To operate within the 256 KB SRAM constraint of the nRF52840, the driver reads only the luminance ($Y$) bytes, discarding chrominance ($U/V$), and skips pixels to produce a normalized $96 \times 96$ byte array ($9,216\text{ bytes}$).

---

## 2. Audio Processing Architecture (Micro Speech)

```
  1. Microphone Capture:
     [ MP34DT05 MEMS ] ===(1-bit PDM @ 1.024 MHz)===> [ nRF52 Decimation Filter ]
                                                                 |
                                                                 v
                                                      [ 16 kHz 16-bit PCM Audio ]
                                                                 |
  2. Audio Ring Buffer:                                          v
     Stores sliding 1000 ms audio window (16,000 samples) <------+
                                                                 |
  3. Feature Generator:                                          v
     - Slices window into 30 ms frames with 20 ms stride
     - Applies Hamming Window
     - Executes Fast Fourier Transform (FFT)
     - Applies Mel Filterbank to generate 2D Spectrogram
                                                                 |
  4. Neural Network Inference:                                   v
     - 2D CNN evaluates 4 classes: [silence, unknown, yes, no]
     - Moving average thresholding filters transient spikes
```

---

## 3. Microcontroller Memory & Latency Profile

| Pipeline Stage | Flash ROM Footprint | SRAM Utilization | Latency on 64 MHz Cortex-M4F |
| :--- | :---: | :---: | :---: |
| **Person Detection (MobileNet)**| $\approx 250\text{ KB}$ | $\approx 73\text{ KB}$ (`TensorArena`) | $\approx 200\text{ ms} - 400\text{ ms}$ |
| **Micro Speech (KWS CNN)** | $\approx 40\text{ KB}$ | $\approx 15\text{ KB}$ (`TensorArena`) | $\approx 50\text{ ms} - 100\text{ ms}$ |
| **Raw Frame Buffer ($96 \times 96$)**| $0\text{ KB}$ | $9.2\text{ KB}$ | Captured during line read |
| **Audio Ring Buffer (1 second)** | $0\text{ KB}$ | $32\text{ KB}$ (16,000 $\times$ 2 bytes) | Background DMA |

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Reference_HandsOnVisionAudio.pdf` (Pages 1-181) & `09_Additional Resources/Reference_HandsOnVisionAudio.pdf`
> 
> "Brian Plancher, Harvard John A. Paulson School of Engineering and Applied Sciences. Hands on Embedded ML (Vision and Audio)... Detailed examination of OV7675 camera SCCB and DVP timing signals, downsampling VGA frames to 96x96 grayscale for MobileNet person detection... Audio pipeline uses MP34DT05 PDM microphone decimated to 16 kHz PCM, transformed into spectrograms via FFT for keyword spotting... Memory arena sizing and cycle latency analysis on Arm Cortex-M4F."
