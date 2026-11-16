// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;  // the number of the pushbutton pin
// variables will change:
uint btnCount = 0;
bool onceFlag = false;
UART mySerial(11, NC,NC,NC);

void setup() {
    delay(200);
    Serial.begin(9600);
    while (!Serial);
    pinMode(LEDR, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT_PULLUP);
    // Note: The RGB LEDs on the Arduino Nano 33 BLE
    // Sense are on when the pin is LOW, off when HIGH.
    digitalWrite(LEDR, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);
    attachInterrupt(digitalPinToInterrupt(buttonPin), button_interrupt, FALLING); //RISING, FALLING, Change and LOW
    onceFlag = false;

    mySerial.begin(9600);
}
void loop() {
    if(onceFlag){
        Serial.println(btnCount);
        mySerial.println(btnCount);
        onceFlag = false;
    }
}
void button_interrupt() {   
    onceFlag = true;    
    btnCount++;
}



