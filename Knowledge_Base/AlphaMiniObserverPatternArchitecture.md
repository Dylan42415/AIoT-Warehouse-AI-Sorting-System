# Alpha Mini Observer Pattern Architecture

The event-driven reactive programming framework in the **Alpha Mini Python SDK** is implemented through the `mini.apis.api_observe` module. Unlike polling-based query blocks (e.g. `FaceDetect` or `GetInfraredDistance`), Observers run continuous background listeners inside the robot daemon and push telemetry asynchronously to user-defined Python handler callbacks.

---

## The Observer Pattern Lifecycle

```
[ Instantiate Observer ]
           |
           v
[ Register Handler Callback: observer.set_handler(handler) ]
           |
           v
[ Start Background Listener: observer.start() ]
           |
           v
[ Enter Event Loop: loop.run_forever() ] <----+
           |                                  | (Incoming Socket Events)
           v                                  |
[ Event Arrives -> Handler Triggered ] -------+
           |
           v (Stop Condition Met)
[ Stop Observer: observer.stop() ]
           |
           v
[ Exit Loop: loop.run_in_executor(None, loop.stop) ]
```

### The 5 Standard Lifecycle Steps:
1. **Instantiation:** Create an instance of the specific observer class (e.g. `observer = ObserveSpeechRecognise()`).
2. **Handler Registration:** Define a callback function `def handler(msg): ...` and register it via `observer.set_handler(handler)`.
3. **Activation:** Call `observer.start()`. This sends an activation frame over the socket to tell the robot daemon to stream event frames.
4. **Loop Management:** Because observers run continuously in the background, the main thread must keep the asyncio event loop alive using `asyncio.get_event_loop().run_forever()`.
5. **Clean Termination:** When a stopping criterion is met:
   - Call `observer.stop()`.
   - Terminate the event loop cleanly from within the callback using:
     `asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)`.

---

## Comprehensive Observers Catalog

| Observer Class | Listened Telemetry / Event | Response Protobuf Message | Typical Use Cases |
| :--- | :--- | :--- | :--- |
| `ObserveSpeechRecognise` | Automated Speech Recognition (ASR) words | `SpeechRecogniseResponse` | Voice commands, hotword triggers, conversational UI |
| `ObserveFaceDetect` | Number of faces in front of camera | `FaceDetectTaskResponse` | Social engagement, greeting people who approach |
| `ObserveFaceRecognise` | Facial identity matching | `FaceRecogniseTaskResponse` | Personalized greetings, stranger alerts |
| `ObserveHeadRacket` | Head capacitive touch sensor taps | `ObserveHeadRacketResponse` | Double-tap dance, emergency stop, tactile game |
| `ObserveInfraredDistance`| Chest ToF obstacle range | `ObserveInfraredDistanceResponse` | Dynamic obstacle avoidance, collision prevention |
| `ObserveRobotPosture` | 6-axis IMU fall & balance changes | `ObserveFallClimbResponse` | Fall detection, distress announcements, recovery |

---

## Event Loop Coordination Architecture

A critical architectural pitfall in observer programming is event loop deadlocks. Because observer callbacks are executed by the asyncio event loop thread, stopping the loop directly from inside a synchronous handler with `loop.stop()` can cause runtime errors or dropped frames. 

The officially supported pattern bridges the callback through an executor:
```python
# Terminate the loop safely from within an observer callback
asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
```

---

## Inter-Node Connectivity
- Speech Observer: [[AlphaMiniSpeechRecognitionObserver]]
- Vision Observers: [[AlphaMiniVisionObservers]]
- Sensory & Posture Observers: [[AlphaMiniSensoryAndPostureObservers]]
- Event Loop Theory: [[AsyncioEventLoopAndScheduling]]

---

## Source Excerpt

> **Verbatim Excerpt from `test_event.py` & Demo Scripts:**
> *"# 定义了事件监听对象,必须让event_loop.run_forever()*
> *observer.set_handler(handler)*
> *observer.start()*
> *await asyncio.sleep(0)*
> *# 结束event_loop*
> *asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)*
> *# demo_touch.py: 监听拍头事件，当机器人头部被拍击时，上报拍头类型. 当机器人头部被双击时，停止监听，并跳一个舞蹈"*
