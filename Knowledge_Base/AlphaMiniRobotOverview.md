# Alpha Mini Robot Overview

The **Alpha Mini Robot** (developed by [[UBTECH-Robotics-Corp-Ltd|UBTECH Robotics Corp Ltd]]) is a compact, highly portable, intelligent humanoid service and educational robot designed for artificial intelligence education, social robotics research, and human-robot interaction (HRI). Measuring 245 mm in height and weighing approximately 700 g, the Alpha Mini packs an embedded Android-based computing system, 14 high-precision servo joints, advanced multi-modal sensory inputs (computer vision, 4-microphone far-field array, chest infrared time-of-flight sensor, dual TFT LCD emotive eyes, and capacitive head touch sensors), and comprehensive multi-network communication interfaces (Wi-Fi, 4G LTE, Bluetooth 4.2, and GPS).

---

## Architectural Role in Social Robotics & AIoT

In educational and enterprise scenarios, Alpha Mini bridges edge robotic embodiment with high-level cognitive computing:
- **Physical Embodiment:** Acts as an autonomous physical agent capable of expressive locomotion, human-like postures, gestures, and choreographed dances through its [[AlphaMiniMotionAndLocomotionAPI|14 servo joints]].
- **Emotive Social Interaction:** Displays dynamic facial expressions across two customized 1.22-inch LCD screens ([[AlphaMiniBuiltInEmoticonsCatalog|emoticon engine]]), speaks with natural text-to-speech synthesis, and pulses an RGB LED mouth indicator synchronously with speech output.
- **Perception & Edge Vision:** Employs an integrated 13-megapixel forehead camera for real-time face detection, gender/age estimation, recognized face lookup, and object recognition ([[AlphaMiniVisionAndSensingAPI|Edge Vision APIs]]).
- **Cloud & Edge AI Integration:** Interfaces directly with local Python runtimes via the [[AlphaMiniPythonSDKArchitecture|AlphaMini Python SDK]], or serves as an interactive voice endpoint communicating over RESTful HTTP/WebSocket channels to cloud conversational systems like [[NLPAndNeuralIntentsChatbot|NeuralIntents]] hosted on Google Colab and exposed via [[FlaskAndNgrokCloudIntegration|Flask / Ngrok]].

---

## High-Level Capabilities Matrix

| Functional Subsystem | Hardware / Engine | Core Capabilities | Knowledge Link |
| :--- | :--- | :--- | :--- |
| **Locomotion & Posture** | 14 Digital Servos | Omnidirectional stepping, squatting, lunges, kung-fu, dances, fall detection | [[AlphaMiniHardwareSpecifications]], [[AlphaMiniMotionAndLocomotionAPI]] |
| **Facial & Affective Display** | Dual 1.22" TFT LCDs + RGB Mouth LED | 240×240 pixel emotive eye animations, multi-color pulsing mouth lamp | [[AlphaMiniExpressionsAndMouthLampAPI]], [[AlphaMiniBuiltInEmoticonsCatalog]] |
| **Acoustic & Voice Interaction** | 4-Mic Array + 2×0.8W Stereo Speakers | Far-field voice localization, automated speech recognition (ASR), TTS playback | [[AlphaMiniTextToSpeechAndSoundAPI]], [[AlphaMiniSpeechRecognitionObserver]] |
| **Sensory Perception** | IR ToF Sensor, Head Capacitive Touch, IMU | Distance ranging up to obstacle boundary, tap/double-tap head interaction, fall/climb pose detection | [[AlphaMiniSensoryAndPostureObservers]], [[AlphaMiniVisionObservers]] |
| **Connectivity & Networking** | Wi-Fi 802.11 b/g/n, 4G LTE Nano-SIM, BT4.2 | Local subnet mDNS discovery, mobile app pairing, standalone cellular cloud communication | [[AlphaMiniWiFiAndNetworkConfiguration]], [[AlphaMiniConnectionLifecycle]] |
| **Developer Ecosystem** | Python 3.7 SDK, uCode Visual Programming | Asynchronous coroutine APIs (`asyncio`), wheel package deployment, remote execution | [[AlphaMiniDevelopmentEnvironmentSetup]], [[PythonAsyncioFundamentals]] |

---

## Course Context & Module Integration

Within the **Social Robotics Applications (SRA)** curriculum, Alpha Mini serves as the core physical robotic testbed across six foundational lessons and lab exercises:
1. [[ServiceRobotsClassificationAndTheory|Lesson 1: Introduction to Service Robots, Autonomy, and Kinematics]]
2. [[AlphaMiniDevelopmentEnvironmentSetup|Lesson 2 / Lab 1: Development Environment Setup and Connection Verification]]
3. [[PythonAsyncioFundamentals|Lesson 3 / Lab 2: Introduction to Python Asyncio and Asynchronous Concurrency]]
4. [[AlphaMiniPythonSDKArchitecture|Lesson 4 / Lab 3: Controlling Alpha Mini with Python and the SDK APIs]]
5. [[NLPAndNeuralIntentsChatbot|Lesson 5 / Lab 4: NLP Chatbot Architecture, REST APIs, and Flask on Google Colab]]
6. [[AlphaMiniCloudChatbotIntegrationPipeline|Lesson 6 / Lab 5 & 6: Alpha Mini Front-End Integration with Colab Cloud Chatbot]]

---

## Inter-Node Connectivity
- Parent Topic: [[INDEX-SRA-Social-Robotics-Knowledge-Graph]]
- Hardware Details: [[AlphaMiniHardwareSpecifications]], [[AlphaMiniAnatomyAndStructure]]
- Safety & Maintenance: [[AlphaMiniSafetyPrecautionsAndMaintenance]]
- Environment Setup: [[AlphaMiniDevelopmentEnvironmentSetup]]
- Connection Guide: [[AlphaMiniConnectionLifecycle]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 01 (`Labsheet 01 - Setup AlphaMini Development Environment.docx`):**
> *"What is Alpha Mini robot?*
> *Alpha Mini is a small programmable humanoid robot with great potential despite its small size. Alpha Mini is highly portable, bringing fun interactions and a wealth of expressions and functionality, including voice interaction, 4G LTE connectivity, face recognition, illustrated book recognition, and object recognition.*
> *The Alpha Mini robot was created by UBTECH Robotics Corp Ltd for AI Robotics Education.*
> *What can it do?*
> *Alpha Mini robot was created for the study of human-robot relationships, as well as developing artificial intelligence. It comes with a host of sensors and servos to interact with the environment.*
> *Its facial recognition system thus allows him to track the user's fixation point while interacting with him. Its articulative joints allow it to pose, move and dance. It has a built-in microphone array and speaker that allows it to strike up a conversation with a human, using speech recognition, text-to-speech, and a programmable dialog engine."*
