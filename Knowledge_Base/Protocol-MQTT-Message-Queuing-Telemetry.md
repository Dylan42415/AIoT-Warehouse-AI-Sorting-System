# Protocol: MQTT Message Queuing Telemetry Transport

**MQTT (Message Queuing Telemetry Transport)** is an extremely lightweight, publish-subscribe network protocol designed for constrained devices and high-latency, low-bandwidth, or unreliable networks. It operates over TCP/IP and acts as the primary telemetry uplink protocol from the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]] to the [[Cloud-ThingSpeak-IoT-Analytics-Platform|ThingSpeak cloud]].

---

## 1. Publish-Subscribe Architecture

Unlike client-server architectures (like HTTP) where clients must poll for updates, MQTT decouples the publisher (data producer) from the subscriber (data consumer) via an intermediary **MQTT Broker**:

```
  [ Publisher (SBC) ]
  Publishes sensor data
  to topic: "channels/1927000/publish"
          |
          v (TCP Port 1883)
  +-----------------------------------------------+
  |            MQTT Broker (ThingSpeak)           |
  |  - Filters messages by topic                  |
  |  - Routes payloads to subscribed listeners    |
  +-----------------------------------------------+
          |
          v
  [ Subscribers (Dashboard, Analytics, Alerts) ]
  Receives live update immediately
```

---

## 2. Quality of Service (QoS) Levels

MQTT defines three end-to-end delivery guarantees:
1. **QoS 0 (At most once):** Best-effort delivery. Messages are sent without acknowledgment; packets may be lost if network drops. Minimal latency and overhead.
2. **QoS 1 (At least once):** Handshake with acknowledgment (`PUBACK`). Guarantees message delivery, but duplicate packets can occur if acknowledgments are delayed.
3. **QoS 2 (Exactly once):** Four-step handshake (`PUBREC`, `PUBREL`, `PUBCOMP`). Guarantees exactly one copy is delivered without duplication. Highest protocol overhead.

---

## 3. ThingSpeak MQTT Broker Specifications & Authentication

ThingSpeak provides a public MQTT broker endpoint supporting MQTT devices:

| Broker Parameter | Configuration Value |
| :--- | :--- |
| **Broker Hostname** | `mqtt3.thingspeak.com` |
| **TCP Port (Unencrypted)** | `1883` |
| **TCP Port (TLS / Encrypted)** | `8883` |
| **Client ID** | Unique 24-character hexadecimal ID generated in ThingSpeak Device Setup |
| **Username** | 24-character hexadecimal username string |
| **Password** | Secret base64-encoded authentication key |
| **Topic Hierarchy** | `channels/<channelID>/publish` |
| **Payload Format** | URL-encoded string: `field1=<val>&field2=<val>&status=MQTTPUBLISH` |

---

## 4. Python Implementation (`paho.mqtt.publish`)

In [[Lab-06-Update-ThingSpeak-Channel-from-SBC-by-MQTT|Lab 06]] and [[Code-Python-SBC-01-ThingSpeak-Basic|Code 01 ThingSpeak Basic]]:

```python
import paho.mqtt.publish as publish
import psutil

channel_ID = "1927000"
hostname   = "mqtt3.thingspeak.com"
topic      = f"channels/{channel_ID}/publish"

# Credentials from ThingSpeak MQTT Device
mqtt_client_ID = "YOUR_CLIENT_ID"
mqtt_username  = "YOUR_USERNAME"
mqtt_password  = "YOUR_PASSWORD"

auth_dict = {
    'client_id': mqtt_client_ID,
    'username': mqtt_username,
    'password': mqtt_password
}

# Collect sensor / system metric
cpu_load = psutil.cpu_percent()
payload  = f"field1={cpu_load}&status=MQTTPUBLISH"

# Publish single message with QoS 0
publish.single(
    topic,
    payload=payload,
    hostname=hostname,
    port=1883,
    auth=auth_dict,
    qos=0
)
print("Published MQTT telemetry payload successfully!")
```

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab06_Update ThingSpeak channel from SBC by MQTT.pdf` (Pages 1-3) & `03_Lab Sheets/Code Example for students/AIoT_SBC/01_thingSpeak_Basic.py`
> 
> "MQTT is an OASIS standard messaging protocol for the Internet of Things (IoT). It is designed as an extremely lightweight publish/subscribe messaging transport that will be ideal for connecting remote devices with a small code footprint and minimal network bandwidth... Broker: mqtt3.thingspeak.com, Port: 1883, Topic: channels/<channelID>/publish... Python library: paho-mqtt."
