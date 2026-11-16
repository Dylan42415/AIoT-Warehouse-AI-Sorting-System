# Lab 16: Bluetooth Communication

This document provides the exhaustive laboratory execution protocol for **Lab 16: Bluetooth Communication** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the [[Protocol-Bluetooth-Low-Energy-GATT|BLE GATT architecture]] on the nRF52840 SoC.
2. Configure the [[Arduino-Nano-33-BLE-Sense|Arduino Nano 33 BLE Sense]] as a BLE Peripheral (GATT Server).
3. Advertise a custom Service UUID and Characteristic UUID with Read and Write permissions.
4. Connect from an external BLE Central (smartphone with **nRF Connect** or SBC running **Bleak**) to control the onboard LED wirelessly.

---

## 2. Laboratory Sketch Code

```cpp
#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic switchCharacteristic("7299a27d-8110-4eb7-b67b-ad03cec64c02", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Nano33BLE_LED");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  switchCharacteristic.writeValue(0); // Default off

  BLE.advertise();
  Serial.println("BLE Peripheral active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {
          Serial.println("LED turned ON");
          digitalWrite(ledPin, HIGH);
        } else {
          Serial.println("LED turned OFF");
          digitalWrite(ledPin, LOW);
        }
      }
    }
    Serial.println("Central disconnected.");
  }
}
```

Detailed in [[Code-Arduino-BLE-Callback-Basic|Code Arduino BLE Callback Basic]].

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab16_Bluetooth_Communication.pdf` (Pages 1-4)
> 
> "Configure the Arduino Nano 33 BLE Sense as a BLE peripheral... Advertise custom Service UUID: 19B10000-E8F2-537E-4F6C-D104768A1214 and Characteristic UUID: 7299a27d-8110-4eb7-b67b-ad03cec64c02... Central devices connect and write 0x01 to turn LED on or 0x00 to turn LED off."
