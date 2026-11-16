# Service Robots Classification and Theory

This document details the core theoretical definitions, operational taxonomy, task profiles, and functional architectures of **Service Robots** as established in SRA Lesson 1.

---

## What is a Service Robot?

A **Service Robot** is an automated system engineered to execute specific tasks, services, or functions to assist individuals, businesses, or organizations.

### The "4D + R" Task Domain
Service robots are primarily deployed to automate operations characterized as:
1. **Dirty:** Hazardous cleaning, sewage inspection, sanitation, biohazard handling.
2. **Dull:** Monotonous, repetitive material transfer, shelf scanning, tray returns.
3. **Distant:** Deep-sea inspection, planetary rovers, high-altitude maintenance.
4. **Dangerous:** Bomb disposal, disaster search-and-rescue, high-voltage line inspection.
5. **Repetitive:** Standardized assembly lines, food dispensing, routine hospitality greetings.

### Primary Purpose & Industrial Drivers
Service robots are deployed to improve **operational efficiency**, enhance **execution accuracy**, elevate **safety standards**, and deliver **convenience** across diverse sectors (healthcare, retail, logistics, education, hospitality).

---

## Operational Classifications

Service robotics is broadly bisected into two overarching industry categories:

```
                         +-----------------------------+
                         |       Service Robots        |
                         +--------------+--------------+
                                        |
         +------------------------------+------------------------------+
         |                                                             |
         v                                                             v
+-------------------------------+             +-------------------------------+
|   Personal / Domestic Robots  |             | Professional / Commercial     |
| Used by non-experts in homes  |             | Used by trained personnel or  |
| - Robotic vacuum cleaners     |             | organizations in enterprise   |
| - Lawn mowers                 |             | - Surgical & medical robots   |
| - Companion / Pet robots      |             | - Warehouse AGVs & AMRs       |
| - Educational humanoids       |             | - Delivery / Logistics robots |
|   (e.g., Alpha Mini)          |             | - Hospitality / Food runners  |
+-------------------------------+             +-------------------------------+
```

---

## General Architectural Subsystems of Service Robots

| Subsystem Component | Engineering Options & Technologies | SRA Course Context |
| :--- | :--- | :--- |
| **Manipulators** | Grippers, multi-axis arms, suction cups, or None (e.g. food delivery trays) | Differ in number of joints, degrees of freedom (DOF), and end-effector types |
| **Movement / Locomotion**| Bipedal/Quadruped legs, Tracks, Mecanum wheels, Omni-wheels, Drone rotors | Selection dictated by terrain topography and payload constraints |
| **Sensors & Perception** | LiDAR, Ultrasonic rangefinders, RGB Cameras, Microphone arrays, IMUs | Environmental mapping, obstacle avoidance, HRI tracking |
| **Connectivity** | GPS, Ethernet LAN, Wi-Fi 2.4/5GHz, 4G/5G Cellular, Bluetooth, Serial UART | Multi-tiered edge-to-cloud communication |
| **Autonomy Level** | Fully Autonomous, Semi-Autonomous, Fixed / Teleoperated | Governs dependency on human intervention |

---

## Autonomy Classifications

1. **Fully Autonomous:** Operates entirely independently in dynamic, unstructured environments without real-time human intervention. Generates internal maps, performs path planning, and recovers from unexpected obstacles.
2. **Semi-Autonomous:** Executes automated local behaviors (e.g., obstacle collision avoidance, trajectory following) but requires human supervisory guidance, target waypoint selection, or teleoperation during exceptions.
3. **Fixed / Teleoperated:** Operates from fixed anchor points or follows strict predetermined paths under continuous remote operator control.

---

## Inter-Node Connectivity
- Locomotion Systems: [[RoboticsLocomotionMechanisms]]
- Sensor Perception: [[RoboticsSensorsAndPerception]]
- Autonomous Navigation: [[AutonomousNavigationAndRoomMapping]]
- Alpha Mini Humanoid: [[AlphaMiniRobotOverview]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 2–6, `SRA Lesson 1- v1.0.pptx`:**
> *"What are Service Robots? Service robots are automated systems designed to perform specific tasks or services to assist individuals or organizations.*
> *These tasks are typically dirty, dull, distant, dangerous or repetitive.*
> *They are created to improve efficiency, accuracy, and convenience in various industries.*
> *General Features:*
> *- Manipulators: Differs in the number of joints, degrees-of-freedom and end-effector. Can be none, eg. food service robots.*
> *- Movement: Track, Mecanum Wheels, Omni-wheels, Legs, Rotors. Mobility needs depend on the operating environment: terrain topography, loading weight.*
> *- Sensors: LiDAR, Ultrasonic, Camera, Microphone-array, Gyroscope.*
> *- Connectivity: GPS, LAN, Wifi, 4G/5G, Bluetooth, Serial Interface.*
> *- Autonomy: Autonomous, Semi-autonomous, Fixed."*
