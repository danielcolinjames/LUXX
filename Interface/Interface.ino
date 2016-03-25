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


SoftwareSerial consoleToTouch(5, 4); // to send into TouchDesigner
SoftwareSerial touchToConsole(3, 2); // to receive from TouchDesigner

uint8_t fromConsole;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}

  consoleToTouch.begin(9600);
  touchToConsole.begin(9600);
}

void loop() {
  
  consoleToTouch.listen();
  while (consoleToTouch.available() > 0) {
    fromConsole = (uint8_t)consoleToTouch.readString().toInt();
    Serial.print((uint8_t)fromConsole);
  }
}

