# Alpha Mini Python SDK Architecture

The **Alpha Mini Python SDK** (`alphamini` package, internal namespace `mini`) is an asynchronous, event-driven client library that allows local Python applications to control the robot's locomotion, vision, speech, sensors, and emotive expressions.

---

## Package Architecture & Module Layout

The SDK is organized into a modular hierarchy:

```
mini/
├── mini_sdk.py          # Core gateway: lifecycle, connection, logging, robot type
├── pkg_tool.py          # Package tool: wheel installation, packaging, script execution
├── dns/
│   └── dns_browser.py   # mDNS / DNS-SD discovery engine, WiFiDevice, browser()
├── channels/
│   └── websocket_client # Transport layer communicating with robot daemon (port 8800)
├── pb2/                 # Protocol Buffers compiled response classes
└── apis/                # Functional block APIs and observers
    ├── base_api.py      # BaseApi, MiniApiResultType (Success, Timeout, Fail)
    ├── api_action.py    # MoveRobot, PlayAction, GetActionList
    ├── api_behavior.py  # StartBehavior, StopBehavior (Dances)
    ├── api_expression.py# PlayExpression, SetMouthLamp, ControlMouthLamp
    ├── api_sound.py     # StartPlayTTS, StopPlayTTS, PlayAudio, FetchAudioList, StopAllAudio
    ├── api_sence.py     # FaceDetect, FaceAnalysis, FaceRecognise, ObjectRecognise, TakePicture
    ├── api_observe.py   # Event observers: Speech, Face, IR, Posture, HeadRacket
    └── api_content.py   # QueryWiKi, StartTranslate
```

---

## Core SDK Gateway Primitives (`mini.mini_sdk`)

The entry point for SDK interaction is `mini.mini_sdk as MiniSdk`.

### 1. Logging Level Configuration
Controls SDK diagnostic verbosity in the terminal:
```python
import logging
import mini.mini_sdk as MiniSdk

MiniSdk.set_log_level(logging.INFO)  # Options: logging.DEBUG, logging.INFO, logging.WARNING
```

### 2. Robot Type Declaration
Specifies the hardware variant for protocol framing:
```python
MiniSdk.set_robot_type(MiniSdk.RobotType.DEDU) # Or MiniSdk.RobotType.EDU
```
*Note: In educational environments, use `RobotType.DEDU` or `EDU` depending on robot firmware configuration.*

### 3. Device Discovery Primitives
- `MiniSdk.get_device_by_name(name: str, timeout: int = 10) -> WiFiDevice`:
  Asynchronously broadcasts an mDNS query for a robot whose serial suffix matches `name`.
- `MiniSdk.get_device_list(timeout: int = 10) -> List[WiFiDevice]`:
  Scans all Alpha Mini robots responding on the local subnet within the timeout window.

### 4. Connection & Session Lifecycle Primitives
- `MiniSdk.connect(dev: WiFiDevice) -> bool`:
  Establishes the TCP socket / WebSocket communication channel with the robot.
- `MiniSdk.enter_program() -> bool`:
  Puts Alpha Mini into **Programming Mode**. Robot announces *"Enter Programming Mode"* via TTS. Required before sending motor, speech, or sensor API blocks.
- `MiniSdk.quit_program() -> bool`:
  Exits programming mode and returns robot to default autonomous idle state.
- `MiniSdk.release()`:
  Disconnects socket, terminates event listeners, and releases network threads.

---

## API Execution Pattern: Functional Blocks

All command APIs inherit from `BaseApi` and follow a unified execution pattern:
1. **Instantiate Block:** Pass parameters into the API class constructor.
2. **Execute Asynchronously:** Await `block.execute()`.
3. **Destructure Result:** Returns a 2-element tuple: `(resultType, response)`.
   - `resultType`: `MiniApiResultType.Success`, `Timeout`, or `Failure`.
   - `response`: Specific Protobuf response object containing `.isSuccess`, `.resultCode`, and payload fields.

```python
# Standard Block Execution Template
block: PlayAction = PlayAction(action_name="018")
(resultType, response) = await block.execute()

assert resultType == MiniApiResultType.Success, "Operation timed out"
assert response.isSuccess, f"Operation failed with code {response.resultCode}"
```

---

## Inter-Node Connectivity
- Connection Lifecycle: [[AlphaMiniConnectionLifecycle]]
- Package Management: [[AlphaMiniPackageToolAndOfflineExecution]]
- Sound API: [[AlphaMiniTextToSpeechAndSoundAPI]]
- Action API: [[AlphaMiniMotionAndLocomotionAPI]]
- Vision API: [[AlphaMiniVisionAndSensingAPI]]
- Observer Model: [[AlphaMiniObserverPatternArchitecture]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 3, 6, 8, 9, `SRA Lesson 4 - v1.0.pptx`:**
> *"Dissect test_connect.py: import asyncio; import logging; import mini.mini_sdk as MiniSdk; from mini.dns.dns_browser import WiFiDevice*
> *Import mini_sdk sub-package from the mini package but reference it as MiniSdk.*
> *Setup logging level as INFO: MiniSdk.set_log_level(logging.INFO)*
> *Set robot type: MiniSdk.set_robot_type(MiniSdk.RobotType.DEDU)*
> *Enter programming mode: await MiniSdk.enter_program()*
> *Disconnect and release resources: await MiniSdk.quit_program(); await MiniSdk.release()"*
