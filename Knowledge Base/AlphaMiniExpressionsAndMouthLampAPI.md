# Alpha Mini Expressions and Mouth Lamp API

The visual affective feedback of the **Alpha Mini Robot** is driven by two distinct hardware channels: the dual 1.22-inch LCD eye screens (emoticon graphics) and the tricolor RGB mouth LED. These are controlled via the `mini.apis.api_expression` module.

---

## Core Expression API Classes & Enums

```python
from mini.apis.api_expression import (
    PlayExpression,
    PlayExpressionResponse,
    SetMouthLamp,
    SetMouthLampResponse,
    MouthLampColor,
    MouthLampMode,
    ControlMouthLamp,
    ControlMouthResponse
)
```

---

## 1. Eye Facial Expressions: `PlayExpression`

Controls animated facial graphics rendered across the dual 240×240 pixel LCD eye screens.
- **Constructor Parameters:**
  - `express_name: str`: The identifier string of the built-in emoticon animation (see full catalog in [[AlphaMiniBuiltInEmoticonsCatalog]]).
- **Return Value:** `Tuple[MiniApiResultType, PlayExpressionResponse]`
  - `response.isSuccess: bool`
  - `response.resultCode: int`

```python
# Play 'Looking Around' Emoticon
block = PlayExpression(express_name="codemao1")
resultType, response = await block.execute()
assert response.isSuccess, "Failed to render emoticon"

# Play 'Smile' Emoticon
smile_block = PlayExpression(express_name="emo_007")
await smile_block.execute()
```

---

## 2. Advanced Mouth LED Lamp Control: `SetMouthLamp`

Configures color, animation mode, duration, and breathing rates of the mouth RGB LED indicator.

- **Constructor Parameters:**
  - `color: MouthLampColor`:
    - `MouthLampColor.RED = 1`
    - `MouthLampColor.GREEN = 2`
    - `MouthLampColor.BLUE = 3`
    *(Note: White is achieved through system default speech rendering).*
  - `mode: MouthLampMode`:
    - `MouthLampMode.NORMAL = 0`: Solid illumination. The `duration` parameter dictates how long the LED stays on.
    - `MouthLampMode.BREATH = 1`: Pulsing / breathing illumination. The `breath_duration` parameter dictates the cycle rate.
  - `duration: int`: Total operational duration in milliseconds. Passing `-1` sets the lamp to remain on permanently.
  - `breath_duration: int`: The duration of a single breathing pulse cycle in milliseconds (active when `mode=BREATH`).

```python
# Example 1: Constant Green Lamp for 3 seconds
green_block = SetMouthLamp(
    color=MouthLampColor.GREEN,
    mode=MouthLampMode.NORMAL,
    duration=3000,
    breath_duration=1000
)
resultType, response = await green_block.execute()

# Example 2: Pulsing Green Breathing Lamp at 3-second cycle (Lab 3 Idle state)
idle_block = SetMouthLamp(
    color=MouthLampColor.GREEN,
    mode=MouthLampMode.BREATH,
    duration=-1,           # Continuous
    breath_duration=3000   # 3s per breath
)
await idle_block.execute()
```

---

## 3. Basic Mouth Lamp Toggle: `ControlMouthLamp`

Provides simple binary activation or deactivation of the mouth LED:
- `is_open: bool`: `True` to turn the mouth lamp on; `False` to turn it off.

```python
# Turn Mouth Lamp OFF
block = ControlMouthLamp(is_open=False)
resultType, response = await block.execute()

# Turn Mouth Lamp ON
block_on = ControlMouthLamp(is_open=True)
await block_on.execute()
```

---

## Inter-Node Connectivity
- Hardware Indicators: [[AlphaMiniButtonsAndLEDIndicators]]
- Complete Emoticon List: [[AlphaMiniBuiltInEmoticonsCatalog]]
- Behavior API: [[AlphaMiniBehaviorsAndDanceAPI]]
- Lab 3 Challenge Protocol: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 03 (`Labsheet 03 - Alpha Mini Basics in Python.docx`) & `test_expression.py`:**
> *"On line 25, an object block of type PlayExpression is instantiated with a named parameter express_name. The named parameter express_name contains the expression type from a list of built-in expressions for the robot to express.*
> *On line 96, an object block of type SetMouthLamp is instantiated with a pulsing colored LED for 3 seconds (duration=3000), pulsating at a rate of 1 second (breath_duration=1000).*
> *mode: 嘴巴灯模式，0：普通模式，1：呼吸模式*
> *color: 嘴巴灯颜色，1：红色，2：绿色，3：蓝色*
> *duration: 持续时间，单位为毫秒，-1表示常亮*
> *breath_duration: 闪烁一次时长，单位为毫秒*
> *Instead of SetMouthLamp, it instantiates a ControlMouthLamp object with a named parameter is_open. This is a Boolean flag that controls the state of the LED."*
