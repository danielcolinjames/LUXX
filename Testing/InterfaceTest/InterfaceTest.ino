#include <SoftwareSerial.h>

/*
 * 
 * 79 - Game state:
 *      Number of active suits,
 *      Number of red suits,
 *      Number of blue suits
 *      
 * 78 - Start game:
 *      Number of active suits
 *      
 * 77 - Suit was tagged:
 *      New colour (0 if only 1 person is it)
 *      
 * 76 - Friendly fire:
 *      Suit colour
 *      
 * 75 - Game over:
 *      Winning colour (or 0 if timeout)
 *      
 */


SoftwareSerial consoleInterface(3, 2); // rx, tx
SoftwareSerial debugSerial(8, 7);

int fromConsole;
uint8_t toConsole;

uint8_t one = 1;
uint8_t two = 2;
uint8_t three = 3;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  
  consoleInterface.begin(9600);
  debugSerial.begin(9600);
  debugSerial.println("Starting debug...");
}


void loop() {
  listenToConsole();
  listenToMax();
}


void listenToConsole() {
  consoleInterface.listen();
  while (consoleInterface.available() > 0) {
    fromConsole = consoleInterface.read();
    debugSerial.println(fromConsole);
    Serial.println(fromConsole);
  }
}


void listenToMax() {
  if (Serial.available()) {
    int reading = Serial.read();
    debugSerial.print("Message from Max: ");
    debugSerial.println(reading);
    consoleInterface.write(reading);
  }
}

