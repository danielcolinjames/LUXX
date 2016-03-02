#include <SoftwareSerial.h>

String debugMessage;
const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial mySerial(rxPin, txPin); //rx, tx

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.listen();
  
  if (mySerial.available() > 0) {

    debugMessage = mySerial.readString();
    Serial.println(debugMessage);
  }
}
