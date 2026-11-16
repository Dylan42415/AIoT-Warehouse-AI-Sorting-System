# Code: Python SBC BLE Scanner

This document provides the complete annotated source code and architectural analysis for `discover_py.py` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_SBC/discover_py.py`).

---

## 1. Architectural Overview

This utility script uses the `bleak` library on the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]] to perform an asynchronous Bluetooth Low Energy radio scan. It discovers all nearby advertising BLE peripherals and prints their MAC addresses and advertised local names, enabling students to find their [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] device.

---

## 2. Complete Verbatim Source Code

```python
#!/usr/bin/env python3

import asyncio
from bleak import discover

async def run():
    devices = await discover()
    for d in devices:
        print(d)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
```

---

## 3. Sample Terminal Output & Usage

```bash
$ python3 discover_py.py
E4:F4:02:13:5F:72: Nano33BLE_LED
C8:2B:96:4D:11:0A: Wireless Headset
4A:12:F3:99:81:CC: Unknown
```
Students copy the MAC address corresponding to their board (e.g., `E4:F4:02:13:5F:72`) and paste it into [[Code-Python-SBC-03-BLE-ThingSpeak|03_BLE_thingSpeak_Basic.py]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_SBC/discover_py.py`
> 
> "import asyncio; from bleak import discover; async def run(): devices = await discover(); for d in devices: print(d)... Used to discover BLE peripheral addresses."
