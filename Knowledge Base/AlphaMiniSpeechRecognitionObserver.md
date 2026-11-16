# Alpha Mini Speech Recognition Observer

The **Speech Recognition Observer** (`ObserveSpeechRecognise` from `mini.apis.api_observe`) enables the Alpha Mini Robot to act as an acoustic conversational interface, transcribing spoken voice into text in real-time and pushing it to Python handlers.

---

## Class Signature & Response Data Structure

```python
from mini.apis.api_observe import ObserveSpeechRecognise
from mini.pb2.codemao_speechrecognise_pb2 import SpeechRecogniseResponse
```

### `SpeechRecogniseResponse` Fields:
- `msg.text: str`: The transcribed natural language speech string recognized by the robot's onboard ASR engine.
- `msg.isSuccess: bool`: True if audio was captured and transcribed successfully.
- `msg.resultCode: int`: Result status code (0 = Success).

---

## Hotword Dispatch & Dialogue Implementation (`demo_speech_recognize.py`)

Below is the complete implementation showing hotword detection, response triggering, and termination:

```python
import asyncio
from mini.apis.api_observe import ObserveSpeechRecognise
from mini.apis.api_sound import StartPlayTTS
from mini.dns.dns_browser import WiFiDevice
from mini.pb2.codemao_speechrecognise_pb2 import SpeechRecogniseResponse
from test_connect import test_connect, shutdown, test_get_device_by_name, test_start_run_program

async def __tts():
    """Audible response upon hotword detection."""
    block = StartPlayTTS(text="你好， 我是悟空， 啦里啦，啦里啦")
    response = await block.execute()
    print(f"TTS completed: {response}")

async def test_speech_recognise():
    """Configure and activate speech recognition observer."""
    observe: ObserveSpeechRecognise = ObserveSpeechRecognise()

    def handler(msg: SpeechRecogniseResponse):
        print(f"======= Received Speech Recognized: {msg}")
        print(f"Transcribed Text: {msg.text}")

        # Hotword 1: "悟空" (Wukong) triggers greeting
        if str(msg.text).lower() == "悟空":
            print("Hotword recognized! Speaking response...")
            asyncio.create_task(__tts())

        # Hotword 2: "结束" (Stop/End) terminates listener and shuts down loop
        elif str(msg.text).lower() == "结束":
            print("Termination command received. Halting observer...")
            observe.stop()
            # Stop the running event loop cleanly
            asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)

    observe.set_handler(handler)
    observe.start()
    await asyncio.sleep(0)

if __name__ == '__main__':
    device: WiFiDevice = asyncio.get_event_loop().run_until_complete(test_get_device_by_name())
    if device:
        asyncio.get_event_loop().run_until_complete(test_connect(device))
        asyncio.get_event_loop().run_until_complete(test_start_run_program())
        asyncio.get_event_loop().run_until_complete(test_speech_recognise())
        
        # Keep loop running to receive speech frames
        asyncio.get_event_loop().run_forever()
        asyncio.get_event_loop().run_until_complete(shutdown())
```

---

## Role in Cloud Chatbot Pipeline (Lesson 6)

In the advanced conversational architecture (SRA Lesson 6), `ObserveSpeechRecognise` serves as the **Front-End Acoustic Ingestion Engine**:
1. Human speaks to Alpha Mini.
2. `ObserveSpeechRecognise` intercepts utterance and extracts `msg.text`.
3. Handler sends `msg.text` via HTTP POST to the cloud-hosted [[FlaskAndNgrokCloudIntegration|Flask/Ngrok]] endpoint.
4. [[NLPAndNeuralIntentsChatbot|NeuralIntents]] generates a reply.
5. Handler plays reply using [[AlphaMiniTextToSpeechAndSoundAPI|StartPlayTTS]].

---

## Inter-Node Connectivity
- Observer Architecture: [[AlphaMiniObserverPatternArchitecture]]
- Sound & TTS API: [[AlphaMiniTextToSpeechAndSoundAPI]]
- Cloud Chatbot Pipeline: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Natural Language Processing: [[NLPAndNeuralIntentsChatbot]]

---

## Source Excerpt

> **Verbatim Excerpt from `demo_speech_recognize.py` & `test_event.py`:**
> *"监听语音识别demo: 监听语音识别事件，机器人上报语音识别后的文字.*
> *当识别到语音为'悟空'时，播报'你好， 我是悟空， 啦里啦，啦里啦'*
> *当识别到语音为'结束'时，停止监听*
> *# SpeechRecogniseResponse.text*
> *# SpeechRecogniseResponse.isSuccess*
> *if str(msg.text).lower() == '悟空': asyncio.create_task(__tts())*
> *elif str(msg.text).lower() == '结束': observe.stop(); asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)"*
