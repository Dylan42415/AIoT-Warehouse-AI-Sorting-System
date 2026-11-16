# Convolutional Neural Networks for Edge Vision

**Convolutional Neural Networks (CNNs)** are the foundational deep learning architecture for computer vision tasks, providing spatial invariance and feature extraction capabilities within edge memory constraints.

---

## 1. 2D Convolution Operation

A 2D convolution applies a learnable kernel matrix $K \in \mathbb{R}^{k_h 	imes k_w}$ across an input feature map $X \in \mathbb{R}^{H 	imes W}$:

$$Y(i, j) = (X * K)(i, j) = \sum_{m=-k_h/2}^{k_h/2} \sum_{n=-k_w/2}^{k_w/2} X(i - m, j - n) \cdot K(m, n) + b$$

```
  Input Tensor Window        Convolution Kernel (3x3)      Output Activation
  +-----+-----+-----+        +-----+-----+-----+          +------------------+
  | 100 | 120 | 110 |        |  1  |  0  | -1  |          |                  |
  |  90 | 105 |  95 |   *    |  2  |  0  | -2  |  =====>  |  Resulting Sum   |
  |  80 |  90 |  85 |        |  1  |  0  | -1  |          |  (e.g., Edge)    |
  +-----+-----+-----+        +-----+-----+-----+          +------------------+
```

---

## 2. Standard Convolution vs. Depthwise Separable Convolution

Standard convolutions are computationally expensive. For an input of size $H 	imes W 	imes D_{	ext{in}}$ producing $D_{	ext{out}}$ feature maps using a $D_k 	imes D_k$ kernel:
$$	ext{Cost}_{	ext{Standard}} = H 	imes W 	imes D_k^2 	imes D_{	ext{in}} 	imes D_{	ext{out}}$$

In **MobileNet**, this is factored into two steps:
1. **Depthwise Convolution:** Applies a single spatial filter per input channel ($D_k 	imes D_k 	imes 1$).
2. **Pointwise Convolution:** Applies a $1 	imes 1$ convolution across all channels ($1 	imes 1 	imes D_{	ext{in}} 	imes D_{	ext{out}}$).

$$	ext{Cost}_{	ext{Depthwise Separable}} = H 	imes W 	imes D_k^2 	imes D_{	ext{in}} + H 	imes W 	imes D_{	ext{in}} 	imes D_{	ext{out}}$$

$$	ext{Computational Reduction Ratio} = rac{D_k^2 	imes D_{	ext{in}} + D_{	ext{in}} 	imes D_{	ext{out}}}{D_k^2 	imes D_{	ext{in}} 	imes D_{	ext{out}}} = rac{1}{D_{	ext{out}}} + rac{1}{D_k^2}$$

For a standard $3 	imes 3$ kernel ($D_k = 3$), this achieves an **$8	imes$ to $9	imes$ reduction** in computational load, making real-time vision possible on the [[Arduino-Nano-33-BLE-Sense|nRF52840]].

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 8_Image Classification Introduction.pdf` (Pages 12-40)
> 
> "Standard convolutions require high computational power. MobileNet architectures factor convolutions into depthwise and pointwise operations, drastically reducing operations by approximately 8 to 9 times for 3x3 filters, enabling real-time computer vision on embedded hardware."
