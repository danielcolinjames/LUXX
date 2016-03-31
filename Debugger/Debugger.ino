#include <SoftwareSerial.h>

SoftwareSerial debugger(3, 2); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}

  debugger.begin(9600);
  Serial.println("Starting debugger locally...");
}

void loop() {
  debugger.listen();
  while (debugger.available() > 0) {
    Serial.println(debugger.readString());
  }
}

