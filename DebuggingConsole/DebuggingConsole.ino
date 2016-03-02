#include <SoftwareSerial.h>

SoftwareSerial suitDebug(3, 2); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  
  suitDebug.begin(9600);
}

void loop() { // run over and over
  suitDebug.listen();
  while (suitDebug.available() > 0) {
    Serial.println(suitDebug.readString());
  }
}
