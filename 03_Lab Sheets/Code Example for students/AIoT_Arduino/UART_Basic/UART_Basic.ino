
UART mySerial(A6, A7, NC, NC);   // A6 (Nano TX), A7 (Nao RX)
//String testData = "123";
int mmm=123;
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.println("123");
  Serial.println("123");
  delay(3000);
}
