# Alpha Mini Development Environment Setup

This document provides the exhaustive, step-by-step engineering protocol for setting up the local host development workstation to program the **Alpha Mini Robot** using Python and the official `alphamini` SDK.

---

## Toolchain & Software Prerequisites

The software environment relies on Anaconda distribution on a 64-bit Windows PC:
1. **Anaconda Distribution:** Package and environment manager providing Python, conda CLI, Anaconda Prompt, and Jupyter Notebook.
2. **Dedicated Virtual Environment (`testAI`):** Required to isolate Python 3.7 and prevent dependency collisions with modern Python 3.10+ runtimes.
3. **AlphaMini Python SDK (`alphamini`):** Official UBTECH client library providing asynchronous communication primitives.
4. **Sample Codebase Repository:** `mini_demo-master` repository maintained by UBTECH (`https://github.com/marklogg/mini_demo`).
5. **Code Editor:** Visual Studio Code (`code .`) or Jupyter Notebook (`jupyter-notebook`).

---

## Step-by-Step Installation Protocol

### Step 1: Base Anaconda Installation & Maintenance
1. Download Anaconda from `https://www.anaconda.com` and execute the installer using standard default settings.
2. Launch **Anaconda Prompt** from the Windows Start menu.
3. Update the core Conda package manager:
```shell
conda update -n base -c defaults conda
```

### Step 2: Virtual Environment Provisioning (`testAI`)
Because the Alpha Mini SDK binary wheel and internal socket bindings rely on Python 3.7 runtime specifications, the environment must be pinned explicitly:
```shell
conda create --name testAI python=3.7 anaconda tensorflow=2.0
```
- When prompted with `Proceed ([y]/n)?`, input `y` and press **Enter**.
- Verify creation and activate the environment:
```shell
conda activate testAI
```
*(Notice the terminal prefix prompt switches from `(base)` to `(testAI)`).*

### Step 3: Alpha Mini Python SDK Installation
Within the activated `(testAI)` environment, install the SDK via pip:
```shell
pip install alphamini
```

### Step 4: Sample Project Workspace Setup
1. Create a workspace directory on the Desktop:
```shell
mkdir C:\Users\<Username>\OneDrive\Desktop\testAI
cd C:\Users\<Username>\OneDrive\Desktop\testAI
```
2. Download the sample project ZIP from GitHub: `https://github.com/marklogg/mini_demo`.
3. Extract the contents into `Desktop/testAI/mini_demo-master`.
4. Ensure the file structure is unnested so that `Desktop/testAI/mini_demo-master/test/__init__.py` is directly accessible.
5. Launch VS Code in the test folder:
```shell
cd mini_demo-master\test
code .
```

### Step 5: Critical Code Import Correction
In all sample test scripts within `mini_demo-master/test/` (e.g. `test_sound.py`, `test_expression.py`, `test_action.py`), VS Code may flag imports with wavy lines:
```python
# INCORRECT (causes ModuleNotFoundError when running inside test directory):
from test.test_connect import test_connect, shutdown

# CORRECT (edit to direct module import):
from test_connect import test_connect, shutdown
```
*Note: This modification must be applied whenever running scripts directly from within the `test` directory.*

---

## Robot Configuration Cross-Check
Before executing scripts:
1. Turn on Alpha Mini by pressing and holding the backpack power button until mouth LED illuminates.
2. Check battery level; connect charger if low.
3. Double-tap the head touch sensor to confirm Wi-Fi SSID matches the host workstation's subnet.
4. Find the serial number on the barcode label under the backpack; record the last 5 characters (e.g., `"00879"`).

---

## Inter-Node Connectivity
- Hardware Specs: [[AlphaMiniHardwareSpecifications]]
- Wi-Fi Setup: [[AlphaMiniWiFiAndNetworkConfiguration]]
- First Connection: [[AlphaMiniConnectionLifecycle]]
- Lab 1 Guide: [[Lab01-AlphaMiniDevEnvironmentSetupProtocol]]
- SDK Architecture: [[AlphaMiniPythonSDKArchitecture]]

---

## Source Excerpt

> **Verbatim Excerpt from Labsheet 01 (`Labsheet 01 - Setup AlphaMini Development Environment.docx`) & Slide 10–14 `SRA Lesson 2 - v1.0.pptx`:**
> *"Update your Anaconda installation: Run the following command as shown in the prompt: conda update -n base -c defaults conda*
> *Create a virtual environment to encapsulate the specific python version and libraries pertaining to this development. Type the following into the prompt and run it. This will create a virtual environment named testAI and download the relevant python version 3.7 and dependencies: conda create --name testAI python=3.7 anaconda tensorflow=2.0*
> *Enter 'y' at the prompt Proceed ([y]/n)? to continue. Activate the virtual environment testAI if not already activated. The prompt shows (base) if no virtual environment is active, else it will show the name of the virtual environment. conda activate testAI*
> *Install the AlphaMini SDK for Python: Type the following in the prompt: pip install alphamini*
> *Open your browser and go to https://github.com/marklogg/mini_demo. Click Download ZIP and save file to your Desktop/testAI folder."*
