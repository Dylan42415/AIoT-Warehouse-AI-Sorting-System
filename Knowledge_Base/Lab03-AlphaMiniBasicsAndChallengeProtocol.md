# Lab 03: Alpha Mini Basics and Challenge Protocol

This document provides the exhaustive lab manual and execution protocol for **Labsheet 03: Alpha Mini Basics in Python**, detailing the sound, expression, motion APIs and the comprehensive 60-minute multi-step **Lab 3 Challenge**.

---

## Lab Objectives

In this lab, students learn how to:
1. Fix relative import bugs in the official demo codebase.
2. Control speech synthesis and sound effects using `test_sound.py`.
3. Animate facial LCD expressions and control the RGB mouth LED using `test_expression.py`.
4. Command physical bipedal steps and limb gestures using `test_action.py`.
5. Solve the 60-minute multi-step integrated programming challenge.

---

## Core Module Walkthroughs

### 1. Sound & Text-to-Speech (`test_sound.py`)
- **Fix Import Bug:** Change `from test.test_connect import ...` to `from test_connect import ...`.
- **Speech Synthesis:** `StartPlayTTS(text="...")` synthesizes voice and animates mouth LED.
- **Non-blocking Stop:** `StartPlayTTS(is_serial=False, text="...")` combined with `StopPlayTTS()`.
- **Audio Presets:** `PlayAudio(url="horn", storage_type=AudioStorageType.PRESET_LOCAL)` plays built-in sound effects.
- **Volume Adjustment:** `ChangeRobotVolume(volume=0.5)` sets speaker volume to 50%.

### 2. Facial Displays, LEDs & Dances (`test_expression.py`)
- **Emoticon Rendering:** `PlayExpression(express_name="codemao1")` renders animated eyes.
- **Mouth Lamp Control:**
  - `SetMouthLamp(color=MouthLampColor.GREEN, mode=MouthLampMode.NORMAL, duration=3000, breath_duration=1000)` sets a solid green light for 3 seconds.
  - `ControlMouthLamp(is_open=False)` turns off the mouth lamp.
- **Dance Choreography:** `StartBehavior(name="dance_0004")` commands robot to dance to 'Little Star'.

### 3. Motion & Locomotion (`test_action.py`)
- **Discrete Actions:** `PlayAction(action_name="018")` raises the right leg.
- **Bipedal Stepping:** `MoveRobot(step=10, direction=MoveRobotDirection.LEFTWARD)` takes 10 steps sideways.
- **Action Catalog:** `GetActionList(action_type=RobotActionType.INNER)` retrieves built-in actions.

---

## The 60-Minute Lab 3 Challenge

### Challenge Requirements Matrix:
1. **Idle State Lighting:** Whenever Alpha Mini is idle (waiting for user input), the Mouth LED must be **pulsing GREEN at a 3-second breathing interval** (`mode=BREATH`, `breath_duration=3000`).
2. **Speech State Lighting:** Whenever Alpha Mini is speaking, the Mouth LED must illuminate **WHITE**.
3. **Interactive Step Gates:** Each step must be isolated into a discrete function and gated by a Python `input("Press Enter to continue...")` prompt.
4. **File Naming Convention:** Submit script as `Lab03-challenge-[Student ID].py` (e.g., `Lab03-challenge-0034.py`).

### Complete 10-Step Execution Table:

| Step | Functional Description | Category / Type | API Implementation |
| :---: | :--- | :--- | :--- |
| **0** | Connect to Alpha Mini & Enter Programming Mode | SETUP | `test_connect.py` (`MiniSdk.connect()`, `MiniSdk.enter_program()`) |
| **1** | Greet user using student's name | TTS + EXPR | `StartPlayTTS(text="Hello [Name]")` + `PlayExpression(express_name="codemao1")` |
| **2** | Press ENTER key to continue | KEYBOARD GATE | `input("Press Enter to continue...")` |
| **3** | Show a smiling face | EXPR | `PlayExpression(express_name="emo_007")` |
| **4** | Press ENTER key to continue | KEYBOARD GATE | `input("Press Enter to continue...")` |
| **5** | Sneeze routine | ACTION / DANCE | `StartBehavior(name="w_stand_0008")` or `PlayAction` |
| **6** | Press ENTER key to continue | KEYBOARD GATE | `input("Press Enter to continue...")` |
| **7** | Hug and say "I love you" simultaneously | ACT + TTS + EXPR | `asyncio.gather(PlayAction(action_name="random_short2").execute(), StartPlayTTS(text="I love you").execute())` |
| **8** | Press ENTER key to continue | KEYBOARD GATE | `input("Press Enter to continue...")` |
| **9** | Exit program & release resources | TEARDOWN | `MiniSdk.quit_program()`, `MiniSdk.release()` |

---

## Complete Challenge Reference Implementation (`Lab03-challenge-0034.py`)

