#include <SoftwareSerial.h>

SoftwareSerial consoleInterface(3, 2); // rx, tx
//SoftwareSerial debugSerial(8, 7);

int fromConsole;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  
  consoleInterface.begin(9600);
//  debugSerial.begin(9600);
//  debugSerial.println("Starting debugger from interface...");
}


void loop() {
  listenToConsole();
}


void listenToConsole() {
  consoleInterface.listen();
  while (consoleInterface.available() > 0) {
    fromConsole = consoleInterface.read();
//    debugSerial.print("Message from console: ");
//    debugSerial.println(fromConsole);
    Serial.println(fromConsole);
  }
}
