# Theory: Sensors, Actuators, and Microcontrollers

This document captures the foundational hardware theory for the AIoT module (`AI53003FP`, Chapter 1), detailing the physics and electronic classification of sensors, actuators, and microcontroller architectures.

---

## 1. Electronics Fundamentals & Physical Quantities

Embedded IoT systems bridge the physical world and computational logic through electric circuits:
- **Voltage ($V$, Volts):** The potential difference between two points that drives electric charges.
- **Current ($I$, Amperes):** The rate of electrical charge flow through a conductor ($1	ext{ A} = 1	ext{ C/s}$).
- **Resistance ($R$, Ohms $\Omega$):** The opposition to current flow in a material.
- **Ohm's Law:** 
  $$V = I 	imes R \quad \iff \quad I = rac{V}{R} \quad \iff \quad R = rac{V}{I}$$
- **Electric Power ($P$, Watts):**
  $$P = V 	imes I = I^2 	imes R = rac{V^2}{R}$$

---

## 2. Sensor Classification & Transduction Principles

A **sensor** is an electronic transducer that converts a physical property (temperature, light, pressure, motion) into a measurable electrical signal (voltage, current, frequency, or digital words).

```
  Physical Phenomenon        Transducer Element         Electrical Output
  [ Temperature, Light, ] --> [ Resistive, Capacitive, ] --> [ Analog Voltage or ]
  [ Sound, Motion, etc. ]     [ Inductive, Optical     ]     [ Serial Digital Bus]
```

### Taxonomy of Sensors:
1. **Passive vs. Active Sensors:**
   - **Passive Sensors:** Do not require an external excitation power source to produce an electrical signal (e.g., thermocouples, piezoelectric crystals).
   - **Active Sensors:** Require an external operating voltage to bias sensing elements (e.g., [[HTS221-Humidity-Temperature-Sensor|HTS221]], [[APDS-9960-Digital-Sensor|APDS-9960]], strain gauges).
2. **Analog vs. Digital Sensors:**
   - **Analog Sensors:** Produce a continuous voltage or current signal proportional to the physical quantity (e.g., LDR photoresistors, thermistors). Must be digitized by an Analog-to-Digital Converter (ADC).
   - **Digital Sensors:** Contain integrated analog front-ends (AFE), ADCs, and digital logic that transmit data packets over serial communication buses ([[Protocol-UART-Serial-Communication|UART]], I2C, SPI) (e.g., [[LSM9DS1-9-Axis-IMU|LSM9DS1 IMU]], [[LPS22HB-Barometric-Pressure-Sensor|LPS22HB barometer]]).

---

## 3. Actuator Classification & Power Conversion

An **actuator** converts electrical signals into physical action, motion, force, or energy:
- **Electromechanical Actuators:** DC motors, stepper motors, and [[PWM-Servo-Motor-Control|servo motors]] that convert electrical energy into mechanical rotation or linear displacement.
- **Electromagnetic Actuators:** Solenoids and relays that convert electric currents into magnetic fields to switch contacts or open valves.
- **Optical & Acoustic Actuators:** LEDs (converting current to photons) and piezo buzzers/speakers (converting electrical oscillations to acoustic waves).

---

## 4. Microcontroller Unit (MCU) vs. Microprocessor (MPU)

| Feature | Microcontroller Unit (MCU) | Microprocessor Unit (MPU) |
| :--- | :--- | :--- |
| **Typical Target** | [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] (nRF52840) | [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi]] (BCM2711) |
| **Architecture** | Self-contained single chip with CPU, RAM, Flash, and Peripherals | CPU core requiring external RAM, Flash storage, and PMIC |
| **Clock Frequency** | Typically 16 MHz to 100 MHz (nRF52 is 64 MHz) | Typically 1.0 GHz to 2.5 GHz |
| **RAM Capacity** | Kilobytes (e.g., 256 KB) | Gigabytes (e.g., 2 GB – 8 GB LPDDR4) |
| **Operating System** | Bare-metal firmware or Real-Time OS (RTOS / Mbed OS) | Full Operating System (Linux, Android) |
| **Power Consumption** | Extremely low (milliwatts to microwatts) | Moderate (2W to 15W) |
| **Execution Determinism**| Highly deterministic real-time execution | Non-deterministic (preemptive OS scheduling) |

---

## 5. Microcontroller Peripheral Subsystems

1. **General Purpose Input/Output (GPIO):** Pins that can be configured in software as digital inputs (high impedance) or outputs (push-pull / open-drain).
2. **Analog-to-Digital Converter (ADC):** Quantizes continuous analog voltages into discrete $N$-bit digital integers:
   $$	ext{Digital Value} = 	ext{round}\left(rac{V_{	ext{in}}}{V_{	ext{ref}}} 	imes (2^N - 1)ight)$$
   For a 12-bit ADC with $V_{	ext{ref}} = 3.3	ext{V}$, an input of $1.65	ext{V}$ yields $pprox 2047$.
3. **Pulse Width Modulation (PWM):** Generates high-frequency square waves with variable duty cycle to simulate analog voltages and control actuators like [[PWM-Servo-Motor-Control|servos]] and LED brightness.
4. **Hardware Timers & Interrupts:** Hardware counters that trigger execution via [[Hardware-Interrupts-and-ISR-Programming|Interrupt Service Routines (ISRs)]] without polling.

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 1_Sensors, Actuators, and Microcontrollers.pdf` (Pages 5-64)
> 
> "A sensor is an electronic component that transforms a physical quantity, such as temperature, position, or light, into an electrical signal... An actuator is a component that converts electrical energy into physical motion or action... A microcontroller is a compact integrated circuit designed to govern a specific operation in an embedded system. A typical microcontroller includes a processor, memory, and input/output (I/O) peripherals on a single chip."
