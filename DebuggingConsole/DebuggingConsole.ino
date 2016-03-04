#include <SoftwareSerial.h>

SoftwareSerial consoleDebug(3, 2); // rx, tx

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}

  consoleDebug.begin(9600);
  Serial.println("Starting console debugger locally...");
}

void loop() {
  
  consoleDebug.listen();
  while (consoleDebug.available() > 0) {
    Serial.println(consoleDebug.readString());
  }
}