```python
import asyncio
import logging
import mini.mini_sdk as MiniSdk
from mini.dns.dns_browser import WiFiDevice
from mini.apis.api_sound import StartPlayTTS
from mini.apis.api_expression import PlayExpression, SetMouthLamp, MouthLampColor, MouthLampMode
from mini.apis.api_action import PlayAction
from mini.apis.api_behavior import StartBehavior
from test_connect import test_connect, shutdown, test_get_device_by_name

STUDENT_NAME = "Dylan"
STUDENT_ID = "0034"

async def set_idle_mouth_lamp():
    """Pulsing green mouth LED at 3-second intervals during idle/waiting states."""
    block = SetMouthLamp(
        color=MouthLampColor.GREEN,
        mode=MouthLampMode.BREATH,
        duration=-1,
        breath_duration=3000
    )
    await block.execute()

async def step1_greeting():
    print("--- Step 1: Greeting ---")
    # Mouth LED automatically pulses white during speech
    tts = StartPlayTTS(text=f"Hello {STUDENT_NAME}, it is great to see you!")
    expr = PlayExpression(express_name="codemao1")
    await asyncio.gather(tts.execute(), expr.execute())

async def step3_smile():
    print("--- Step 3: Smiling Face ---")
    block = PlayExpression(express_name="emo_007")  # Smile emoticon
    await block.execute()

async def step5_sneeze():
    print("--- Step 5: Sneeze ---")
    block = StartBehavior(name="w_stand_0008")  # Built-in Sneeze behavior
    await block.execute()

async def step7_hug_and_love():
    print("--- Step 7: Hug and Say 'I love you' (Simultaneous) ---")
    act_task = PlayAction(action_name="random_short2").execute()  # Hug action
    tts_task = StartPlayTTS(text="I love you!").execute()
    expr_task = PlayExpression(express_name="codemao19").execute() # Love emoticon
    await asyncio.gather(act_task, tts_task, expr_task)

async def main():
    MiniSdk.set_log_level(logging.INFO)
    MiniSdk.set_robot_type(MiniSdk.RobotType.DEDU)
    
    # Step 0: Setup & Connect
    device: WiFiDevice = await test_get_device_by_name()
    if not device:
        print("Device not found!")
        return

    connected = await test_connect(device)
    if not connected:
        print("Connection failed!")
        return

    await MiniSdk.enter_program()
    await asyncio.sleep(6)

    try:
        # Step 1: Greet
        await step1_greeting()
        
        # Step 2: Gate
        await set_idle_mouth_lamp()
        input("Press Enter to continue to Step 3...")

        # Step 3: Smile
        await step3_smile()

        # Step 4: Gate
        await set_idle_mouth_lamp()
        input("Press Enter to continue to Step 5...")

        # Step 5: Sneeze
        await step5_sneeze()

        # Step 6: Gate
        await set_idle_mouth_lamp()
        input("Press Enter to continue to Step 7...")

        # Step 7: Hug and say "I love you"
        await step7_hug_and_love()

        # Step 8: Gate
        await set_idle_mouth_lamp()
        input("Press Enter to exit program...")

    finally:
        # Step 9: Teardown
        print("--- Step 9: Shutting Down ---")
        await shutdown()

if __name__ == '__main__':
    asyncio.run(main())
```

---

## Inter-Node Connectivity
- Sound API: [[AlphaMiniTextToSpeechAndSoundAPI]]
- Expressions API: [[AlphaMiniExpressionsAndMouthLampAPI]]
- Actions API: [[AlphaMiniMotionAndLocomotionAPI]]
- Behaviors API: [[AlphaMiniBehaviorsAndDanceAPI]]
- Action Catalog: [[AlphaMiniBuiltInActionsCatalog]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 03 (`Labsheet 03 - Alpha Mini Basics in Python.docx`):**
> *"Challenge time (60mins). Program the Alpha Mini to do the following:*
> *The Mouth LED should be WHITE whenever Alpha Mini is talking.*
> *If Alpha Mini is not doing anything, the Mouth LED should be pulsing GREEN at 3 seconds interval.*
> *Your program structure should be like all the programs we had seen so far: the top of the file should be all the import statements followed by some setup code and ends with a teardown. Between the setup and teardown are the steps. Each of the steps (1-8) should be a separate function followed by the Python input statement to wait for user to press the ENTER key.*
> *[ADVANCED] Can you make the robot Mouth LED WHITE while it is talking? Similarly, for step 7 (Hug and say 'I love you'), try to make the robot do this simultaneously.*
> *The filename should be Lab03-challenge-[Student ID].py (e.g. Lab03-challenge-0034.py).*
> *Step Table: 0: Connect; 1: Greet; 2: Enter; 3: Smile; 4: Enter; 5: Sneeze; 6: Enter; 7: Hug and say 'I love you'; 8: Enter; 9: Exit program."*
