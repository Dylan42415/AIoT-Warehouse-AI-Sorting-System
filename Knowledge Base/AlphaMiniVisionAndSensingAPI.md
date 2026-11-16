# Alpha Mini Vision and Sensing API

The visual perception, facial intelligence, object recognition, and infrared distance telemetry of the **Alpha Mini Robot** are commanded via polling-style query blocks from the `mini.apis.api_sence` module.

---

## Core Vision & Sensing Classes & Enums

```python
from mini.apis.api_sence import (
    FaceDetect,
    FaceDetectResponse,
    FaceAnalysis,
    FaceAnalyzeResponse,
    FaceRecognise,
    FaceRecogniseResponse,
    GetRegisterFaces,
    GetRegisterFacesResponse,
    ObjectRecognise,
    RecogniseObjectResponse,
    ObjectRecogniseType,
    GetInfraredDistance,
    GetInfraredDistanceResponse,
    TakePicture,
    TakePictureResponse,
    TakePictureType
)
```

---

## 1. Face Detection & Quantitative Counting: `FaceDetect`

Scans the 13 MP camera feed for human faces within a designated timeout window.
- **Constructor Parameters:** `timeout: int = 10` (duration in seconds to scan).
- **Response Fields:**
  - `response.count: int`: Number of faces detected.
  - `response.isSuccess: bool`
  - `response.resultCode: int`

```python
block = FaceDetect(timeout=10)
resultType, response = await block.execute()
print(f"Detected {response.count} faces in front of robot.")
```

---

## 2. Face Analysis (Demographics & Bounding Box): `FaceAnalysis`

Performs demographic and spatial analysis on the primary human face (the face occupying the largest area in the frame).
- **Constructor Parameters:** `timeout: int = 10`
- **Response Fields & Metric Scales:**
  - `response.age: int`: Estimated chronological age in years.
  - `response.gender: int`: Numeric gender confidence scale ranging from `1` to `100`:
    - **`< 50`**: Classified as **Female**.
    - **`> 50`**: Classified as **Male**.
  - `response.height: int`: Height of face bounding box in pixels.
  - `response.width: int`: Width of face bounding box in pixels.

```python
block = FaceAnalysis(timeout=10)
resultType, response = await block.execute()
gender_str = "Male" if response.gender > 50 else "Female"
print(f"Face Profile: Age={response.age}, Gender={gender_str} ({response.gender}), Box={response.width}x{response.height}")
```

---

## 3. Face Identification & Registered Face Database: `FaceRecognise` & `GetRegisterFaces`

Matches detected faces against the local facial database registered via the AlphaMini mobile app.
- **`FaceRecognise(timeout=10)`:**
  - `response.faceInfos: List[FaceInfoResponse]`
    - `face.id: str`: Internal unique face record ID.
    - `face.name: str`: Registered person's name. **If stranger, defaults to `"stranger"`**.
    - `face.gender: int`
    - `face.age: int`
- **`GetRegisterFaces()`:**
  - Queries all registered face profiles currently stored on the robot.

```python
block = FaceRecognise(timeout=10)
resultType, response = await block.execute()
for face in response.faceInfos:
    print(f"Identified Person: {face.name} (ID: {face.id})")
```

---

## 4. Object Recognition: `ObjectRecognise`

Performs edge visual categorization across three specific object classes.
- **Constructor Parameters:**
  - `object_type: ObjectRecogniseType`:
    - `ObjectRecogniseType.FLOWER`: Identifies plant and floral species.
    - `ObjectRecogniseType.FRUIT`: Identifies fruit items.
    - `ObjectRecogniseType.GESTURE`: Identifies human hand gestures.
  - `timeout: int = 10`
- **Response Fields:**
  - `response.objects: List[str]`: Array of recognized object classification labels.

```python
# Recognize Flower
block = ObjectRecognise(object_type=ObjectRecogniseType.FLOWER, timeout=10)
resultType, response = await block.execute()
print(f"Recognized Flowers: {response.objects}")
```

---

## 5. Infrared Distance Measurement: `GetInfraredDistance`

Reads the chest ToF infrared range sensor to detect proximity to obstacles in front of the robot.
- **Response Fields:**
  - `response.distance: int`: Measured obstacle distance in millimeters (mm).

```python
block = GetInfraredDistance()
resultType, response = await block.execute()
print(f"Forward obstacle distance: {response.distance} mm")
```

---

## 6. Camera Capture: `TakePicture`

Triggers image capture using the 13 MP camera.
- **Constructor Parameters:**
  - `take_picture_type: TakePictureType`:
    - `TakePictureType.IMMEDIATELY`: Snaps photo instantly.
    - `TakePictureType.FINDFACE`: Waits until a human face is framed before snapping.
- **Response Fields:**
  - `response.picPath: str`: Absolute storage path of captured photo on robot filesystem.

```python
block = TakePicture(take_picture_type=TakePictureType.IMMEDIATELY)
resultType, response = await block.execute()
print(f"Photo captured and saved to: {response.picPath}")
```

---

## Inter-Node Connectivity
- Hardware Specs: [[AlphaMiniHardwareSpecifications]]
- Vision Observers: [[AlphaMiniVisionObservers]]
- Infrared Observers: [[AlphaMiniSensoryAndPostureObservers]]
- Perception Theory: [[RoboticsSensorsAndPerception]]

---

## Source Excerpt

> **Verbatim Excerpt from `test_sence.py`:**
> *"# 测试人脸分析(性别): 侦测人脸信息(性别、年龄)，超时时间10s，并等待回复结果. 当多人存在摄像头前时，返回占画面比例最大的那个人脸信息.*
> *返回值：示例 {'age': 24, 'gender': 99, 'height': 238, 'width': 238}*
> *gender：[1, 100], 小于50为女性，大于50为男性.*
> *# object_type: 支持FLOWER, FRUIT, GESTURE 三类物体.*
> *# take_picture_type: IMMEDIATELY-立即拍照, FINDFACE-找到人脸再拍照 两种拍照效果.*
> *# FaceRecogniseResponse.faceInfos : [FaceInfoResponse] 人脸信息数组. FaceInfoResponse.name : 姓名，如果是陌生人，则默认name为'stranger'."*
