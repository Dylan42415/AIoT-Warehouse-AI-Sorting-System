# Cloud: ThingSpeak IoT Analytics Platform

**ThingSpeak** (developed by MathWorks) is an open-source IoT analytics platform service that enables aggregating, visualizing, and analyzing live telemetry data streams in the cloud. It provides interactive visual dashboard widgets and built-in MATLAB analytics engines.

---

## 1. Architectural Model & Channels

The fundamental organizational unit in ThingSpeak is a **Channel**:
- Each channel contains up to **8 individual data fields** (`field1` through `field8`).
- Metadata fields include: `Latitude`, `Longitude`, `Elevation`, `Status`, and `Tag`.
- Each channel is uniquely identified by a numerical **Channel ID** (e.g., `1927000`).

```
  +-------------------------------------------------------------+
  |                   ThingSpeak Channel (ID: 1927000)          |
  +-------------------------------------------------------------+
  |  Field 1: CPU Utilization (%) or Proximity Value (0-255)    |
  |  Field 2: Person Detection Confidence (0.0 - 1.0)           |
  |  Field 3: Ambient Temperature (°C)                          |
  |  Field 4: Status Indicator ("Alert" / "Normal")             |
  +-------------------------------------------------------------+
```

---

## 2. Authentication & API Security Keys

Every channel generates two 16-character alphanumeric API keys:
1. **Write API Key:** Mandatory secret token required to write or append data to the channel. Must be included in every HTTP POST/GET request or MQTT session.
2. **Read API Key:** Required to read private channel data programmatically. (Public channels do not require a read key).

---

## 3. Data Ingestion Protocols: REST API vs. MQTT

ThingSpeak supports two primary data ingestion mechanisms:

### Method 1: HTTP REST API (GET / POST)
- **Endpoint URL:** `https://api.thingspeak.com/update`
- **Query Parameters:** `api_key=<WRITE_API_KEY>&field1=<VALUE>&field2=<VALUE>`
- **Example Request:**
  ```http
  GET https://api.thingspeak.com/update?api_key=A1B2C3D4E5F6G7H8&field1=42 HTTP/1.1
  Host: api.thingspeak.com
  ```
- Evaluated in [[Lab-A-How-to-Send-Data-from-Arduino-to-Cloud|Lab A]] and [[Lab-05-Setup-Cloud-ThingSpeak|Lab 05]].

### Method 2: MQTT Broker Telemetry
- **Host:** `mqtt3.thingspeak.com` on port `1883` (unencrypted) or `8883` (TLS).
- Detailed in [[Protocol-MQTT-Message-Queuing-Telemetry|Protocol MQTT]].

### Ingestion Rate Limits:
- Free educational accounts enforce an update rate limit of **1 message every 15 seconds**. Rapid updates sent under 15 seconds are dropped with HTTP status `0` or TCP disconnections.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab05_Setup_Cloud.pdf` (Pages 1-6) & `Lab06_Update ThingSpeak channel from SBC by MQTT.pdf`
> 
> "ThingSpeak is an IoT analytics platform service that allows you to aggregate, visualize, and analyze live data streams in the cloud. You can send data to ThingSpeak from your devices, create instant visualizations of live data, and send alerts... Each channel has 8 fields. API Keys are used to read and write data... Rate limit is 15 seconds per update for free channels."
