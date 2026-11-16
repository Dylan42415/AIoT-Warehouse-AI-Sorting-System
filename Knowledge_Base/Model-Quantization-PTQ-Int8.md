# Model Quantization (PTQ Int8)

**Post-Training Quantization (PTQ)** is the process of converting a pre-trained deep learning model's floating-point parameters ($	ext{float32}$) into low-bitwidth integer representations ($	ext{int8}$) without requiring full model retraining.

---

## 1. Mathematical Mapping Formulation

Quantization maps a continuous real floating-point value $r$ to an 8-bit signed integer $q \in [-128, 127]$:

$$r = S 	imes (q - Z)$$
$$q = 	ext{round}\left(rac{r}{S}ight) + Z$$

Where:
- $r$: Real-world continuous floating-point number.
- $q$: Quantized 8-bit signed integer ($[-128, 127]$).
- $S$: **Scale factor** (positive real float representing the resolution step size):
  $$S = rac{r_{	ext{max}} - r_{	ext{min}}}{q_{	ext{max}} - q_{	ext{min}}} = rac{r_{	ext{max}} - r_{	ext{min}}}{255}$$
- $Z$: **Zero-point offset** (integer value corresponding to real-world $0.0$):
  $$Z = 	ext{round}\left(-rac{r_{	ext{min}}}{S}ight) + q_{	ext{min}}$$

---

## 2. Symmetric vs. Asymmetric Quantization

```
  Asymmetric Quantization:
  Real Float Range: [ r_min ----------------- 0.0 -------------- r_max ]
  Quantized Range:  [ -128 ------------------- Z ---------------- +127 ]
  (Zero-point Z is offset from 0)

  Symmetric Quantization:
  Real Float Range: [ -r_max ---------------- 0.0 ------------- +r_max ]
  Quantized Range:  [ -127 ------------------- 0 ---------------- +127 ]
  (Zero-point Z = 0; simplifies multiply-accumulate arithmetic)
```

1. **Asymmetric Quantization:** Uses both Scale $S$ and non-zero Zero-point $Z$. Essential for activations following ReLU where all values are non-negative ($\ge 0$).
2. **Symmetric Quantization:** Forces $Z = 0$, aligning real $0.0$ directly with integer $0$. Widely used for convolutional neural network weights to accelerate matrix multiplication on Arm Cortex-M4 DSP instructions.

---

## 3. Representative Dataset Calibration

Because post-training quantization must determine the dynamic ranges $[r_{	ext{min}}, r_{	ext{max}}]$ of intermediate activation layers, the developer feeds a small representative dataset ($pprox 100	ext{ to }500$ unlabelled samples) through the TensorFlow Lite Converter:

```python
import tensorflow as tf

def representative_data_gen():
    for input_value in tf.data.Dataset.from_tensor_slices(train_images).batch(1).take(100):
        yield [tf.dtypes.cast(input_value, tf.float32)]

converter = tf.lite.TFLiteConverter.from_saved_model(saved_model_dir)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_data_gen
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

quantized_tflite_model = converter.convert()
```

---

## 4. Hardware Acceleration Benefits

1. **Memory Reduction:** $4	imes$ memory reduction ($1	ext{ byte}$ per parameter vs. $4	ext{ bytes}$ for float32).
2. **DSP Instruction Acceleration:** Arm Cortex-M4 SIMD instructions (e.g., `SMLAD` - Signed Multiply Accumulate Dual) process multiple 8-bit and 16-bit integers in a single clock cycle.
3. **Power Savings:** Reduces energy-intensive Flash read bus accesses.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 6_Introduction to TensorFlow Lite and TFL-Micro.pdf` (Pages 18-28)
> 
> "Quantization transforms 32-bit floating point weights and activations to 8-bit integers. The mapping formula uses scale (S) and zero-point (Z): r = S * (q - Z). Post-training quantization uses a representative dataset to calibrate activation ranges, delivering a 4x reduction in model size and significant inference speedup on embedded processors."
