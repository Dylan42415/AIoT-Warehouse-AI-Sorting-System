# Alpha Mini Sensory and Posture Observers

This document details the reactive telemetry observers for environmental obstacle range (`ObserveInfraredDistance`) and whole-body physical posture / fall status (`ObserveRobotPosture`).

---

## 1. Infrared Distance Observer: `ObserveInfraredDistance`

Continuously streams distance telemetry measured by the chest ToF infrared optical rangefinder.

### `ObserveInfraredDistanceResponse` Structure:
- `msg.distance: int`: Distance to nearest forward obstacle in millimeters (mm).

### Obstacle Avoidance Trigger (`demo_infrared.py`):
When distance drops below 500 mm (0.5 meters), the robot halts the observer and audibly announces the obstacle distance:

```python
import asyncio
from mini.apis.api_observe import ObserveInfraredDistance
from mini.apis.api_sound import StartPlayTTS
from mini.pb2.codemao_observeinfrareddistance_pb2 import ObserveInfraredDistanceResponse

async def test_ObserveInfraredDistance():
    observer = ObserveInfraredDistance()

    def handler(msg: ObserveInfraredDistanceResponse):
        print(f"Current distance: {msg.distance} mm")
        # Trigger condition: Obstacle closer than 500 mm
        if msg.distance < 500:
            print("Obstacle within 500mm! Stopping observer...")
            observer.stop()
            asyncio.create_task(__tts(msg.distance))

    observer.set_handler(handler)
    observer.start()
    await asyncio.sleep(0)

async def __tts(distance: int):
    await StartPlayTTS(text=f"检测到红外距离{distance}毫米").execute()
    asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## 2. Robot Posture & Fall Detection: `ObserveRobotPosture`

Monitors the internal 6-axis IMU to determine physical pose and detect balance loss (falls).

### Complete `RobotPosture` Enum Specification:

| Value Code | Enum Constant | Physical Pose Meaning |
| :---: | :--- | :--- |
| `1` | `STAND` | Standard upright bipedal standing |
| `2` | `SPLITS_LEFT` | Left lunge / split stance |
| `3` | `SPLITS_RIGHT` | Right lunge / split stance |
| `4` | `SITDOWN` | Seated posture |
| `5` | `SQUATDOWN` | Squatting posture |
| `6` | `KNEELING` | Kneeling posture |
| `7` | `LYING` | Lateral fall / Lying on side |
| `8` | `LYINGDOWN` / `LYING_DOWN`| Flat fall / Lying on back or stomach |
| `9` | `SPLITS_LEFT_1` | Deep left split |
| `10` | `SPLITS_RIGHT_2`| Deep right split |
| `11` | `BEND` | Torso bent forward |

### Fall Detection Implementation (`demo_robot_posture.py`):
If the robot tips over onto its side (`LYING`) or back (`LYINGDOWN`), it halts the listener and broadcasts a distress message:

```python
import asyncio
from mini.apis.api_observe import ObserveRobotPosture, RobotPosture
from mini.apis.api_sound import StartPlayTTS
from mini.pb2.codemao_observefallclimb_pb2 import ObserveFallClimbResponse

async def test_ObserveRobotPosture():
    observer = ObserveRobotPosture()

    def handler(msg: ObserveFallClimbResponse):
        print(f"Posture update code: {msg.status}")
        # Detect fall: status 7 (LYING) or status 8 (LYING_DOWN)
        if msg.status in (RobotPosture.LYING.value, RobotPosture.LYING_DOWN.value):
            print("ALERT: Robot has fallen over!")
            observer.stop()
            asyncio.create_task(__tts_fall())

    observer.set_handler(handler)
    observer.start()
    await asyncio.sleep(0)

async def __tts_fall():
    await StartPlayTTS(text="我摔倒了，请帮帮我！").execute()
    asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## Inter-Node Connectivity
- Observer Framework: [[AlphaMiniObserverPatternArchitecture]]
- Hardware Anatomy: [[AlphaMiniAnatomyAndStructure]]
- Safety Protocols: [[AlphaMiniSafetyPrecautionsAndMaintenance]]
- Perception Theory: [[RoboticsSensorsAndPerception]]

---

## Source Excerpt

> **Verbatim Excerpt from `demo_robot_posture.py` & `demo_infrared.py`:**
> *"# demo_infrared.py: 当返回的红外距离小于500时，停止监听，并播报'检测到红外距离xx'*
> *# demo_robot_posture.py: 当机器人侧躺(LYING)或平躺(LYINGDOWN)时，停止监听，并播报'我摔倒了'*
> *# ObserveFallClimbResponse.status:*
> *STAND = 1; SPLITS_LEFT = 2; SPLITS_RIGHT = 3; SITDOWN = 4; SQUATDOWN = 5; KNEELING = 6; LYING = 7; LYINGDOWN = 8; SPLITS_LEFT_1 = 9; SPLITS_RIGHT_2 = 10; BEND = 11;"*
