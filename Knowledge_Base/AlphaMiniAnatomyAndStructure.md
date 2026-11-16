# Alpha Mini Anatomy and Structure

The **Alpha Mini Robot** features an engineered humanoid anatomy with integrated sensors, visual displays, manipulators, audio transducers, and mechanical actuation joints.

---

## Structural Diagram & Component Locations

```
                     +---------------------------+
                     |    Touch Sensor (Top)     |
                     |  [4-Microphone Array]     |
                     +-------------+-------------+
                                   |
                     +-------------+-------------+
                     |       13MP Camera         |
                     |  [Dual 1.22" LCD Eyes]    |
                     |        LED Mouth          |
                     +-------------+-------------+
                                   |
         +-------------------------+-------------------------+
         |                         |                         |
+--------+--------+      +---------+---------+      +--------+--------+
|   Right Arm     |      |  Chest IR Sensor  |      |    Left Arm     |
| (2 Servo DOF)   |      |  Belly Microphone |      |  (2 Servo DOF)  |
|                 |      | 2x Stereo Speaker |      | [Nano-SIM Slot] |
+--------+--------+      +---------+---------+      +--------+--------+
         |                         |                         |
         |               +---------+---------+               |
         |               |   1 Waist Servo   |               |
         |               +---------+---------+               |
         |                         |                         |
+--------+--------+      +---------+---------+      +--------+--------+
| Back Chassis:   |      |   Left / Right    |      | Type-C Port:    |
| - Sling Hole    |      |   Leg Assemblies  |      | Base of         |
| - Handy Grips   |      |    (6 DOF Total)  |      | Backpack        |
| - Power/Vol Btn |      +-------------------+      +-----------------+
+-----------------+
```

---

## Detailed Component Anatomy

### 1. Head Module
- **Capacitive Touch Sensor:** Positioned on the crown/forehead area of the skull. Detects touch interactions (single tap, double tap).
- **4-Microphone Beamforming Array:** Symmetrically placed around the upper skull (two front aperture holes, two rear aperture holes). Used for 360-degree acoustic direction of arrival (DoA) detection and far-field speech recognition.
- **13 MP HD Camera:** Embedded centrally on the forehead above the eyes. Provides high-resolution image acquisition for [[AlphaMiniVisionAndSensingAPI|face detection, face analysis, and object recognition]].
- **Dual LCD Eyes:** 1.22-inch 240×240 pixel TFT displays showing animated graphic pupils and expressions ([[AlphaMiniBuiltInEmoticonsCatalog|emoticon engine]]). Also displays Wi-Fi connection state and IP addresses upon request.
- **Tricolor LED Mouth:** Translucent mouth bar with internal RGB LED used for speech visual synchronization and system health indications ([[AlphaMiniButtonsAndLEDIndicators|mouth LED status matrix]]).
- **Head Articulation:** 3 dedicated digital servos controlling pan (yaw), tilt (pitch), and nodding motions.

### 2. Torso & Chest Module
- **Chest Infrared (IR) Distance Sensor:** Concentric circular emitter and detector mounted on the central chest emblem. Measures range to nearest forward obstacles in millimeters ([[AlphaMiniSensoryAndPostureObservers|Infrared Distance Observer]]).
- **Communication Microphone:** Single dedicated close-range mic port positioned below the IR emblem on the lower belly.
- **Stereo Speakers:** Two lateral 0.8 W speakers integrated into the rib cage flanks for high-fidelity audio output and speech synthesis.
- **Nano-SIM Card Slot:** Located under the **left armpit**. Accessed by inserting a SIM ejector tool into the pinhole while the robot is powered down. Supports 4G cellular SIM cards.
- **Waist Joint:** 1 digital servo providing axial torso rotation.

### 3. Backpack & Control Module
- **Sling Hole:** Structural aperture at the upper rim of the backpack for tethering or lanyard attachment.
- **Handy Grips:** Ergonomic indentations on the upper rear backpack designed as the primary grip surface for lifting and handling the robot.
- **Volume Button:** Rocker / dual button on the side of the backpack for manual volume increment/decrement.
- **Power Button:** Illuminated tactile button located on the lower side of the backpack. Long press (3s) powers on/off; short press queries battery status or exits programming mode.
- **USB Type-C Interface:** Located on the lower bottom base plate of the backpack. Used for battery charging and factory data diagnostics.

### 4. Limbs & Locomotion Assemblies
- **Arms (Upper Manipulators):** 2 servos per arm (4 total), allowing flexion, extension, waving, hugs, and expressive gesturing.
- **Legs (Lower Manipulators):** 3 servos per leg (6 total) driving the hip, knee, and ankle joints. Enables forward, backward, and lateral stepping, lunges, and balancing.
- **Internal 6-Axis IMU:** Internal gyroscopic and accelerometric sensors inside the chassis for dynamic stability and fall detection.

---

## Inter-Node Connectivity
- Hardware Parameters: [[AlphaMiniHardwareSpecifications]]
- Indicators & Controls: [[AlphaMiniButtonsAndLEDIndicators]]
- Handling Protocols: [[AlphaMiniSafetyPrecautionsAndMaintenance]]
- Locomotion Mechanics: [[AlphaMiniMotionAndLocomotionAPI]], [[RoboticsLocomotionMechanisms]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 6 & 7, `Alphamini_User_Manual.pptx`:**
> *"06 Structure - User Manual*
> *Front/Side: Camera, LCD eyes, LED mouth, Infrared sensor, Microphone, SIM slot, Stereo speakers*
> *Back/Side: Sling hole, Handy grip, Volume button, Power button, Type-C interface*
> *Top View: Touch sensor, Microphone array (4 holes around head)"*
