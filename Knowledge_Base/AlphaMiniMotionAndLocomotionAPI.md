# Alpha Mini Motion and Locomotion API

The physical actuation, limb gestures, and bipedal locomotion of the **Alpha Mini Robot** are commanded through the `mini.apis.api_action` module.

---

## Core Action API Classes & Enums

```python
from mini.apis.api_action import (
    PlayAction,
    PlayActionResponse,
    MoveRobot,
    MoveRobotDirection,
    MoveRobotResponse,
    GetActionList,
    GetActionListResponse,
    RobotActionType
)
```

---

## 1. Single Action Execution: `PlayAction`

Commands the robot to perform a discrete physical action file (e.g. push-ups, squats, hugs, leg raises).

- **Constructor Parameters:**
  - `action_name: str`: The name or numeric code of the action file (see full catalog in [[AlphaMiniBuiltInActionsCatalog]]).
- **Interruptibility Note:** Some actions (e.g. `009` Reset, `010` Laughing, `random_short2` Hug) can be interrupted mid-stride, whereas heavy structural balance actions (e.g. `012` Push-ups, `024` Yoga, `031` Squat) cannot be interrupted until physically completed.

```python
# Execute '018' (Raise Right Leg)
block = PlayAction(action_name="018")
resultType, response = await block.execute()
assert response.isSuccess, f"Action failed with code {response.resultCode}"

# Execute Hug Action ('random_short2')
hug_block = PlayAction(action_name="random_short2")
await hug_block.execute()
```

---

## 2. Bipedal Locomotion: `MoveRobot`

Commands Alpha Mini to take a specified number of bipedal steps in a cardinal direction.

- **Constructor Parameters:**
  - `step: int`: The number of discrete steps to execute.
  - `direction: MoveRobotDirection`:
    - `MoveRobotDirection.FORWARD`
    - `MoveRobotDirection.BACKWARD`
    - `MoveRobotDirection.LEFTWARD`
    - `MoveRobotDirection.RIGHTWARD`

```python
# Move 10 steps to the left
block = MoveRobot(step=10, direction=MoveRobotDirection.LEFTWARD)
resultType, response = await block.execute()
assert response.isSuccess, f"Locomotion failed: {response.code}"

# Step 5 steps forward
forward_block = MoveRobot(step=5, direction=MoveRobotDirection.FORWARD)
await forward_block.execute()
```

---

## 3. Querying Action Library: `GetActionList`

Retrieves the list of action files installed on the robot:
- `action_type: RobotActionType`:
  - `RobotActionType.INNER`: Built-in immutable factory action files stored in internal system firmware.
  - `RobotActionType.CUSTOM`: User-defined motion sequences placed in `/sdcard/customize/action`.

```python
block = GetActionList(action_type=RobotActionType.INNER)
resultType, response = await block.execute()
print(f"Action list result: {response.action_list}")
```

---

## Selected Built-in Actions Reference

| Action Code | Motion Description | Interruptible? |
| :--- | :--- | :--- |
| `009` | Reset to standard standing pose | Yes |
| `012` | Push-ups | No |
| `016` | Golden Rooster Independent (one-leg stand) | No |
| `024` | Yoga | No |
| `010` | Laughing with body gesture | Yes |
| `random_short2` | Hug | Yes |
| `027` | Sit down | No |
| `031` | Squat | No |
| `013` | Kung Fu | No |
| `017` | Raise your hands | Yes |
| `random_short3` | Waving left hand | Yes |
| `random_short4` | Waving right hand | Yes |

*(For the complete, uncompressed 37-action table, refer to [[AlphaMiniBuiltInActionsCatalog]]).*

---

## Inter-Node Connectivity
- Action Catalog: [[AlphaMiniBuiltInActionsCatalog]]
- Hardware Joints: [[AlphaMiniHardwareSpecifications]]
- Kinematics & Mobility: [[RoboticsLocomotionMechanisms]]
- Posture Observers: [[AlphaMiniSensoryAndPostureObservers]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 03 (`Labsheet 03 - Alpha Mini Basics in Python.docx`) & `test_action.py`:**
> *"On line 26, the function instantiates a PlayAction object block with the named parameter action_name. This action_name specifies the action to perform.*
> *The test_move_robot function moves the robot in the specified direction several steps indicated by the named parameter step. It instantiates an object block of type MoveRobot and run it's execute method to perform the action.*
> *block: MoveRobot = MoveRobot(step=10, direction=MoveRobotDirection.LEFTWARD)*
> *action_type: INNER 是指机器人内置的不可修改的动作文件, CUSTOM 是放置在sdcard/customize/action目录下可被开发者修改的动作.*
> *block: GetActionList = GetActionList(action_type=RobotActionType.INNER)"*
