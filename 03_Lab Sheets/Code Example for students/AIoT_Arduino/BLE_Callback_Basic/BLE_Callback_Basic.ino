#include <ArduinoBLE.h>
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic("7299a27d-8110-4eb7-b67b-ad03cec64c02", BLERead | BLEWrite);

const int ledPin = LEDB; // pin to use for the LED
int myValue = 10;
unsigned long myTime = millis();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(ledPin, OUTPUT); // use the LED pin as an output
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  // set the local name peripheral advertises
  BLE.setLocalName("Meng_Callback");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);
  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  // add service
  BLE.addService(ledService);
  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLERead, switchCharacteristicRead);
  // set an initial value for the characteristic
  switchCharacteristic.setValue(myValue);
  // start advertising
  BLE.advertise();
  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  //switchCharacteristic.writeValue((int16_t) round(t * 100)); // Shift for two decimal places.
  if( millis() - myTime > 5000)
  {
      myValue++;
      myTime = millis();
      Serial.println(myValue);
  }

}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  //Serial.print("Connected event, central: ");
  //Serial.println(central.address());
  // Update Bluetooth characteristics with new values.
  switchCharacteristic.writeValue(myValue); // Shift for two decimal places.
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  //Serial.print("Disconnected event, central: ");
  //Serial.println(central.address());
}

void switchCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.println("Characteristic event, Read: ");
 
}
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  //Serial.print("Characteristic event, written: ");

  if (switchCharacteristic.value()) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}


