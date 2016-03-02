#include <RFIDuino.h>

#include <Printers.h>
#include <XBee.h>

#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial mySerial (rxPin, txPin);

//Sender Code

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
}

void loop() {

    mySerial.print("sent");
    delay(1000);
  
}
