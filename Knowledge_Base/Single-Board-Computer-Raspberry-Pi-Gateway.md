# Single Board Computer Raspberry Pi Gateway

The **Single Board Computer (SBC)** (specifically the **Raspberry Pi 3/4** running Linux) serves as the **Fog Computing Gateway Node** in the AIoT architectural hierarchy. It bridges resource-constrained microcontroller endpoints ([[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]]) with cloud analytics services ([[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak]]).

---

## 1. Architectural Role in the AIoT Ecosystem

```
  [ Edge Sensor Node ]                  [ Fog Gateway Node ]                [ Cloud Analytics ]
  Arduino Nano 33 BLE Sense  =======>  Raspberry Pi SBC (Linux)  ========>  ThingSpeak Cloud
  - APDS-9960 Proximity                - Protocol Conversion                - Data Storage
  - TinyML Inference                   - Data Buffering / Filtering         - Live Dashboard
  (Transmits via USB/UART/BLE)         (Transmits via MQTT / HTTP)          - MATLAB Analytics
```

### Key Responsibilities of the SBC:
1. **Protocol Conversion:** Ingests local serial streams ([[Protocol-UART-Serial-Communication|UART]] / USB Virtual COM) or [[Protocol-Bluetooth-Low-Energy-GATT|BLE GATT]] characteristics and transforms them into IP-based [[Protocol-MQTT-Message-Queuing-Telemetry|MQTT]] or HTTP REST payloads.
2. **Local Data Persistence & Buffering:** Temporarily caches telemetry during internet WAN outages.
3. **Edge Analytics & Orchestration:** Evaluates threshold rules locally before forwarding data upstream.

---

## 2. Remote Access & Headless Network Configuration

As documented in [[Lab-04-Accessing-SBC-Remotely|Lab 04 Accessing SBC Remotely]], the gateway is operated without physical monitors via headless networking:

### 1. SSH Terminal Access (via MobaXterm):
- **Protocol:** Secure Shell (SSH) over TCP Port `22`.
- **Default Credentials:** Username: `pi`, Password: (configured student password).
- **Network Discovery:** Scanning local subnet using tools like Angry IP Scanner or router DHCP tables to discover assigned IP (e.g., `192.168.1.105`).

### 2. Graphical Remote Desktop (via RealVNC Viewer):
- **Protocol:** Virtual Network Computing (VNC) over TCP Port `5900`.
- **Display Resolution:** Default virtual display configured to $1920 	imes 1080$ or $1280 	imes 720$.

---

## 3. Physical Interfacing with the Arduino Nano 33

The SBC communicates with the Arduino endpoint through two primary physical interfaces:

### Interface 1: USB Serial Interface (`/dev/ttyACM0` or `/dev/ttyUSB0`)
- **Connection:** Micro-USB to USB-A cable.
- **Device Node:** Linux kernel instantiates `/dev/ttyACM0` (CDC-ACM driver) or `/dev/ttyUSB0`.
- **Python Driver:** `pyserial` (`serial.Serial('/dev/ttyACM0', 9600)`).
- **Used In:** [[Lab-07-Arduino-to-SBC-Communication|Lab 07]] and [[Code-Python-SBC-02-USB-ThingSpeak|Code 02 USB ThingSpeak]].

### Interface 2: Hardware UART Interface (`/dev/ttyS0` or `/dev/serial0`)
- **Wiring:** 
  - Arduino `TX` (`D1` / `P0.23`) $
ightarrow$ SBC `RX` (GPIO 15 / Pin 10)
  - Arduino `RX` (`D0` / `P1.09`) $\leftarrow$ SBC `TX` (GPIO 14 / Pin 8)
  - Common `GND` $\leftrightarrow$ Common `GND` (Pin 6/9/14/20/25/30/34/39)
- **Used In:** [[Code-Python-SBC-04-UART-Duplex|Code 04 UART Duplex]] and [[Code-Python-SBC-05-UART-ThingSpeak|Code 05 UART ThingSpeak]].

---

## 4. Upstream Cloud Communication Stack

The SBC executes continuous Python 3 daemon scripts utilizing:
- `paho.mqtt.publish`: Publishes telemetry to `mqtt3.thingspeak.com:1883` over [[Protocol-MQTT-Message-Queuing-Telemetry|MQTT]].
- `urllib.request`: Sends HTTP GET requests to `https://api.thingspeak.com/update`.
- `bleak`: Asynchronous BLE client connecting to the Nano 33 BLE GATT server.
- `psutil`: Collects host CPU, memory, and disk health metrics.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab04_Accessing_SBC_Remotely.pdf` (Pages 1-8) & `Lab07_Arduino_SBC_Communication.pdf` (Pages 1-4)
> 
> "A Single Board Computer (SBC) is a complete computer built on a single circuit board, with microprocessor(s), memory, input/output (I/O) and other features required of a functional computer... Accessing the SBC remotely via SSH on port 22 using MobaXterm and VNC Viewer on port 5900."
>
> "Communication between Arduino and SBC can be established using USB cable (/dev/ttyACM0 or /dev/ttyUSB0) or UART serial connections using Python pyserial library at 9600 baud rate."
