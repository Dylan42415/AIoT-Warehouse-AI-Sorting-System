# Theory: Machine Learning Applications and Workflow

This document synthesizes Chapter 5 (`ML Applications Overview`, Prof. Marcelo Rovai, UNIFEI / IESTI01 TinyML), detailing the machine learning lifecycle, training paradigms, and mathematical evaluation metrics.

---

## 1. The Machine Learning Paradigm vs. Traditional Programming

```
  Traditional Programming:
  [ Data ]   +   [ Rules (Code) ]    =======>    [ Answers / Output ]

  Machine Learning:
  [ Data ]   +   [ Answers (Labels) ] =======>    [ Rules (Learned Model) ]
```

- **Traditional Programming:** Software engineers manually construct explicit algorithmic rules in code to transform inputs into outputs.
- **Machine Learning:** Algorithms automatically extract latent mathematical patterns, weights, and decision boundaries from input-output training pairs.

---

## 2. Paradigms of Machine Learning

1. **Supervised Learning:** The model trains on labeled datasets $(X, y)$:
   - **Classification:** Predicting discrete category labels (e.g., [[Person-Detection-TinyML-Pipeline|Person vs. No Person]], [[Keyword-Spotting-KWS-TinyML-Pipeline|Keyword: "Yes" vs. "No"]]).
   - **Regression:** Predicting continuous real-valued quantities (e.g., predicting temperature or remaining battery lifetime).
2. **Unsupervised Learning:** The model discovers intrinsic structures within unlabeled datasets $X$:
   - **Clustering:** Grouping similar data points (e.g., K-Means).
   - **Anomaly Detection:** Flagging samples that deviate significantly from baseline normal behavior (e.g., predictive vibration maintenance).
3. **Reinforcement Learning:** An agent learns optimal policy actions through trial, error, reward, and penalty signals within an interactive environment.

---

## 3. The End-to-End Machine Learning Lifecycle

```
  +--------------------+     +---------------------+     +-----------------------+
  | 1. Problem Framing | --> | 2. Data Acquisition | --> | 3. Data Preprocessing |
  +--------------------+     +---------------------+     +-----------+-----------+
                                                                     |
  +--------------------+     +---------------------+                 |
  | 6. Quantization &  | <-- | 5. Architecture &   | <---------------+
  |    Edge Deployment |     |    Model Training   |
  +---------+----------+     +---------------------+
            |
            v
  +--------------------+
  | 7. Model Inference |
  |    & Maintenance   |
  +--------------------+
```

1. **Problem Framing:** Formulate whether the task is classification, regression, or anomaly detection under embedded edge constraints.
2. **Data Acquisition:** Gathering representative sensor telemetry (accelerometer, audio, images) via tools like [[Lab-14-Edge-Impulse-Data-Collection|Edge Impulse]].
3. **Data Preprocessing & Feature Engineering:** Filtering noise, normalizing scale, computing [[Audio-Feature-Extraction-and-Spectrograms|Spectrograms / FFTs]].
4. **Dataset Splitting:** Partitioning data into Training (70%), Validation (15%), and Test (15%) subsets.
5. **Model Architecture & Training:** Optimizing weights via backpropagation using loss functions (Cross-Entropy, MSE) and optimizers (Adam).
6. **Quantization & Optimization:** Converting floating-point models to [[Model-Quantization-PTQ-Int8|int8 representations]].
7. **Edge Deployment:** Compiling models into C++ byte arrays running on [[Arduino-Nano-33-BLE-Sense|microcontrollers]].

---

## 4. Classification Evaluation Metrics & Confusion Matrix

For a binary classification task:

| | Predicted Positive | Predicted Negative |
| :---: | :---: | :---: |
| **Actual Positive** | **True Positive (TP)** | **False Negative (FN)** (Type II Error) |
| **Actual Negative** | **False Positive (FP)** (Type I Error) | **True Negative (TN)** |

### Core Mathematical Formulations:
1. **Accuracy:** Overall proportion of correct classifications:
   $$	ext{Accuracy} = rac{	ext{TP} + 	ext{TN}}{	ext{TP} + 	ext{TN} + 	ext{FP} + 	ext{FN}}$$
2. **Precision:** Ratio of correctly predicted positive observations to total predicted positives:
   $$	ext{Precision} = rac{	ext{TP}}{	ext{TP} + 	ext{FP}}$$
3. **Recall (Sensitivity):** Ratio of correctly predicted positive observations to all actual positives:
   $$	ext{Recall} = rac{	ext{TP}}{	ext{TP} + 	ext{FN}}$$
4. **F1-Score:** Harmonic mean of Precision and Recall:
   $$F_1 = 2 	imes rac{	ext{Precision} 	imes 	ext{Recall}}{	ext{Precision} + 	ext{Recall}} = rac{2	ext{TP}}{2	ext{TP} + 	ext{FP} + 	ext{FN}}$$

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 5_ML Applications Overview.pdf` (Pages 1-62)
> 
> "IESTI01 - TinyML: Embedded Machine Learning. 15. ML Applications Overview, AI Lifecycle and ML Workflow. Prof. Marcelo Rovai, UNIFEI... Machine learning turns traditional software programming on its head: instead of coding rules and feeding data to get answers, we feed data and answers to discover rules... Supervised learning maps inputs to outputs using labeled data for classification and regression tasks... Metrics include Confusion Matrix, Accuracy, Precision, Recall, and F1-Score."
