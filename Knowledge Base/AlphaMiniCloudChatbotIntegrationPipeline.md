# Alpha Mini Cloud Chatbot Integration Pipeline

This document details the complete end-to-end integration architecture that bridges the **Alpha Mini Robot** as an embodied conversational front-end with a cloud-hosted **NeuralIntents NLP engine** running on Google Colab (SRA Lesson 6).

---

## System Architecture Diagram

```
+-------------------------------------------------------------------------------+
|                       EDGE DOMAIN: Alpha Mini Robot                           |
|                                                                               |
|  1. Acoustic Capture: 4-Mic Array + Onboard ASR                              |
|  2. Event Trigger: ObserveSpeechRecognise pushes transcribed text             |
|                                                                               |
|  [ Python Client Application on Workstation ]                                 |
|  - Handler extracts msg.text                                                  |
|  - Serializes to JSON payload: {"query": msg.text}                            |
|  - Sends HTTP POST request over WAN to Public URL                             |
+---------------------------------------+---------------------------------------+
                                        |
                                        | Internet (HTTPS via Ngrok Tunnel)
                                        v
+-------------------------------------------------------------------------------+
|                       CLOUD DOMAIN: Google Colab                              |
|                                                                               |
|  [ Flask Web Server (Port 5000) ]                                             |
|  - Ingests HTTP POST at /predict endpoint                                     |
|  - Extracts text from request.json                                            |
|                                                                               |
|  [ NeuralIntents NLP Engine ]                                                 |
|  - Lemmatizes tokens via NLTK                                                 |
|  - Evaluates trained CNN intent model                                         |
|  - Resolves intent tag -> selects candidate response string                   |
|                                                                               |
|  - Returns HTTP 200 JSON Response: {"response": "..."}                        |
+---------------------------------------+---------------------------------------+
                                        |
                                        | HTTP Response Payload
                                        v
+-------------------------------------------------------------------------------+
|                       EDGE REACTION: Alpha Mini Robot                         |
|                                                                               |
|  3. Deserializes HTTP response text                                           |
|  4. Triggers StartPlayTTS(text=reply) -> Dual Stereo Speakers                 |
|  5. Animates Mouth LED (white flicker) & Eye LCDs (emoticons) synchronously   |
+-------------------------------------------------------------------------------+
```

---

## Component Roles & Responsibilities

### 1. Alpha Mini (Front-End)
- **Acoustic Sensing:** Captures user voice via 4-mic beamforming array.
- **Speech Recognition (ASR):** Converts acoustic waveform to string text on-device.
- **Client Transmission:** Forwards transcribed text to cloud via HTTP REST request.
- **Physical Output:** Converts returned cloud response into speech via TTS, accompanied by mouth LED flickering and LCD emotive eye displays.

### 2. Colab Chatbot (Back-End)
- **NLP Engine:** Classifies intent using trained NeuralIntents neural network model.
- **API Server:** Flask server handling routing, JSON serialization, and error trapping.
- **Ingress Gateway:** Ngrok tunnel exposing the Colab virtual machine to public HTTPS requests over the Internet.

---

## Full End-to-End Client Implementation Script

```python
import asyncio
import requests
from mini.apis.api_observe import ObserveSpeechRecognise
from mini.apis.api_sound import StartPlayTTS
from mini.apis.api_expression import PlayExpression, SetMouthLamp, MouthLampColor, MouthLampMode
from mini.dns.dns_browser import WiFiDevice
from mini.pb2.codemao_speechrecognise_pb2 import SpeechRecogniseResponse
from test_connect import test_connect, shutdown, test_get_device_by_name, test_start_run_program

# Public Ngrok URL provided by Google Colab
COLAB_API_URL = "https://xxxx-xx-xxx.ngrok-free.app/predict"

async def speak_and_animate(text: str, expr_name: str = "codemao1"):
    """Simultaneously plays TTS and renders expressive eye animations."""
    # 1. Trigger LCD Expression
    asyncio.create_task(PlayExpression(express_name=expr_name).execute())
    
    # 2. Execute Speech Output (Blocks until speech concludes)
    tts_block = StartPlayTTS(text=text)
    await tts_block.execute()

def query_cloud_chatbot(user_utterance: str) -> str:
    """Synchronous HTTP POST call to Colab Flask endpoint."""
    try:
        payload = {"query": user_utterance}
        response = requests.post(COLAB_API_URL, json=payload, timeout=5)
        if response.status_code == 200:
            return response.json().get("response", "I could not understand that.")
        else:
            return "Sorry, my cloud brain returned an error."
    except Exception as e:
        print(f"Network error querying cloud: {e}")
        return "I am currently having trouble reaching the Internet."

async def run_conversational_robot():
    observer = ObserveSpeechRecognise()

    def on_speech_received(msg: SpeechRecogniseResponse):
        print(f"User Spoke: {msg.text}")
        if msg.isSuccess and len(msg.text) > 0:
            # Check for termination command
            if msg.text.lower() in ("stop", "退出", "结束"):
                print("Exiting conversational loop...")
                observer.stop()
                asyncio.get_running_loop().run_in_executor(None, asyncio.get_running_loop().stop)
                return

            # Query Cloud NLP engine
            print("Querying Colab Brain...")
            bot_reply = query_cloud_chatbot(msg.text)
            print(f"Brain Answer: {bot_reply}")

            # Speak response
            asyncio.create_task(speak_and_animate(bot_reply))

    observer.set_handler(on_speech_received)
    observer.start()
    await asyncio.sleep(0)

if __name__ == '__main__':
    device: WiFiDevice = asyncio.get_event_loop().run_until_complete(test_get_device_by_name())
    if device:
        asyncio.get_event_loop().run_until_complete(test_connect(device))
        asyncio.get_event_loop().run_until_complete(test_start_run_program())
        asyncio.get_event_loop().run_until_complete(run_conversational_robot())
        
        # Enter persistent listener loop
        asyncio.get_event_loop().run_forever()
        asyncio.get_event_loop().run_until_complete(shutdown())
```

---

## Inter-Node Connectivity
- Front-End Speech: [[AlphaMiniSpeechRecognitionObserver]]
- Back-End NLP: [[NLPAndNeuralIntentsChatbot]]
- HTTP REST Architecture: [[RESTfulAPIArchitectureInRobotics]]
- Flask / Ngrok Tunnel: [[FlaskAndNgrokCloudIntegration]]
- Lab 6 Protocol: [[Lab06-AlphaMiniColabIntegrationProtocol]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 2, `SRA Lesson 6 - v1.0.pptx`:**
> *"Overview:*
> *Alpha Mini is the front-end:*
> *- Speech recognition*
> *- Send text to Colab Chatbot*
> *- Text-to-Speech response from Chatbot*
> *Colab Chatbot is the back-end:*
> *- NLP engine*
> *- Expose API to Alpha Mini using Flask-NGROK*
> *- Process text from Alpha Mini*
> *- Generate possible response to text*
> *- Relay response to Alpha Mini*
> *Internet: Speech Recognition -> Text-to-Speech; Query -> Response; Response -> Query."*
