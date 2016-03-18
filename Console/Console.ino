// This code will run on the console and control all the suits

#include <XBee.h>
#include <SoftwareSerial.h>


// ---------------------------------------------------------//
// --------------------  Global arrays ---------------------//
// ---------------------------------------------------------//
// 89 for blue, 88 for red, 80 for purple/white

uint8_t states[10] = { 80, 80, 80, 80, 80,
  80, 80, 80, 80, 80 };
  
boolean activeSuits[10] = { false, false, false, false, false,
  false, false, false, false, false };
  
uint16_t addresses[10] = { 0x1, 0x2, 0x3, 0x4, 0x5, 
  0x6, 0x7, 0x8, 0x9, 0x10 };


// ---------------------------------------------------------//
// --------------------- Packet types  ---------------------//
// ---------------------------------------------------------//

uint8_t taggedByte = 99;
uint8_t gameStartByte = 98;
uint8_t positiveResponseByte = 97;
uint8_t negativeResponseByte = 96;
uint8_t gameOverByte = 95;


// ---------------------------------------------------------//
// -------------------  Global variables  ------------------//
// ---------------------------------------------------------//
uint8_t suitID;
uint8_t taggerID;

uint8_t gameMode;

boolean suitReceivedInstruction;


// ---------------------------------------------------------//
// ---------------------  XBee variables  ------------------//
// ---------------------------------------------------------//
SoftwareSerial debugSerial (9, 8); //rx, tx

XBee xbee = XBee();

uint16_t address = 0x0;
uint8_t payload[] = {0};
uint8_t packetSize = 0;

Tx16Request tx = Tx16Request(address, payload, packetSize);


// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {
  
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  debugSerial.begin(9600);
  debugSerial.println("Starting...");
  
  delay(10);
  
  gameMode = 1;
  
  startGame();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForMessages();
  gameStateCheck();
  sendToTouch();
}






