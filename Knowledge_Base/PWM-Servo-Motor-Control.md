# PWM Servo Motor Control

The **PWM Servo Motor** (standard hobby actuator such as the **TowerPro SG90**) is an electromechanical actuator used in physical control loops, automated barriers, and cyber-physical systems. In the course curriculum, it is programmed in [[Lab-11-Servo-Motor-Control-Program|Lab 11 Servo Motor Control]] and integrated into [[Lab-12-Integrating-Proximity-Sensor-and-Servo-Motor-Control|Lab 12 Closed Loop Proximity & Servo Control]].

---

## 1. Electromechanical Operating Principles

A standard analog servo motor consists of four core elements:
1. **DC Motor:** Provides rotational mechanical power.
2. **Reduction Gearbox:** Steps down speed while dramatically multiplying torque.
3. **Potentiometer:** Mechanically coupled to the output shaft to provide closed-loop position feedback.
4. **Error Amplifier & Control Circuitry:** Compares incoming PWM pulse duration with potentiometer position to drive the motor to the matching target angle ($0^\circ$ to $180^\circ$).

---

## 2. Pulse Width Modulation (PWM) Timing Protocol

Standard hobby servos operate on a **50 Hz** PWM frequency (period $T = 20	ext{ ms}$):

```
  0° Position (1.0 ms)   +---+
                         |   |
                         +---+-----------------------+ (20 ms period)
                         
  90° Position (1.5 ms)  +-----+
                         |     |
                         +-----+---------------------+ (20 ms period)
                         
  180° Position (2.0 ms) +-------+
                         |       |
                         +-------+-------------------+ (20 ms period)
```

| Output Shaft Angle | PWM Pulse Width ($\mu	ext{s}$) | Duty Cycle at 50 Hz |
| :---: | :---: | :---: |
| **$0^\circ$** (Minimum) | $1000\ \mu	ext{s}$ ($1.0	ext{ ms}$) | $5.0\%$ |
| **$90^\circ$** (Center) | $1500\ \mu	ext{s}$ ($1.5	ext{ ms}$) | $7.5\%$ |
| **$180^\circ$** (Maximum) | $2000\ \mu	ext{s}$ ($2.0	ext{ ms}$) | $10.0\%$ |

---

## 3. Hardware Wiring & Electrical Isolation

The SG90 servo connector features a 3-wire color-coded cable:
- **Brown / Black Wire:** Common Ground (`GND`).
- **Red Wire:** Power Supply (`+5V` DC).
- **Orange / Yellow Wire:** PWM Control Signal (connected to digital pin `D9`).

> [!CAUTION] Power Separation Warning
> The [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] logic level is **3.3V** and its onboard MPM3610 regulator cannot supply the heavy stall current surges of an inductive motor (which can exceed 500 mA). 
> **Never power a servo motor directly from the `+3V3` pin.** Power the servo via an external 5V power supply or from the USB `VUSB` terminal (with shared common ground).

---

## 4. Software Implementation (`Servo.h`)

```cpp
#include <Servo.h>

Servo myServo;  // Instantiate servo object
const int servoPin = 9;

void setup() {
  myServo.attach(servoPin); // Attach PWM pin 9
  myServo.write(0);         // Initialize position to 0 degrees
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);
    delay(15); // Wait 15ms for servo to reach position
  }
  delay(1000);

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15);
  }
  delay(1000);
}
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab11_Servo_Motor_Control_Program.pdf` (Pages 1-6)
> 
> "A servo motor is a rotary actuator that allows for precise control of angular position. The position of a servo motor is set by the length of a pulse. The servo expects to see a pulse every 20 milliseconds (50Hz). The length of the pulse determines how far the motor turns. For example, a 1.5 millisecond pulse will turn the motor to the 90 degree position (often called the neutral position)."
>
> "Wiring: Brown = Ground, Red = 5V Power, Orange = Signal connected to Pin 9. Use Arduino Servo library: myServo.attach(9), myServo.write(angle)."
