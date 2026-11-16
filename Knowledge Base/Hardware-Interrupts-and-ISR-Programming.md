# Hardware Interrupts and ISR Programming

**Hardware Interrupts** are physical hardware signals driven into the microcontroller's CPU (Nested Vectored Interrupt Controller / NVIC) that temporarily halt the main program execution loop to execute a high-priority subprogram called an **Interrupt Service Routine (ISR)**.

---

## 1. Polling vs. Hardware Interrupts

| Characteristic | Polling (Synchronous) | Hardware Interrupts (Asynchronous) |
| :--- | :--- | :--- |
| **Mechanism** | CPU repeatedly checks pin state in a loop | Hardware detector fires only on physical state change |
| **CPU Utilization** | High (CPU spins waiting for input) | Low (CPU can sleep or perform other tasks) |
| **Response Latency** | Non-deterministic (delayed by `delay()` calls)| Deterministic (immediate jump in nanoseconds) |
| **Power Efficiency** | Wasteful | Highly power efficient |

---

## 2. Critical Rules for Writing Interrupt Service Routines (ISRs)

Because an ISR preempts main program flow, violating embedded design constraints causes deadlocks or memory corruption:

1. **Keep it Short and Fast:** Do not execute extensive mathematical algorithms inside an ISR; set a flag and handle processing in `loop()`.
2. **Never Call `delay()`:** `delay()` relies on timer hardware interrupts which are blocked inside an ISR. Calling `delay()` causes an infinite CPU lockup.
3. **Avoid `Serial.print()`:** Hardware serial depends on interrupt-driven TX buffers. Printing within an ISR can trigger deadlocks.
4. **Use the `volatile` Keyword:** Any variable modified inside an ISR and read by the main loop **must** be declared with `volatile` to prevent the compiler from optimizing variable reads into CPU registers:
   ```cpp
   volatile uint btnCount = 0;
   volatile bool buttonFlag = false;
   ```

---

## 3. Switch Contact Bounce & Software Debouncing

Mechanical pushbuttons (like `PB1` on the [[Machine-Learning-Carrier-V1.0|Carrier]]) consist of spring metal contacts. When pressed, the contacts physically chatter/bounce for 5 to 50 milliseconds before settling, generating dozens of false interrupt triggers:

```
  Clean Digital Signal:   HIGH ___________|LOW|____________
  
  Physical Switch Bounce: HIGH ______|_|||_|||||||________ (Chatter: 10-20 ms)
```

### Millis-Based Non-Blocking Debounce Routine:
```cpp
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // 200 ms lockout

void isr_button_pressed() {
  unsigned long currentTime = millis();
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    btnCount++;
    lastDebounceTime = currentTime;
  }
}
```

Detailed in [[Code-Arduino-Button-Interrupt-Debounce|Code Button Interrupt]] and [[Lab-13-Button-Interrupt|Lab 13 Button Interrupt]].

---

## 4. Arduino Interrupt API (`attachInterrupt`)

```cpp
attachInterrupt(digitalPinToInterrupt(pin), ISR_Function, MODE);
```

### Trigger Modes:
- `LOW`: Triggers continuously while the pin is `LOW`.
- `CHANGE`: Triggers on both rising and falling electrical transitions.
- `RISING`: Triggers when signal transitions from `LOW` to `HIGH`.
- `FALLING`: Triggers when signal transitions from `HIGH` to `LOW` (ideal for active-low buttons with pull-ups).

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab13_Button_Interrupt.pdf` (Pages 1-5) & `03_Lab Sheets/Code Example for students/AIoT_Arduino/Button_Interrupt/Button_Interrupt/Button_Interrupt.ino`
> 
> "Interrupts are useful for making things happen automatically in microcontroller programs and can help solve timing problems. A task that needs to happen when a button is pressed is a good candidate for an interrupt... attachInterrupt(digitalPinToInterrupt(buttonPin), isr, FALLING)... Variables shared between an ISR and normal code must be declared volatile... Debouncing is required to prevent multiple triggers from mechanical switch contact bounce."
