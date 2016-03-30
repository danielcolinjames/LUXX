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


SoftwareSerial consoleToInterface(5, 4); // to send into TouchDesigner
SoftwareSerial interfaceToConsole(3, 2); // to receive from TouchDesigner

int fromConsole;
uint8_t toConsole;

uint8_t one = 1;
uint8_t two = 2;
uint8_t three = 3;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}
  
  consoleToInterface.begin(9600);
  interfaceToConsole.begin(9600);
}

void loop() {
//  toSendToInterface();
testSend();
//  toSendToConsole();

//  sendValuesToMax();
//  toSendToConsole();
}


void sendValuesToMax() {
  Serial.println(one);
  delay(100);
  Serial.println(two);
  delay(100);
  Serial.println(three);
  delay(100);
}

void testSend() {
  Serial.print(75);
  Serial.println();
  delay(500);

  Serial.print(8);
  Serial.println();
  delay(500);

  Serial.print(3);
  Serial.println();
  delay(500);
}

void toSendToInterface() {
  consoleToInterface.listen();
  while (consoleToInterface.available() > 0) {
    fromConsole = consoleToInterface.read();
    Serial.println(fromConsole);
    delay(1000);
//    fromConsole = consoleToInterface.readString().toInt();
//    Serial.println(fromConsole.toInt());
  }
}


void toSendToConsole() {
//  toConsole = Serial.read();
  if (Serial.available()) {
    int reading = Serial.read();
    if (reading == 9) {
      one = 4;
      two = 5;
      three = 6;
    }
    else {
      one = 75;
      two = 10;
      three = 11;
    }
  }
}

