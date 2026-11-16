# Lab 05: Setup Cloud (ThingSpeak)

This document provides the exhaustive laboratory execution protocol for **Lab 05: Setup Cloud** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Create and configure an IoT cloud analytics account on **MathWorks ThingSpeak**.
2. Create a new ThingSpeak Channel and configure data field parameters.
3. Obtain and manage **Channel ID**, **Write API Key**, and **Read API Key**.
4. Test manual REST data updates using web browser URL query parameters.
5. Configure visual dashboard widgets (Line charts, Gauges, Numeric displays).

---

## 2. Channel Configuration Protocol

1. Navigate to [ThingSpeak](https://thingspeak.com/) and create a user account.
2. Go to **Channels** > **My Channels** > click **New Channel**.
3. Fill in the channel settings:
   - **Name:** `AIoT Edge Telemetry Gateway`
   - **Description:** `Telemetry and TinyML inference channel`
   - **Field 1:** `Proximity / CPU Load`
   - **Field 2:** `Person Detection Score`
   - **Field 3:** `Actuator State`
4. Click **Save Channel**.

---

## 3. API Key Acquisition & REST Verification

1. Click the **API Keys** tab:
   - Note the **Channel ID** (e.g., `1927000`).
   - Copy the **Write API Key** (16-character alphanumeric string, e.g., `A1B2C3D4E5F6G7H8`).
2. Test manual HTTP GET data ingestion:
   Open a web browser and paste the following URL (substituting your Write API Key):
   ```
   https://api.thingspeak.com/update?api_key=YOUR_WRITE_API_KEY&field1=75
   ```
3. The server responds with an integer indicating the updated entry count (e.g., `1`).
4. Return to the **Private View** tab in ThingSpeak to verify that the point appears on the Field 1 chart.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab05_Setup_Cloud.pdf` (Pages 1-6)
> 
> "ThingSpeak is an IoT analytics platform service that allows you to aggregate, visualize, and analyze live data streams in the cloud... Create a new channel, assign field names, and retrieve the Write API Key and Channel ID... Update the channel using HTTP GET: https://api.thingspeak.com/update?api_key=<WRITE_API_KEY>&field1=value."
