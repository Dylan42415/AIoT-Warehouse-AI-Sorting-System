# Lab 06: Alpha Mini Colab Integration Protocol

This document details the lab protocol for **Lab 6: Alpha Mini + Chatbot in Colab Integration** (SRA Lesson 6).

---

## Lab Objectives

In this lab, students learn how to:
1. Configure Alpha Mini as an acoustic front-end client using `ObserveSpeechRecognise`.
2. Connect the local workstation Python client to the Google Colab cloud backend via an Ngrok HTTPS URL.
3. Transmit transcribed user voice queries over HTTP POST.
4. Synthesize the returned cloud response into speech using `StartPlayTTS`.
5. Trigger simultaneous facial and motion reactions based on the returned conversational intent.

---

## Complete Lab 6 Script Implementation

```python
import asyncio
import requests
from mini.apis.api_observe import ObserveSpeechRecognise
from mini.apis.api_sound import StartPlayTTS
from mini.apis.api_expression import PlayExpression, SetMouthLamp, MouthLampColor, MouthLampMode
from mini.apis.api_behavior import StartBehavior
from mini.dns.dns_browser import WiFiDevice
from mini.pb2.codemao_speechrecognise_pb2 import SpeechRecogniseResponse
from test_connect import test_connect, shutdown, test_get_device_by_name, test_start_run_program

# REPLACE WITH YOUR LIVE PUBLIC NGROK URL GENERATED IN COLAB
COLAB_ENDPOINT = "https://xxxx-xx-xxx.ngrok-free.app/chat"

async def process_robot_response(reply_text: str):
    """Converts cloud response into speech and expressive gestures."""
    print(f"Speaking response: {reply_text}")
    
    # Check if response warrants a dance
    if "dance" in reply_text.lower():
        asyncio.create_task(StartBehavior(name="dance_0004").execute())
    else:
        asyncio.create_task(PlayExpression(express_name="codemao1").execute())
        
    tts_block = StartPlayTTS(text=reply_text)
    await tts_block.execute()

def query_cloud_nlp(user_speech: str) -> str:
    """Sends HTTP POST request to Google Colab server."""
    try:
        response = requests.post(COLAB_ENDPOINT, json={"message": user_speech}, timeout=6)
        if response.status_code == 200:
            return response.json().get("response", "I could not compute a response.")
        return "My cloud server returned an error."
    except Exception as e:
        print(f"HTTP Communication Error: {e}")
        return "I cannot reach the cloud server right now."

async def setup_voice_loop():
    observer = ObserveSpeechRecognise()

    def on_speech(msg: SpeechRecogniseResponse):
        print(f"Recognized Speech: {msg.text}")
        if msg.isSuccess and len(msg.text) > 0:
            if msg.text.lower() in ("stop", "exit", "结束"):
                print("Ending dialogue loop...")
                observer.stop()
                asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
                return

            reply = query_cloud_nlp(msg.text)
            asyncio.create_task(process_robot_response(reply))

    observer.set_handler(on_speech)
    observer.start()
    await asyncio.sleep(0)

async def main():
    device: WiFiDevice = await test_get_device_by_name()
    if device:
        await test_connect(device)
        await test_start_run_program()
        await setup_voice_loop()
        
        # Keep event loop running for continuous conversation
        asyncio.get_event_loop().run_forever()
        await shutdown()

if __name__ == '__main__':
    asyncio.run(main())
```

---

## Inter-Node Connectivity
- Pipeline Architecture: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Speech Observer: [[AlphaMiniSpeechRecognitionObserver]]
- Cloud Server Setup: [[Lab04-ColabChatbotImplementationProtocol]]
- Text-to-Speech API: [[AlphaMiniTextToSpeechAndSoundAPI]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 2 & 5, `SRA Lesson 6 - v1.0.pptx`:**
> *"Lab 6: Alpha Mini + Chatbot in Colab.*
> *Overview: Alpha Mini is the front-end: Speech recognition, Send text to Colab Chatbot, Text-to-Speech response from Chatbot.*
> *Colab Chatbot is the back-end: NLP engine, Expose API to Alpha Mini using Flask-NGROK, Process text from Alpha Mini, Generate possible response to text, Relay response to Alpha Mini."*
