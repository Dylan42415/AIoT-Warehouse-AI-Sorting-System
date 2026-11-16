# Reference: Arduino Programming Notebook

This document provides an exhaustive reference of the **Arduino Programming Notebook (v1.1)** by Brian W. Evans, documenting the syntax, control structures, data types, and core standard functions of the Arduino C++ programming environment.

---

## 1. Program Structure

Every Arduino sketch must define two primary functions:
- **`void setup()`:** Executes once when program starts. Used to initialize pin modes (e.g. `pinMode(13, OUTPUT)`), begin serial communication (`Serial.begin(9600)`), and instantiate libraries.
- **`void loop()`:** Executes consecutively in an infinite loop, reading inputs, calculating logic, and driving actuator outputs.

---

## 2. Control Structures

### 1. Conditional Branching (`if`, `if...else`):
```cpp
if (x > 100) {
  digitalWrite(13, HIGH);
} else if (x > 50) {
  digitalWrite(13, LOW);
} else {
  digitalWrite(13, LOW);
}
```

### 2. For Loop:
```cpp
for (int i = 0; i < 255; i++) {
  analogWrite(PWM_PIN, i);
  delay(10);
}
```

### 3. While & Do-While Loops:
```cpp
while (sensorValue < 500) {
  sensorValue = analogRead(A0);
}

do {
  sensorValue = analogRead(A0);
} while (sensorValue < 500);
```

---

## 3. Data Types & Variables

| Data Type | Bit Width | Memory Range | Usage Notes |
| :--- | :---: | :---: | :--- |
| `boolean` | 8-bit | `true` (1) or `false` (0) | Logical conditions |
| `char` | 8-bit | -128 to 127 | ASCII character literal (e.g., `'A'`) |
| `byte` | 8-bit | 0 to 255 | Unsigned 8-bit integer |
| `int` | 16-bit (AVR) / 32-bit (nRF52) | -32,768 to 32,767 (AVR) | Primary integer type |
| `unsigned int`| 16-bit / 32-bit | 0 to 65,535 (AVR) | Positive counting integers |
| `long` | 32-bit | -2,147,483,648 to 2,147,483,647 | Large integer quantities |
| `unsigned long`| 32-bit | 0 to 4,294,967,295 | Essential for timer variables (`millis()`) |
| `float` | 32-bit | $\approx \pm 3.4028235 \times 10^{38}$ | Single precision floating point (6-7 decimals) |
| `double` | 32-bit (AVR) / 64-bit (Arm) | Same as float on AVR; 64-bit on Arm | Double precision floating point |
| `array` | Variable | Indexed collection | `int mySensors[6] = {2, 4, -8, 3, 2};` |

---

## 4. Digital & Analog I/O Functions

### Digital I/O:
- `pinMode(pin, mode)`: Configures pin as `INPUT`, `OUTPUT`, or `INPUT_PULLUP`.
- `digitalRead(pin)`: Returns `HIGH` (1) or `LOW` (0).
- `digitalWrite(pin, value)`: Drives pin to 3.3V/5V (`HIGH`) or 0V (`LOW`).

### Analog I/O:
- `analogRead(pin)`: Reads voltage from an analog input channel. On the [[Arduino-Nano-33-BLE-Sense|Nano 33 BLE Sense]], returns a 10-bit (`0` to `1023`) or configurable 12-bit (`0` to `4095`) integer.
- `analogWrite(pin, value)`: Generates hardware PWM square wave on capable pins (values `0` to `255`).

---

## 5. Time & Concurrency

- `millis()`: Returns the number of milliseconds elapsed since the board began running the current sketch as an `unsigned long`. Overflows/rolls over after $\approx 50\text{ days}$. Essential for non-blocking timers.
- `micros()`: Returns microseconds elapsed (rolls over after $\approx 70\text{ minutes}$).
- `delay(ms)`: Pauses program execution for specified milliseconds (blocking).
- `delayMicroseconds(us)`: Pauses execution for specified microseconds.

---

## 6. Mathematical & Random Functions

- `min(x, y)` & `max(x, y)`: Return the minimum or maximum of two values.
- `abs(x)`: Returns the absolute value of $x$.
- `constrain(x, a, b)`: Constrains $x$ within bounds $[a, b]$.
- `map(value, fromLow, fromHigh, toLow, toHigh)`: Re-maps a number from one range to another.
- `pow(base, exponent)` & `sqrt(x)`: Exponential and square root calculations.
- `randomSeed(seed)`: Seeds pseudo-random number generator (commonly initialized with `randomSeed(analogRead(0))`).
- `random(max)` or `random(min, max)`: Generates pseudo-random integers.

---

## 7. Serial Communication (`Serial`)

- `Serial.begin(speed)`: Sets baud rate (e.g., `9600`, `115200`).
- `Serial.print(val)` / `Serial.println(val)`: Prints human-readable ASCII data.
- `Serial.available()`: Returns number of incoming bytes waiting in the serial buffer.
- `Serial.read()`: Reads the first incoming byte from buffer (returns `-1` if empty).
- `Serial.write(buf, len)`: Transmits raw binary bytes.

---

## Traceability & Source Excerpts

> **Source:** `09_Additional Resources/11. arduino_notebook_v1-1 - Copy.pdf` (Pages 1-40)
> 
> "Arduino Programming Notebook by Brian W. Evans... Serves as a convenient, easy to use programming reference for the command structure and basic syntax of the Arduino microcontroller... Covers Structure (setup, loop), Control Structures (if, for, while), Syntax, Operators, Variables, Digital I/O, Analog I/O, Time, Math, and Serial communication."
