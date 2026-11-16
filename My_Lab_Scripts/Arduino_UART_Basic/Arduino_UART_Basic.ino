/*
  Lab 07 / 08 - Arduino Transmitter to Raspberry Pi
  Transmits data via both USB Serial and Hardware UART pins (A6/A7)
*/

// Define Hardware UART on Pins A6 (TX) and A7 (RX)
UART mySerial(A6, A7, NC, NC);

int sensorValue = 123;

void setup() {
  // Initialize USB Serial (for laptop monitoring)
  Serial.begin(9600);
  
  // Initialize Hardware UART (for direct Raspberry Pi GPIO connection)
  mySerial.begin(9600);
  
  while (!Serial && millis() < 3000); // Wait up to 3s for Serial Monitor
  Serial.println("Arduino UART Transmitter Initialized.");
}

void loop() {
  // 1. Send data to Raspberry Pi via Hardware UART pins (A6/A7)
  mySerial.println(sensorValue);
  
  // 2. Send data to Laptop/Pi via USB cable
  Serial.println(sensorValue);
  
  // Delay 3 seconds between transmissions
  delay(3000);
}
