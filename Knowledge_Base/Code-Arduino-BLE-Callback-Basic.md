# Code: Arduino BLE Callback Basic

This document provides the complete annotated source code and architectural analysis for `BLE_Callback_Basic.ino` (Source Path: `03_Lab Sheets/Code Example for students/AIoT_Arduino/BLE_Callback_Basic/BLE_Callback_Basic.ino`).

---

## 1. Architectural Overview

The sketch configures the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] as an event-driven **Bluetooth Low Energy (BLE) Peripheral / GATT Server**. Instead of continuously polling states in `loop()`, it registers callback event handlers that trigger asynchronously when a remote Central device (smartphone or [[Single-Board-Computer-Raspberry-Pi-Gateway|Raspberry Pi SBC]]) connects, disconnects, or writes data to the target GATT characteristic.

---

## 2. Complete Verbatim Source Code

```cpp
/*
  LED Callback

  This example creates a BLE peripheral with a service that contains a
  characteristic to control an LED. The value of the characteristic is calculated,
  and event handlers are used to handle incoming events from the central.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    or Arduino Nano 33 BLE Sense board.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic("7299a27d-8110-4eb7-b67b-ad03cec64c02", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("LEDCallback");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handler for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);

  // set an initial value for the characteristic
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // poll for BLE events
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  if (switchCharacteristic.value()) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}
```

---

## 3. Technical Breakdown & Implementation Details

1. **UUID Specifications:**
   - **Service UUID:** `19B10000-E8F2-537E-4F6C-D104768A1214` (128-bit custom vendor service).
   - **Characteristic UUID:** `7299a27d-8110-4eb7-b67b-ad03cec64c02` (`BLEByteCharacteristic` supporting single-byte transfers `0x00` and `0x01`).
2. **Permissions:** Configured as `BLERead | BLEWrite`, allowing central clients to both inspect current LED state and write control bytes.
3. **Event Handler Binding:**
   - `BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler)`
   - `BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler)`
   - `switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten)`
4. **Polling Function (`BLE.poll()`):** Under Arduino Mbed OS, `BLE.poll()` must be invoked in `loop()` to service the radio stack interrupt queue and dispatch pending callback events.

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Code Example for students/AIoT_Arduino/BLE_Callback_Basic/BLE_Callback_Basic.ino`
> 
> "This example creates a BLE peripheral with a service that contains a characteristic to control an LED... Service UUID: 19B10000-E8F2-537E-4F6C-D104768A1214, Characteristic UUID: 7299a27d-8110-4eb7-b67b-ad03cec64c02... Uses event handlers BLEConnected, BLEDisconnected, and BLEWritten to toggle LED."
