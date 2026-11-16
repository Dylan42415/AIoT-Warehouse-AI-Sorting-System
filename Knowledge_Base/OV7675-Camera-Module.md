# OV7675 Camera Module

The **OmniVision OV7675** is a low-power, high-sensitivity 1/9-inch CMOS VGA (640x480) camera module designed for embedded vision and [[Theory-Computer-Vision-and-Image-Classification|edge computer vision]] applications. In the TinyML Kit, it mounts into header `J8` of the [[Machine-Learning-Carrier-V1.0|Machine Learning Carrier V1.0]] to provide real-time image capture for the [[Person-Detection-TinyML-Pipeline|Person Detection TinyML Pipeline]] and [[Lab-15-Edge-Impulse-Image-Classification-Protocol|Edge Impulse Image Classification]].

---

## 1. Technical Specifications & Image Formats

| Parameter | Specification |
| :--- | :--- |
| **Optical Size** | 1/9 inch |
| **Active Array Resolution** | 640 x 480 pixels (VGA) |
| **Maximum Image Transfer Rate** | 30 frames per second (fps) at VGA |
| **Output Image Formats** | YUV422 / YCbCr422, RGB565 / RGB555 / RGB444, Raw RGB, Grayscale (Y) |
| **Control Bus Interface** | Serial Camera Control Bus (SCCB) — I2C compatible |
| **Video Data Interface** | 8-bit parallel Digital Video Port (DVP) |
| **Core Operating Voltage** | 1.8V DC (internally regulated) |
| **I/O Logic Levels** | 3.3V DC |
| **Pixel Size** | 1.75 µm x 1.75 µm |
| **Lens Focal Adjustment** | Manual focus ring with locking ring |

---

## 2. Interface Timing & Signal Protocol

The OV7675 communicates with the [[Arduino-Nano-33-BLE-Sense|nRF52840]] via the following synchronization lines:

```
  VSYNC (Frame Sync)   ____/-------------------------------\____
  HREF  (Line Sync)    ________/-----\___/-----\___/-----\______
  PCLK  (Pixel Clock)  --|_|--|_|--|_|--|_|--|_|--|_|--|_|--|_|-
  D[7:0](Data Bus)     ====[ Byte 0 ]===[ Byte 1 ]===[ Byte 2 ]=
```

1. **`VSYNC` (Vertical Sync):** High during vertical frame blanking; transitions low when active video frame transmission starts.
2. **`HREF` / `HSYNC` (Horizontal Reference):** High when active pixel data for a horizontal line is being driven onto the 8-bit bus.
3. **`PCLK` (Pixel Clock):** Driven by the camera. The microcontroller samples the 8-bit data bus `D[7:0]` on the rising edge of `PCLK`.
4. **`XCLK` (System Master Clock):** Microcontroller PWM signal (typically 12 MHz to 24 MHz) driven into the camera module to drive its internal image sensor pipeline.

---

## 3. TinyML Image Downsampling Pipeline (96x96 Grayscale)

Due to the strict 256 KB SRAM constraint on the nRF52840, processing full VGA RGB565 frames ($640 	imes 480 	imes 2 = 614,400	ext{ bytes}$) is impossible. The acquisition pipeline uses:

1. **Subsampling / Cropping:** The camera window is configured to a central square crop and downsampled to **96 x 96 pixels**.
2. **Grayscale Extraction ($Y$-channel):** Instead of color processing, only the luminance ($Y$) channel of the YUV422 stream is extracted.
3. **Memory Footprint:**
   $$	ext{Buffer Size} = 96 	imes 96 	imes 1	ext{ byte} = 9,216	ext{ bytes} pprox 9.2	ext{ KB}$$
   This comfortably fits inside the static memory allocation of [[TensorFlow-Lite-for-Microcontrollers-TFLM|TFLM's TensorArena]].

---

## 4. Hardware Lens Focusing Protocol

Out of the box, the OV7675 lens focal distance may be preset to infinity, resulting in blurry close-up images:
1. Loosen the outer knurled locking ring.
2. Run the camera preview / person detection sketch.
3. Slowly rotate the inner threaded lens barrel while holding a test subject 30–50 cm away until sharp contrast is achieved.
4. Tighten the locking ring to secure optical alignment.

---

## Traceability & Source Excerpts

> **Source:** `04_Hardware_Documents/TinyML_Introduction.pdf` (Pages 6-8) & `Reference_HandsOnVisionAudio.pdf` (Pages 20-35)
> 
> "The OV7675 camera is an OmniVision CMOS VGA image sensor module. It mounts to the TinyML carrier socket J8. It provides image frames to the microcontroller over an 8-bit parallel video port with VSYNC, HREF, and PCLK synchronization signals."
> 
> "In TinyML person detection, images are captured at 96x96 resolution in 8-bit grayscale format, requiring 9,216 bytes of memory per frame. The lens focus ring must be adjusted manually to achieve optimal sharp focus for near-field objects."
