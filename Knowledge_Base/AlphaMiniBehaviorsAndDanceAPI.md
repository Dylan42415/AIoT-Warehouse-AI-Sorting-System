# Alpha Mini Behaviors and Dance API

The expressive behavioral gestures and complex choreographic dance routines of the **Alpha Mini Robot** are controlled through the `mini.apis.api_behavior` module.

---

## Core Behavior API Classes

```python
from mini.apis.api_behavior import (
    StartBehavior,
    ControlBehaviorResponse,
    StopBehavior
)
```

---

## 1. Executing Choreographed Dances: `StartBehavior`

Coordinates simultaneous whole-body limb movements, head tracking, eye LCD emotive animations, and synchronized audio playback.
- **Constructor Parameters:**
  - `name: str`: The unique name identifier of the behavior or dance routine (see full catalog in [[AlphaMiniBuiltInDancesCatalog]]).
- **Execution Characteristics:**
  - Dances typically run for 10 to 60 seconds.
  - When awaited synchronously, `await block.execute()` blocks until the entire dance routine concludes.
  - Can be scheduled as a background Task via `asyncio.create_task()` to allow concurrent execution with other logic.

```python
# Execute 'Little Star' Dance (dance_0004)
block = StartBehavior(name="dance_0004")
resultType, response = await block.execute()
assert response.isSuccess, f"Dance failed: {errors.get_express_error_str(response.resultCode)}"
```

---

## 2. Interrupting Active Behavior: `StopBehavior`

Forces the robot to immediately halt its ongoing dance routine or behavior sequence, re-centering its limbs and returning to the default upright standing posture:

```python
import asyncio
from mini.apis.api_behavior import StartBehavior, StopBehavior

async def dance_with_interruption():
    # Start dance in background
    block = StartBehavior(name="dance_0004")
    dance_task = asyncio.create_task(block.execute())

    # Allow dance to run for 5 seconds
    await asyncio.sleep(5)

    # Forcefully halt the behavior
    stop_block = StopBehavior()
    resultType, response = await stop_block.execute()
    print("Dance halted.")
```

---

## Selected Built-in Dance Routines

| Dance Identifier | English Description | Typical Context |
| :--- | :--- | :--- |
| `dance_0001en` | Healthy Song and Dance | Full-body aerobic choreography |
| `dance_0002en` | Shaolin Hero Dance | Martial-arts inspired dynamic poses |
| `dance_0003en` | Youth Training Manual Dance | Pop choreography |
| `dance_0004en` | Little Star | Gentle musical arm & head dance |
| `dance_0005en` | Grass Dance | Swaying torso and arm movements |
| `dance_0006en` | Seaweed Dance | Rhythmic waving motion |
| `014` | Tai Chi | Slow, balanced bipedal weight shifting |
| `custom_0035` | Happy Birthday | Celebratory birthday song & gestures |

*(For the complete, uncompressed 19-routine dance table, refer to [[AlphaMiniBuiltInDancesCatalog]]).*

---

## Inter-Node Connectivity
- Dance Catalog: [[AlphaMiniBuiltInDancesCatalog]]
- Physical Actions: [[AlphaMiniMotionAndLocomotionAPI]]
- Sound & Music: [[AlphaMiniTextToSpeechAndSoundAPI]]
- Lab 3 Guide: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 03 (`Labsheet 03 - Alpha Mini Basics in Python.docx`) & `test_expression.py`:**
> *"On line 45, the code instantiates an object block of type StartBehavior with a named parameter name. This parameter specifies the dance to perform. On line 47, the result is awaited from the asynchronous execute method and destructured into resultType and response.*
> *block: StartBehavior = StartBehavior(name='dance_0004')*
> *# 5秒后停止*
> *await asyncio.sleep(5)*
> *block: StopBehavior = StopBehavior()*
> *(resultType, response) = await block.execute()"*
