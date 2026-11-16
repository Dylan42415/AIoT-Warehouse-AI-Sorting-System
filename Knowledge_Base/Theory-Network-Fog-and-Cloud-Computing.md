# Theory: Network, Fog and Cloud Computing

This document synthesizes Chapter 3 of the AIoT module (`AI53003FP`), detailing network topological models, protocol taxonomies, IP addressing, and the Fog/Edge computing paradigm.

---

## 1. Network Classifications by Geographic Scope

IoT devices interact across three fundamental network boundaries:

```
  +---------------------------------------------------------------+
  |   Personal Area Network (PAN): BLE, Zigbee (1 - 10 meters)    |
  |     +-------------------------------------------------------+ |
  |     |   Local Area Network (LAN): Wi-Fi, Ethernet (100 m)   | |
  |     |     +-----------------------------------------------+ | |
  |     |     |   Wide Area Network (WAN): Cellular, Cloud    | | |
  |     |     +-----------------------------------------------+ | |
  |     +-------------------------------------------------------+ |
  +---------------------------------------------------------------+
```

1. **Personal Area Network (PAN):** Low-power, short-range communications (1–10 meters) linking wearable or localized nodes (e.g., [[Protocol-Bluetooth-Low-Energy-GATT|Bluetooth Low Energy]]).
2. **Local Area Network (LAN):** High-throughput communications within a localized facility (up to 100 meters) using IEEE 802.11 (Wi-Fi) or IEEE 802.3 (Ethernet).
3. **Wide Area Network (WAN):** Long-distance communications connecting distributed sites across cities or continents (Cellular 4G/5G, Internet, Satellite, LoRaWAN).

---

## 2. Network Topologies

- **Star Topology:** Central gateway / hub node connected to multiple peripheral nodes. (e.g., BLE Central connecting to multiple Peripherals; Wi-Fi Access Point connecting to IoT devices). Simple, but the central node represents a single point of failure.
- **Mesh Topology:** Every node can route packets for adjacent nodes. Self-healing and highly resilient, but introduces routing protocol overhead (e.g., Zigbee, Thread).
- **Bus Topology:** Devices share a common physical communication backbone (e.g., I2C bus connecting sensors to the microcontroller).

---

## 3. Internet Protocol (IP) & Networking Foundations

- **IPv4 Addressing:** 32-bit numerical label written in dot-decimal notation (e.g., `192.168.1.105`). Divided into network prefix and host ID via the **Subnet Mask** (e.g., `255.255.255.0` or `/24`).
- **Private IP Ranges (RFC 1918):** `10.0.0.0/8`, `172.16.0.0/12`, and `192.168.0.0/16` reserved for local networks. Translated to public IPs via Network Address Translation (NAT).
- **DHCP (Dynamic Host Configuration Protocol):** Automatically leases IP addresses, default gateways, and DNS server addresses to joining devices.
- **Static IP:** Manually fixed IP assigned to server or gateway nodes (such as the [[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]]) to ensure reliable remote connectivity.

---

## 4. The Cloud, Fog, and Edge Computing Paradigm

Traditional IoT systems streamed raw sensor readings directly to centralized cloud data centers. The massive volume of connected devices necessitated the distributed computing continuum:

```
  [ Cloud Data Center ]   <--- High Latency, Global Aggregation, Massive Storage
          ^
          | WAN
          v
    [ Fog Node ]          <--- Local Gateway (SBC), Protocol Translation, Caching
          ^
          | LAN / Serial
          v
    [ Edge Node ]         <--- Microcontroller (Nano 33 BLE), Real-time TinyML
```

### Architectural Comparison:

| Characteristic | Edge Computing | Fog Computing | Cloud Computing |
| :--- | :--- | :--- | :--- |
| **Location** | Sensor endpoint ([[Arduino-Nano-33-BLE-Sense|Nano 33 BLE]])| Local Gateway ([[Single-Board-Computer-Raspberry-Pi-Gateway|SBC]]) | Centralized Data Center |
| **Latency** | Sub-millisecond (instantaneous)| Low (1–10 ms) | Moderate to High (50–500 ms) |
| **Bandwidth Demand** | Zero network transmission | Moderate (filtered telemetry) | High (if streaming raw data) |
| **Data Privacy** | Maximum (data never leaves chip) | Strong (local network perimeter)| Lower (data crosses internet) |
| **Compute Capacity** | Heavily constrained (<1MB Flash) | Intermediate (1–8 GB RAM) | Virtually infinite scalable |
| **Offline Autonomy** | 100% operational offline | High operational resilience | Non-functional without internet|

---

## Traceability & Source Excerpts

> **Source:** `02_Theory Notes/Chapter 3_Network, Fog and Cloud Computing.pdf` (Pages 1-35)
> 
> "PAN networks cover short distances such as Bluetooth; LAN covers buildings such as Wi-Fi and Ethernet; WAN covers vast geographical regions... Cloud computing provides centralized processing and storage, but suffers from latency and bandwidth limits. Fog and Edge computing bring computational power closer to the data source, improving response times, reducing bandwidth consumption, and protecting privacy."
