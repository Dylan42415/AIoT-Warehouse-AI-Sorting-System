# Protocol: Bluetooth Low Energy (BLE) & GATT

**Bluetooth Low Energy (BLE)** (Bluetooth 4.0/5.0) is an ultra-low-power wireless Personal Area Network (PAN) protocol operating in the 2.4 GHz ISM band. It is supported natively on the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] via the **u-blox NINA-B306** module (Nordic nRF52840).

---

## 1. Network Topology & Roles (GAP Architecture)

The **Generic Access Profile (GAP)** defines how devices advertise, discover, and establish wireless links:

```
  +--------------------------------+               +--------------------------------+
  |  GATT Server / Peripheral      |               |  GATT Client / Central         |
  |  Arduino Nano 33 BLE Sense     | <-----------> |  Raspberry Pi SBC / Smartphone |
  |  - Broadcasts Advertising pkts |   BLE Link    |  - Scans for Advertisements    |
  |  - Hosts Services & Data       |               |  - Initiates Connection        |
  |  - Accepts Reads / Writes      |               |  - Requests Reads / Writes     |
  +--------------------------------+               +--------------------------------+
```

1. **Peripheral Role (Server):** Low-power sensor node that advertises its presence and accepts incoming connections.
2. **Central Role (Client):** Gateway or host device (such as the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]]) that scans for advertising packets, initiates the connection, and reads/writes sensor attributes.

---

## 2. Generic Attribute Profile (GATT) Data Hierarchy

GATT structures data hierarchically using Universally Unique Identifiers (UUIDs):

```
  [ Profile ]
       |
       +--> [ Service (UUID) ]
                 |
                 +--> [ Characteristic (UUID) ]
                 |         |-- Value (Byte Array)
                 |         |-- Properties (Read, Write, Notify)
                 |         +-- Descriptors (CCCD)
                 |
                 +--> [ Characteristic (UUID) ]
```

1. **Service:** A logical collection of related sensor data and control points identified by a 16-bit or 128-bit UUID.
   - Example Custom Service UUID: `19B10000-E8F2-537E-4F6C-D104768A1214`
2. **Characteristic:** A discrete data value container containing:
   - **Value:** The raw binary payload (e.g., an 8-bit integer, float, or string).
   - **Properties:** Access permissions:
     - `BLERead`: Client can read value.
     - `BLEWrite`: Client can write value to control hardware (e.g., toggle an LED).
     - `BLENotify`: Server pushes unsolicited value updates to client without polling.
   - Example Characteristic UUID: `7299a27d-8110-4eb7-b67b-ad03cec64c02`

---

## 3. Arduino Implementation (`ArduinoBLE.h`)

The official `ArduinoBLE` library provides event-driven callback handlers:

```cpp
#include <ArduinoBLE.h>

// Declare Service and Characteristic
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic switchCharacteristic("7299a27d-8110-4eb7-b67b-ad03cec64c02", BLERead | BLEWrite);

void setup() {
  BLE.begin();
  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();
}

void loop() {
  BLE.poll(); // Keep BLE stack responsive
}
```

Detailed in [[Code-Arduino-BLE-Callback-Basic|Code Arduino BLE Callback Basic]] and [[Lab-16-Bluetooth-Communication|Lab 16 Bluetooth Communication]].

---

## 4. SBC Asynchronous Python Client (`bleak`)

On the SBC gateway, the `bleak` library connects to the GATT server asynchronously:

```python
import asyncio
from bleak import BleakClient

DEVICE_UUID = "XX:XX:XX:XX:XX:XX" # Nano 33 BLE MAC Address
CHAR_UUID   = "7299a27d-8110-4eb7-b67b-ad03cec64c02"

async def main():
    async with BleakClient(DEVICE_UUID) as client:
        if client.is_connected:
            print("Connected to Nano 33 BLE Sense!")
            data = await client.read_gatt_char(CHAR_UUID)
            print(f"Characteristic Value: {int.from_bytes(data, byteorder='little')}")

asyncio.run(main())
```

Detailed in [[Code-Python-SBC-03-BLE-ThingSpeak|Code Python SBC BLE ThingSpeak]] and [[Code-Python-SBC-BLE-Scanner|Code Python SBC BLE Scanner]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab16_Bluetooth_Communication.pdf` (Pages 1-4) & `03_Lab Sheets/Code Example for students/AIoT_Arduino/BLE_Callback_Basic/BLE_Callback_Basic.ino`
> 
> "Bluetooth Low Energy (BLE) is designed for very low power operation. In BLE, devices can act as Central or Peripheral... The Generic Attribute Profile (GATT) establishes how data is organized into Services and Characteristics... Service UUID: 19B10000-E8F2-537E-4F6C-D104768A1214, Characteristic UUID: 7299a27d-8110-4eb7-b67b-ad03cec64c02... In Python, BleakClient allows asynchronous discovery, connection, and data retrieval."
