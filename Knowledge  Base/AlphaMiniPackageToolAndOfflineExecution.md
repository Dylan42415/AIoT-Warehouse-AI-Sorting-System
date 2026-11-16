# Alpha Mini Package Tool and Offline Execution

The **Alpha Mini Package Tool** (`mini.pkg_tool`) provides administrative and deployment functions to manage internal Python packages (`.whl`), upload Python scripts directly into the robot's internal storage, and trigger standalone **offline execution** on the robot's embedded Android OS without requiring an active PC Wi-Fi connection.

---

## Module Overview: `mini.pkg_tool`

The `pkg_tool` module allows developers to treat Alpha Mini as an edge compute platform:
- Inspect installed Python packages on the robot.
- Install third-party binary wheels (e.g. `simple_socket-0.0.2-py3-none-any.whl`).
- Package custom local Python modules into installable wheels using `setup_py_pkg()`.
- Upload standalone execution scripts to the robot filesystem and launch them as background daemons.

---

## Package Management APIs

```python
import mini.pkg_tool as Tool

robot_id = "0090"  # Last 4-5 digits of robot serial number

# 1. Query info on a specific package installed in robot
info = Tool.query_py_pkg(pkg_name="alphamini", robot_id=robot_id)
print(f"Alphamini package info: {info}")

# 2. List all Python packages installed inside robot
pkg_list = Tool.list_py_pkg(robot_id=robot_id)
print(f"Installed packages on robot: {pkg_list}")

# 3. Install a local .whl file into robot
Tool.install_py_pkg(package_path="simple_socket-0.0.2-py3-none-any.whl", robot_id=robot_id, debug=True)

# 4. Uninstall a package from robot
Tool.uninstall_py_pkg(pkg_name="simple-socket", robot_id=robot_id, debug=True)

# 5. Build and package a local folder (e.g. tts_demo) into a wheel (.whl)
wheel_path = Tool.setup_py_pkg("tts_demo")
print(f"Generated package wheel: {wheel_path}")

# 6. Install and run packaged module offline
Tool.install_py_pkg(package_path=wheel_path, robot_id=robot_id, debug=False)
Tool.run_py_pkg(pkg_name="tts_demo", robot_id=robot_id)
```

---

## Script Upload & Remote Daemon Execution

Using `pt.upload_script`, developers can push raw Python script strings to the robot and execute them independently of the host workstation:

### Action Command Codes
`pt.upload_script(action_code, device, filename, content_bytes)` supports 5 core action modes:
1. `action=1`: **Upload Script** to robot storage.
2. `action=2`: **Check Script Status** (verifies if script file exists on robot).
3. `action=3`: **Run Script** (triggers embedded execution on robot).
4. `action=4`: **Stop Script** (terminates active running script process).
5. `action=5`: **List Uploaded Scripts** (returns directory listing of uploaded scripts).

### Implementation Example (`test_pkg_tool_upload_script.py`)

```python
import mini
import mini.pkg_tool as pt

device = "0717"
fileName = "DanceTest.py"

py_script_dance = """import asyncio
import mini
from mini.apis.api_behavior import StartBehavior

async def main():
    wifiDevice = mini.WiFiDevice(port=8800)
    connected = await mini.mini_sdk.connect(wifiDevice)
    if connected:
        await mini.mini_sdk.enter_program()
        block = StartBehavior(name="dance_0004")
        await block.execute()
        await mini.mini_sdk.quit_program()
        await mini.mini_sdk.release()

if __name__ == '__main__':
    asyncio.run(main())
"""

# Upload script to robot
pt.upload_script(1, device, fileName, bytes(py_script_dance, encoding="utf-8"))

# Verify script is uploaded
pt.upload_script(2, device, fileName)

# Execute script on robot
pt.upload_script(3, device, fileName)

# Stop script execution
# pt.upload_script(4, device)

# List all uploaded scripts
# pt.upload_script(5, device)
```

---

## Inter-Node Connectivity
- SDK Architecture: [[AlphaMiniPythonSDKArchitecture]]
- Connection Lifecycle: [[AlphaMiniConnectionLifecycle]]
- Hardware Specifications: [[AlphaMiniHardwareSpecifications]]
- Text-to-Speech API: [[AlphaMiniTextToSpeechAndSoundAPI]]

---

## Source Excerpt

> **Verbatim Excerpt from `test_pkg_tool.py` & `test_pkg_tool_upload_script.py`:**
> *"# 将当前目录下的tts_demo工程打包成py wheel， 返回值为打包生成后的wheel路径*
> *pkg_path = Tool.setup_py_pkg('tts_demo')*
> *Tool.uninstall_py_pkg(pkg_name='tts_demo', robot_id=robot_id)*
> *Tool.install_py_pkg(package_path=pkg_path, robot_id=robot_id, debug=False)*
> *Tool.run_py_pkg('tts_demo', robot_id=robot_id)*
> *def test_upload_script(): pt.upload_script(1, device, fileName, bytes(py_script_tts, encoding='utf-8'))*
> *def test_check_upload_script(): pt.upload_script(2, device, fileName)*
> *def test_run_upload_script(): pt.upload_script(3, device, fileName)*
> *def test_stop_upload_script(): pt.upload_script(4, device)*
> *def test_list_upload_script(): pt.upload_script(5, device)"*
