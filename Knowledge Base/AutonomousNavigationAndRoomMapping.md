# Autonomous Navigation and Room Mapping

Autonomous mobility allows service robots to operate without human intervention. This document outlines the engineering architecture of **Autonomous Navigation** and the 5-step iterative **Room Mapping Pipeline** defined in SRA Lesson 1.

---

## What is Autonomous Navigation?

**Autonomous Navigation** is the capability of a robotic system to determine its own position, construct or reference an environmental map, plan collision-free paths, and control actuators to reach destination coordinates without human guidance.

---

## The 5-Step Room Mapping Pipeline

The construction of an environmental floor plan is an iterative process combining sensory range-finding, state estimation, and spatial optimization:

```
[ Step 1: Exploration ]
  Robot traverses physical room, sweeping environment with LiDAR, cameras, and sonars.
         |
         v
[ Step 2: Data Collection ]
  Sensory range measurements, odometry readings, and optical images are logged to memory.
         |
         v
[ Step 3: Localization (SLAM) ]
  Estimates robot pose (x, y, θ) relative to previously observed landmarks / point clouds.
         |
         v
[ Step 4: Mapping ]
  Integrates localized sensor scans into a unified grid map (occupancy grid / 3D mesh).
         |
         v
[ Step 5: Navigation & Path Planning ]
  Uses finalized map to plan collision-free trajectories (A*, Dijkstra) and guide actuators.
```

---

## Detailed Step Breakdown

### Step 1: Exploration
- The robot navigates unexplored spatial zones.
- Continuously steers sensors to detect geometric boundaries: perimeter walls, furniture legs, doorways, and temporary obstacles.
- Exploration algorithms prioritize frontier boundaries (regions between known open space and unknown territory).

### Step 2: Data Collection
- High-frequency data logging from multi-modal sensor payloads:
  - LiDAR range arrays ($r_i, \theta_i$)
  - Ultrasonic time-of-flight echos
  - Wheel encoder odometry ticks
  - IMU angular acceleration vectors

### Step 3: Localization (SLAM)
- Solves the chicken-or-egg paradox of robotics: *To build a map, the robot must know its position; but to know its position, the robot needs a map.*
- Solved through **Simultaneous Localization and Mapping (SLAM)** algorithms (e.g. Gmapping, Cartographer, Fast-SLAM).
- Compares incoming sensor scans against previous frames to estimate precise coordinates $(x, y, \theta)$ on the fly.

### Step 4: Mapping
- Translates accumulated spatial telemetry into persistent spatial data structures:
  - **Occupancy Grid Maps:** 2D cell grids where each cell holds a probability between 0.0 (free space) and 1.0 (occupied obstacle).
  - **Point Cloud / Octomap:** 3D volumetric voxel models.

### Step 5: Navigation & Path Planning
- Once an environment is mapped, the robot performs autonomous transit:
  - **Global Path Planning:** Computes optimal route from start to target coordinate using graph search algorithms ($A^*$, Dijkstra).
  - **Local Collision Avoidance:** Dynamically circumvents unexpected moving obstacles (people, pets) using reactive sensor fields.

---

## Inter-Node Connectivity
- Theory Overview: [[ServiceRobotsClassificationAndTheory]]
- Sensors & Perception: [[RoboticsSensorsAndPerception]]
- Locomotion Mechanics: [[RoboticsLocomotionMechanisms]]
- Alpha Mini Navigation Context: [[AlphaMiniRobotOverview]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 22–24, `SRA Lesson 1- v1.0.pptx`:**
> *"Features - Autonomy: Autonomous robots can work independently without the help of a human operator. Autonomous navigation enables the robot to move about without the need for human input or control. Relies on various sensors to learn about the operating environment.*
> *Room Mapping: Autonomous robots can map a room using a variety of techniques and sensors. The most common sensors used for mapping are cameras, LIDAR, and ultrasonic sensors. The process is usually iterative to improve the accuracy.*
> *The process of mapping a room usually involves several steps:*
> *Exploration: The robot moves around the room, scanning its surroundings using its sensors to detect objects, walls, and obstacles.*
> *Data collection: The robot collects data from its sensors, such as images or range data from LIDAR, and stores it in a memory or a computer.*
> *Localization: The robot determines its position in the room by comparing the collected data with a previously stored map or using techniques such as SLAM (Simultaneous Localization and Mapping).*
> *Mapping: The robot builds a map of the room using the collected data and its estimated position.*
> *Navigation: Once the map is created, the robot can navigate within the room using the map and its sensors to avoid obstacles and reach its destination."*
