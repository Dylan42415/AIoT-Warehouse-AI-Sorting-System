# Alpha Mini Text-to-Speech and Sound API

The sound and speech subsystem in the **Alpha Mini Python SDK** is managed by the `mini.apis.api_sound` module. It provides classes for text-to-speech synthesis (TTS), local audio playback, online audio streaming, volume control, and audio interruption.

---

## Core Sound API Classes

```python
from mini.apis.api_sound import (
    StartPlayTTS,
    StopPlayTTS,
    ControlTTSResponse,
    PlayAudio,
    PlayAudioResponse,
    AudioStorageType,
    FetchAudioList,
    GetAudioListResponse,
    AudioSearchType,
    StopAllAudio,
    StopAudioResponse,
    ChangeRobotVolume,
    ChangeRobotVolumeResponse
)
```

---

## 1. Text-to-Speech (TTS) Synthesis

### `StartPlayTTS`
Synthesizes speech from a text string and plays it through the robot's dual stereo speakers while animating the mouth LED.

- **Constructor Parameters:**
  - `text: str`: The text string to be synthesized. Supports English, Chinese, and other languages.
  - `is_serial: bool` (default `True`):
    - `is_serial=True`: Blocking execution mode; `await block.execute()` blocks until the robot finishes speaking the entire utterance.
    - `is_serial=False`: Non-blocking (asynchronous dispatch); `await block.execute()` returns immediately once the speech command is dispatched to the robot.
- **Return Value:** `Tuple[MiniApiResultType, ControlTTSResponse]`
  - `response.isSuccess: bool`
  - `response.resultCode: int` (0 = Success; query errors via `errors.get_speech_error_str(response.resultCode)`)

```python
# Synchronous / Blocking TTS
block = StartPlayTTS(text="Hello, my name is Alpha Mini!")
resultType, response = await block.execute()
assert response.isSuccess, f"TTS error: {errors.get_speech_error_str(response.resultCode)}"

# Non-blocking TTS (allows simultaneous motion)
block_async = StartPlayTTS(is_serial=False, text="I am speaking while walking.")
await block_async.execute()
```

### `StopPlayTTS`
Immediately aborts any ongoing text-to-speech playback:
```python
block = StopPlayTTS()
resultType, response = await block.execute()
```

---

## 2. Audio Playback: Local Presets & Online Streaming

### `PlayAudio`
Plays pre-recorded audio files from either the robot's internal flash storage or an external HTTP URL.

- **Constructor Parameters:**
  - `url: str`: The identifier or URL of the audio file.
    - For local storage: The name of the built-in preset (e.g. `"read_016"` or `"horn"`).
    - For online storage: The absolute HTTP/HTTPS URL (supports MP3, AMR, WAV).
  - `storage_type: AudioStorageType`:
    - `AudioStorageType.PRESET_LOCAL`: Built-in factory audio stored in robot ROM.
    - `AudioStorageType.CUSTOM_LOCAL`: User audio stored on internal storage (`/sdcard/customize/music`).
    - `AudioStorageType.NET_PUBLIC`: Remote streaming audio URL over the Internet.

```python
# Play Built-in Local Audio
local_block = PlayAudio(url="horn", storage_type=AudioStorageType.PRESET_LOCAL)
resultType, response = await local_block.execute()

# Play Online Audio File
online_block = PlayAudio(
    url="http://hao.haolingsheng.com/ring/000/995/52513bb6a4546b8822c89034afb8bacb.mp3",
    storage_type=AudioStorageType.NET_PUBLIC
)
resultType, response = await online_block.execute()
```

---

## 3. Querying Available Audio Resources: `FetchAudioList`

Retrieves a directory listing of audio files currently accessible on the robot:
- `search_type: AudioSearchType`:
  - `AudioSearchType.INNER`: Built-in immutable factory sound effects.
  - `AudioSearchType.CUSTOM`: Developer files stored in `/sdcard/customize/music`.
- **Response:** `GetAudioListResponse` containing an array of `Audio` objects:
  - `audio.name: str`: Sound effect name.
  - `audio.suffix: str`: File extension (e.g. `.mp3`).

```python
block = FetchAudioList(search_type=AudioSearchType.INNER)
resultType, response = await block.execute()
for audio in response.audio:
    print(f"Sound effect: {audio.name}.{audio.suffix}")
```

---

## 4. Master Audio Stop & Volume Configuration

### `StopAllAudio`
Terminates all active audio tracks, background music, and TTS streams simultaneously:
```python
block = StopAllAudio()
resultType, response = await block.execute()
```

### `ChangeRobotVolume`
Adjusts the hardware output volume of Alpha Mini's dual 0.8W speakers:
- `volume: float`: Floating-point scalar from `0.0` (mute) to `1.0` (maximum loudness).
```python
# Set volume to 50%
block = ChangeRobotVolume(volume=0.5)
resultType, response = await block.execute()
```

---

## Inter-Node Connectivity
- Hardware Specs: [[AlphaMiniHardwareSpecifications]]
- Speech Observer: [[AlphaMiniSpeechRecognitionObserver]]
- Lab 3 Protocol: [[Lab03-AlphaMiniBasicsAndChallengeProtocol]]
- Facial Expressions: [[AlphaMiniExpressionsAndMouthLampAPI]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 03 (`Labsheet 03 - Alpha Mini Basics in Python.docx`) & `test_sound.py`:**
> *"On line 29, StartPlayTTS is used to instantiate the object block which is of type StartPlayTTS. A named parameter text is used to pass the text to be converted to speech.*
> *The other function test_stop_play_tts is a variation of the test_play_tts. It uses the named parameter is_serial=false to signal to the StartPlayTTS constructor not to wait for the conversion to return a result.*
> *PlayAudio: url表示要播放的音效列表, storage_type=AudioStorageType.NET_PUBLIC or AudioStorageType.PRESET_LOCAL.*
> *FetchAudioList: search_type: AudioSearchType.INNER 是指机器人内置的不可修改的音效.*
> *ChangeRobotVolume: volume: 0~1.0. 设置机器人音量为0.5，等待回复结果."*
