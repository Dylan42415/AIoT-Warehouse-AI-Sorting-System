# Alpha Mini Connection Lifecycle

This document provides the complete, authoritative lifecycle architecture for discovering, connecting, commanding, and cleanly disconnecting from an **Alpha Mini Robot** over local Wi-Fi.

---

## Lifecycle State Machine Diagram

```
[Start Program]
       |
       v
+-------------------------------------------------------------+
| 1. Device Discovery: MiniSdk.get_device_by_name("00879", 10)|
|    - Broadcasts mDNS query across 2.4GHz subnet             |
|    - Returns WiFiDevice(name, ip, port, ...)                |
+------------------------------+------------------------------+
                               |
                               v (device found)
+-------------------------------------------------------------+
| 2. TCP Handshake: await MiniSdk.connect(device)             |
|    - Connects to Robot WebSocket daemon on port 8800        |
|    - Returns bool: True (success) / False (failure)         |
+------------------------------+------------------------------+
                               |
                               v (connected)
+-------------------------------------------------------------+
| 3. Enter Programming Mode: await MiniSdk.enter_program()    |
|    - Locks robot autonomy; prepares execution pipeline      |
|    - Robot announces: "Enter Programming Mode" (delay ~6s)  |
+------------------------------+------------------------------+
                               |
                               v
+-------------------------------------------------------------+
| 4. Command Execution: API Blocks & Observer Listeners       |
|    - TTS, Expressions, Actions, Vision, Sensing, Dances     |
+------------------------------+------------------------------+
                               |
                               v (execution complete)
+-------------------------------------------------------------+
| 5. Teardown & Release: shutdown()                           |
|    - await MiniSdk.quit_program() (releases servos/autonomy)|
|    - await MiniSdk.release()      (terminates sockets/DNS)  |
+-------------------------------------------------------------+
       |
       v
  [Program End]
```

---

## Exhaustive Code Implementation (`test_connect.py`)

Below is the complete, dissected source code of `test_connect.py` as documented in Labsheet 01 and SRA Lesson 4:

```python
import asyncio
import logging

import mini.mini_sdk as MiniSdk
from mini.dns.dns_browser import WiFiDevice


# 1. Search for robot with specified serial number suffix
async def test_get_device_by_name() -> WiFiDevice:
    """Search for devices based on robot serial number suffix.

    To search for a robot with a specified serial number (behind the robot's butt),
    you only need to enter the last few characters (any length) of the serial number.
    It is recommended to use more than 5 characters for an accurate match.
    Timeout is 10 seconds.

    Returns:
        WiFiDevice: includes robot name, ip, port, etc.
    """
    # Replace "00879" with the last 5 digits of your robot's serial number
    result: WiFiDevice = await MiniSdk.get_device_by_name("00879", 10)
    print(f"test_get_device_by_name result: {result}")
    return result


# 2. Search for all robots on local subnet (Optional utility)
async def test_get_device_list():
    """Scan all devices on the local network within 10s timeout."""
    results = await MiniSdk.get_device_list(10)
    print(f"test_get_device_list results = {results}")
    return results


# 3. Connect to target WiFiDevice
async def test_connect(dev: WiFiDevice) -> bool:
    """Connect device using its WiFiDevice descriptor.

    Args:
        dev (WiFiDevice): target device WiFiDevice

    Returns:
        bool: True on success, False on failure
    """
    return await MiniSdk.connect(dev)


# 4. Activate Programming Mode
async def test_start_run_program():
    """Activate programming mode and wait for robot acknowledgment.
    Robot announces 'Enter Programming Mode' via TTS; recommended delay ~6s.
    """
    await MiniSdk.enter_program()
    await asyncio.sleep(6)


# 5. Shutdown, quit programming mode, and release resources
async def shutdown():
    """Disconnect device and cleanly release local and remote resources."""
    await MiniSdk.quit_program()
    await MiniSdk.release()


# 6. Global Configuration & Execution Entrypoint
MiniSdk.set_log_level(logging.INFO)
MiniSdk.set_robot_type(MiniSdk.RobotType.DEDU)


async def main():
    device: WiFiDevice = await test_get_device_by_name()
    if device:
        connected = await test_connect(device)
        print(f"Connection status: {connected}")
        if connected:
            await test_start_run_program()
            # Insert application API blocks here
        await shutdown()


if __name__ == '__main__':
    asyncio.run(main())
```

---

## Common Connection Failure Modes & Remedies

| Error / Failure Symptom | Underlying Cause | Corrective Action |
| :--- | :--- | :--- |
| `test_get_device_by_name` returns `None` after 10s | Host PC and Robot are on different subnets or AP isolation is active | Switch both PC and Alpha Mini to a dedicated Mobile Hotspot. Verify IP subnet. |
| Robot IP shows 0.0.0.0 on head double-tap | Robot failed to obtain DHCP lease from router | Rebind Wi-Fi via mobile app; ensure 2.4 GHz band is active. |
| Connection refused on port 8800 | Robot firmware is below v1.4.0, or robot is not fully booted | Check firmware version in app (> v1.4.0); wait 40s until robot announces *"I am ready"*. |
| Robot hangs / commands ignored | Previous script crashed without calling `quit_program()` | Short-press power button on backpack to force-quit programming mode, or restart robot. |

---

## Inter-Node Connectivity
- Environment Setup: [[AlphaMiniDevelopmentEnvironmentSetup]]
- Wi-Fi Details: [[AlphaMiniWiFiAndNetworkConfiguration]]
- Python SDK Core: [[AlphaMiniPythonSDKArchitecture]]
- Sound Testing: [[AlphaMiniTextToSpeechAndSoundAPI]]

---

## Source Excerpt

> **Verbatim Excerpt from Slide 4–9, `SRA Lesson 4 - v1.0.pptx`:**
> *"Dissect test_connect.py: result: WiFiDevice = await MiniSdk.get_device_by_name('00879', 10)*
> *To search for a robot with a specified serial number (behind the robot's butt), you only need to enter the last few characters (any length) of the serial number. It is recommended to use more than 5 characters for an accurate matched. Timeout is 10 seconds.*
> *return await MiniSdk.connect(dev)*
> *await MiniSdk.enter_program(): Activate programming mode and wait, and then delay for (asyncio.sleep) another 6 seconds to let the robot finish playing (tts) 'Enter Programming Mode'*
> *await MiniSdk.quit_program(); await MiniSdk.release()*
> *async def main(): device: WiFiDevice = await test_get_device_by_name(); if device: await test_connect(device); await shutdown()*
> *if __name__ == '__main__': asyncio.run(main())"*
