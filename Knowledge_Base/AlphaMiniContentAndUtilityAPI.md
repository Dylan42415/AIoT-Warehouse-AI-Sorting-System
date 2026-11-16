# Alpha Mini Content and Utility API

The **Alpha Mini Content and Utility API** encompasses encyclopedia knowledge lookup, multi-engine translation services, and low-level mDNS network device browsing, provided by `mini.apis.api_content` and `mini.dns.dns_browser`.

---

## 1. Encyclopedia Information Query: `QueryWiKi`

Enables Alpha Mini to query online knowledge bases (Baidu Baike / Wikipedia) and automatically speak the retrieved summary aloud to the user.
- **Constructor Parameters:** `query: str` (the keyword topic to look up).
- **Execution Effect:** Robot retrieves the encyclopedia summary and reads it out using its internal TTS engine.

```python
from mini.apis.api_content import QueryWiKi, WikiResponse

# Query encyclopedia entry for UBTECH
block = QueryWiKi(query="UBTECH Robotics")
resultType, response = await block.execute()
assert response.isSuccess, "Wiki query failed"
```

---

## 2. Multi-Engine Language Translation: `StartTranslate`

Translates text across languages using cloud translation engines (Baidu, Google, or Tencent) and reads the translated text aloud.
- **Constructor Parameters:**
  - `query: str`: Source text to translate.
  - `from_lan: LanType`: Source language (e.g. `LanType.CN`, `LanType.EN`).
  - `to_lan: LanType`: Target language.
  - `platform: str` (Optional): Translation backend provider (`BAIDU`, `GOOGLE`, or `TENCENT`).

```python
from mini.apis.api_content import StartTranslate, TranslateResponse, LanType

# Translate Chinese text to English
block = StartTranslate(query="你好，世界", from_lan=LanType.CN, to_lan=LanType.EN)
resultType, response = await block.execute()
assert response.isSuccess, "Translation failed"
```

---

## 3. mDNS Service Browser: `WiFiDeviceListener`

Provides asynchronous network discovery callbacks when Alpha Mini robots appear or disappear on the local Wi-Fi network:

```python
import asyncio
from mini.dns.dns_browser import WiFiDeviceListener, WiFiDevice, browser

class CustomDeviceListener(WiFiDeviceListener):
    def on_device_found(self, device: WiFiDevice):
        print(f"Discovered Alpha Mini: {device.name} at {device.ip}:{device.port}")

    def on_device_updated(self, device: WiFiDevice) -> None:
        print(f"Device updated: {device.name}")

    def on_device_removed(self, device: WiFiDevice) -> None:
        print(f"Device disconnected: {device.name}")

async def run_discovery():
    listener = CustomDeviceListener()
    browser().add_listener(listener)
    browser().start_scan(10000)  # Scan for 10 seconds
    await asyncio.sleep(10)
    browser().stop_scan()
```

---

## Inter-Node Connectivity
- Connection Lifecycle: [[AlphaMiniConnectionLifecycle]]
- Text-to-Speech API: [[AlphaMiniTextToSpeechAndSoundAPI]]
- Cloud Integration: [[AlphaMiniCloudChatbotIntegrationPipeline]]
- Speech Observer: [[AlphaMiniSpeechRecognitionObserver]]

---

## Source Excerpt

> **Verbatim Excerpt from `test_content.py` & `test_wifi_browser.py`:**
> *"# 测试, 查询wiki: 查询百科，查询内容'优必选'，并等待结果，机器人播报查询结果.*
> *block: QueryWiKi = QueryWiKi(query='优必选')*
> *# 测试翻译接口: 使用百度翻译，把'张学友'，从中文翻译成英文，并等待结果，机器人播报翻译结果.*
> *block: StartTranslate = StartTranslate(query='张学友', from_lan=LanType.CN, to_lan=LanType.EN)*
> *# from_lan: 源语言, to_lan: 目标语言, platform: BAIDU, GOOGLE, TENCENT*
> *class MyDeviceListener(WiFiDeviceListener): def on_device_found(self, device: WiFiDevice): print(self, 'onDeviceFound:', device)"*
