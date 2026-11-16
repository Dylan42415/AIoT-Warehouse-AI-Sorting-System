# Theory: Computer Vision and Image Classification

This document synthesizes Chapter 8 (`Image Classification Introduction`, Prof. Marcelo Rovai, UNIFEI), detailing image representation, Convolutional Neural Networks (CNNs), and optimized edge vision architectures.

---

## 1. Digital Image Representation

A digital image is represented computationally as a discrete multidimensional array (tensor) of pixels:
- **Grayscale Images:** 2D tensor of shape $(H, W)$ where each pixel value is an 8-bit integer $\in [0, 255]$ representing light intensity ($0 = 	ext{Black}$, $255 = 	ext{White}$).
- **Color Images (RGB):** 3D tensor of shape $(H, W, C)$ with $C = 3$ color channels (Red, Green, Blue).

```
  Grayscale Pixel Grid:
  +-----+-----+-----+
  |  12 | 180 | 255 |   Shape: (Height, Width)
  |  45 | 210 | 128 |   Values: 0 to 255 (8-bit integer)
  +-----+-----+-----+
```

---

## 2. Convolutional Neural Networks (CNNs)

Traditional fully connected (dense) neural networks fail on image inputs because flattening high-resolution images destroys 2D spatial locality and results in an explosion of trainable parameters. **CNNs** preserve spatial hierarchy using local receptive fields and shared weights.

```
  Input Image (96x96)       Convolution Kernel (3x3)      Feature Map (94x94)
  +---+---+---+---+         +---+---+---+                +---+---+
  | 1 | 2 | 0 | 1 |         | 1 | 0 |-1 |   Slide &      | 3 | 1 |
  | 0 | 1 | 2 | 0 |    *    | 1 | 0 |-1 |  ==========>   | 2 | 4 |
  | 2 | 0 | 1 | 1 |         | 1 | 0 |-1 |   Dot Product  +---+---+
  +---+---+---+---+         +---+---+---+
```

### Core Operations in CNNs:
1. **2D Convolution:** A small filter/kernel (e.g., $3 	imes 3$) slides across the input tensor computing localized element-wise multiplications and sums to extract features (edges, textures, shapes):
   $$S(i, j) = (I * K)(i, j) = \sum_{m} \sum_{n} I(i - m, j - n) K(m, n)$$
2. **Padding and Stride:**
   - **Padding (Valid vs. Same):** Adding zero-borders to preserve spatial dimensions.
   - **Stride:** Step size of kernel movement (stride 2 downsamples spatial dimensions by half).
3. **Activation Function (ReLU):** Rectified Linear Unit introduces non-linearity by zeroing negative values:
   $$f(x) = \max(0, x)$$
4. **Pooling Layers (Max Pooling):** Downsamples spatial feature maps by taking the maximum value in a window (e.g., $2 	imes 2$ window with stride 2), providing spatial translation invariance and reducing memory footprint.
5. **Softmax Output Layer:** Normalizes final dense logits into a categorical probability distribution summing to 1.0:
   $$\sigma(z)_i = rac{e^{z_i}}{\sum_{j=1}^K e^{z_j}}$$

---

## 3. Efficient Edge Vision: MobileNet & Person Detection

Executing standard vision networks (like VGG-16 or ResNet) on microcontrollers is impossible due to memory limits. The **MobileNet** architecture replaces standard convolutions with **Depthwise Separable Convolutions**:
- **Depthwise Convolution:** Applies a single spatial filter per input channel.
- **Pointwise Convolution:** Applies a $1 	imes 1$ convolution to combine channel features.
- **Efficiency:** Reduces computational operations and model parameters by **$8	imes$ to $9	imes$** with minimal loss of accuracy.

### Person Detection Pipeline (Harvard TinyMLx):
- **Input Dimension:** $96 	imes 96 	imes 1$ (Grayscale).
- **Model Topology:** MobileNet backbone quantized to 8-bit integer weights ($pprox 250	ext{ KB}$ Flash).
- **Classification Output:** 2 output classes:
  - Class 0: `no-person`
  - Class 1: `person`
- Evaluated in [[Lab-09-Person-Detection|Lab 09 Person Detection]].

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 8_Image Classification Introduction.pdf` (Pages 1-50)
> 
> "25. Image Classification Introduction. Prof. Marcelo Rovai, UNIFEI... Computer vision models process pixel tensors. Convolutional Neural Networks (CNNs) use convolution filters, strides, and pooling layers to extract spatial features... MobileNet uses depthwise separable convolutions to drastically reduce parameter counts, enabling models like Person Detection (96x96 grayscale input) to execute within the memory boundaries of microcontrollers."
