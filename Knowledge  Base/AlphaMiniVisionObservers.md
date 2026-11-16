# Alpha Mini Vision and Touch Observers

This document covers reactive edge vision and physical touch observers: `ObserveFaceDetect`, `ObserveFaceRecognise`, and `ObserveHeadRacket`.

---

## 1. Face Detection Observer: `ObserveFaceDetect`

Continuously analyzes video frames from the 13 MP camera and pushes an event whenever the number of visible human faces changes or is detected.

### `FaceDetectTaskResponse` Structure:
- `msg.count: int`: Number of faces detected.
- `msg.isSuccess: bool`: Operation success flag.
- `msg.resultCode: int`: Error/Status code.

### Implementation Pattern (`demo_face_detect.py`):
```python
import asyncio
from mini.apis.api_observe import ObserveFaceDetect
from mini.apis.api_sound import StartPlayTTS
from mini.pb2.codemao_facedetecttask_pb2 import FaceDetectTaskResponse

async def test_ObserveFaceDetect():
    observer = ObserveFaceDetect()

    def handler(msg: FaceDetectTaskResponse):
        print(f"Face detection event: {msg}")
        if msg.isSuccess and msg.count >= 1:
            print(f"Detected {msg.count} faces. Halting observer...")
            observer.stop()
            asyncio.create_task(__tts(msg.count))

    observer.set_handler(handler)
    observer.start()
    await asyncio.sleep(0)

async def __tts(count):
    await StartPlayTTS(text=f"在我面前好像有{count}个人").execute()
    asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## 2. Face Recognition Observer: `ObserveFaceRecognise`

Performs continuous biometric identification against the registered face library on the robot.

### `FaceRecogniseTaskResponse` Structure:
- `msg.faceInfos: List[FaceInfoResponse]`
  - `face.id: str`: Registered ID.
  - `face.name: str`: Person's name; **if unknown/stranger, returns `"stranger"`**.
  - `face.gender: int`: Numeric gender scale.
  - `face.age: int`: Estimated age.

### Implementation Pattern (`demo_face_recognize.py`):
```python
import asyncio
from mini.apis.api_observe import ObserveFaceRecognise
from mini.apis.api_sound import StartPlayTTS
from mini.pb2.codemao_facerecognisetask_pb2 import FaceRecogniseTaskResponse

async def test_ObserveFaceRecognise():
    observer = ObserveFaceRecognise()

    def handler(msg: FaceRecogniseTaskResponse):
        print(f"Face recognized event: {msg}")
        if msg.isSuccess and msg.faceInfos:
            name = msg.faceInfos[0].name
            print(f"Identified: {name}")
            observer.stop()
            asyncio.create_task(__tts(name))

    observer.set_handler(handler)
    observer.start()
    await asyncio.sleep(0)

async def __tts(name):
    greeting = f"你好，{name}" if name != "stranger" else "你好，陌生人"
    await StartPlayTTS(text=greeting).execute()
    asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## 3. Head Capacitive Touch Observer: `ObserveHeadRacket`

Detects physical tap gestures on the crown capacitive sensor on top of the head.

### `HeadRacketType` Enum & Response Fields:
- `msg.type: int`: The gesture classification code:
  - `HeadRacketType.SINGLE_CLICK = 1`: Single quick tap.
  - `HeadRacketType.LONG_PRESS = 2`: Sustained touch (> 2 seconds).
  - `HeadRacketType.DOUBLE_CLICK = 3`: Rapid two-tap gesture.

### Implementation Pattern (`demo_touch.py`):
```python
import asyncio
from mini.apis.api_behavior import StartBehavior
from mini.apis.api_observe import ObserveHeadRacket, HeadRacketType
from mini.pb2.codemao_observeheadracket_pb2 import ObserveHeadRacketResponse

async def test_ObserveHeadRacket():
    observer = ObserveHeadRacket()

    def handler(msg: ObserveHeadRacketResponse):
        print(f"Head touch event code: {msg.type}")
        if msg.type == HeadRacketType.DOUBLE_CLICK.value:
            print("Double-click detected! Halting observer and dancing...")
            observer.stop()
            asyncio.create_task(__dance())

    observer.set_handler(handler)
    observer.start()
    await asyncio.sleep(0)

async def __dance():
    await StartBehavior(name="dance_0002").execute()
    asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## Inter-Node Connectivity
- Observer Overview: [[AlphaMiniObserverPatternArchitecture]]
- Vision Polling API: [[AlphaMiniVisionAndSensingAPI]]
- Buttons & Controls: [[AlphaMiniButtonsAndLEDIndicators]]
- Behavior & Dance: [[AlphaMiniBehaviorsAndDanceAPI]]

---

## Source Excerpt

> **Verbatim Excerpt from `demo_face_detect.py`, `demo_face_recognize.py`, `demo_touch.py`:**
> *"# demo_face_detect.py: 当检测到人脸个数大于等于1个时，停止监听，并播报'在我面前好像有xx个人脸'(xx为人脸个数)*
> *# demo_face_recognize.py: 如果是已注册的人脸，返回人脸详细信息：id，名字，性别，年龄; 如果是陌生人，返回 name: 'stranger'*
> *# demo_touch.py: class HeadRacketType(enum.Enum): SINGLE_CLICK = 1; LONG_PRESS = 2; DOUBLE_CLICK = 3.*
> *if msg.type == HeadRacketType.DOUBLE_CLICK.value: observer.stop(); asyncio.create_task(__dance())"*
