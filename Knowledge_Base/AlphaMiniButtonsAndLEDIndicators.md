# Alpha Mini Buttons and LED Indicators

The **Alpha Mini Robot** provides intuitive physical interface controls and multi-color visual feedback mechanisms. This document provides the complete mapping of buttons, gestures, and LED illumination patterns.

---

## Physical Controls & Capacitive Gestures

### 1. Power Button Operations

| Action | Execution Method | Operational Outcome |
| :--- | :--- | :--- |
| **Power On** | Press and hold power button on backpack | Mouth LED illuminates white; robot boots up. Takes ~40 seconds until robot announces *"I am ready"*. |
| **Power Off** | Press and hold power button for 3 seconds | Robot terminates software processes, releases servos, and shuts down completely. (Alternative: voice command *"Hey Mini, shut down"*). |
| **Query Battery Level** | Short single press while powered on or charging | Robot reports current battery percentage via speech and/or eye LCD icons. |
| **Quit Programming Mode**| Short single press while in Python SDK control | Forces Alpha Mini to terminate `enter_program()` execution and return to default operating mode. |

### 2. Volume Button Operations

- **Press Volume Up / Down:** Increments or decrements robot speaker volume level. (Also controllable programmatically via [[AlphaMiniTextToSpeechAndSoundAPI|ChangeRobotVolume(volume=0.5)]]).

### 3. Head Capacitive Touch Sensor Gestures

| Gesture | Target Location | Triggered Event / Behavior |
| :--- | :--- | :--- |
| **Single Tap** | Crown / Forehead surface | **Interrupts current action / speech immediately.** Stops active locomotion, dance, or TTS playback. |
| **Double Tap** | Crown / Forehead surface | **Checks Network Status.** Robot renders Wi-Fi SSID and assigned IP address across both LCD eyes, and announces connectivity state. |

---

## Tricolor Mouth LED Status Matrix

The LED indicator inside Alpha Mini's mouth communicates system power, network health, charging states, and runtime modes:

| Mouth LED Visual Pattern | Operational State | Detailed Technical Meaning |
| :--- | :--- | :--- |
| **White Light Flickering** | System Active / Speaking | Robot is booted up and running normally. Flickers dynamically when TTS audio is playing to simulate speech. |
| **Constant Red Light** | Privacy Mode Active | Microphone array and 13MP camera are disabled by default for privacy protection. |
| **Flash Red 3 Times (Periodic)**| Offline / Network Disconnected | Robot has lost Wi-Fi or cellular connection; cannot reach cloud backends. |
| **Flickering Red Light** | Charging (Battery < 50%) | Robot is powered off and actively charging via Type-C adapter; charge level is below 50%. |
| **Flickering Green Light** | Charging (50% ≤ Battery < 100%)| Robot is powered off and actively charging; charge level has reached 50% but is not yet full. |
| **Constant Green Light** | Fully Charged (Battery = 100%) | Robot is powered off, connected to charger, and battery has reached 100% capacity. |

---

## Programmatic Mouth LED Control via Python SDK

Developers can override mouth LED behavior during runtime using the [[AlphaMiniExpressionsAndMouthLampAPI|api_expression]] module:
- `ControlMouthLamp(is_open=True/False)`: Turns the mouth lamp on or off.
- `SetMouthLamp(color=MouthLampColor, mode=MouthLampMode, duration=int, breath_duration=int)`:
  - **Colors:** `MouthLampColor.RED` (1), `MouthLampColor.GREEN` (2), `MouthLampColor.BLUE` (3).
  - **Modes:** `MouthLampMode.NORMAL` (0, solid duration), `MouthLampMode.BREATH` (1, pulsing breath duration).

---

## Inter-Node Connectivity
- Hardware Specifications: [[AlphaMiniHardwareSpecifications]]
- Physical Anatomy: [[AlphaMiniAnatomyAndStructure]]
- SDK Expression Control: [[AlphaMiniExpressionsAndMouthLampAPI]]
- Reactive Touch Handling: [[AlphaMiniVisionObservers]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 7, `Alphamini_User_Manual.pptx`:**
> *"07 Buttons and Lights - User Manual*
> *Power button:*
> *- Press and hold: Press and hold for 3 seconds to turn your AlphaMini Robot on or off.*
> *- Press: Press the power button to check the battery level after turning on your robot or during the charging process. Press the power button to quit programming mode.*
> *Volume button: Increase and decrease the volume of your AlphaMini Robot.*
> *Touch sensor on the head:*
> *- Tap your AlphaMini Robot on the forehead to stop current action.*
> *- Double tap the robot's forehead to check the network.*
> *LED Light in mouth:*
> *- While light is flickering: Your robot is turned on.*
> *- Constant red light: Your robot is in privacy mode and his microphone and camera have been disabled by default.*
> *- Flash red light 3 times every once in a while: Your Robot is offline.*
> *- Flickering red light: Your robot is turned off and charging (robot battery <50%).*
> *- Flickering green light: Your robot is turned off and charging (50%<=robot battery<100%).*
> *- Constant green light: Your robot is off and fully charged."*
