// This code will run on the console and control all the suits

#include <XBee.h>
#include <SoftwareSerial.h>


int gameModeOneButtonPin = 7;
int gameModeOneButtonState = 0;

int gameModeTwoButtonPin = 7;
int gameModeTwoButtonState = 0;

int gameModeThreeButtonPin = 7;
int gameModeThreeButtonState = 0;

int gameOverButtonPin = 7;
int gameOverButtonState = 0;


// ---------------------------------------------------------//
// --------------------  Global arrays ---------------------//
// ---------------------------------------------------------//
uint8_t states[] = { 80, 80, 80, 80, 80,
  80, 80, 80, 80, 80 };

// this is to keep track of which suits are active in each game
// since there's a good chance we won't have 10 people in each game
boolean activeSuits[] = { false, false, false, false, false,
  false, false, false, false, false };

uint16_t addresses[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5,
  0x6, 0x7, 0x8, 0x9 };

uint16_t structureAddress = 0x15;

boolean responseReceived[] = { false, false, false, false, false,
  false, false, false, false, false };


// ---------------------------------------------------------//
// --------------------- Packet types  ---------------------//
// ---------------------------------------------------------//
uint8_t pingByte = 101;

uint8_t confusedByte = 100;
uint8_t taggedByte = 99;

uint8_t gameStartByte = 98;
uint8_t positiveResponseByte = 97;
uint8_t negativeResponseByte = 96;
uint8_t gameOverByte = 95;

uint8_t manualChangeByte = 94;

uint8_t structureTagPacket = 79;
uint8_t structureGameStartPacket = 78;
uint8_t structureGameOverPacket = 77;


// ---------------------------------------------------------//
// -------------------  Global variables  ------------------//
// ---------------------------------------------------------//
uint8_t suitID;
uint8_t taggerID;

uint8_t gameMode;

boolean gameOverBool = false;

uint8_t numberOfActiveSuits;

uint8_t numberOfWarmSuits;
uint8_t numberOfCoolSuits;

uint8_t warmColour;
uint8_t coolColour;

boolean suitReceivedInstruction;
boolean suitReceivedPing;

long stateMillis = 0;
int stateCheckInterval = 1000;

long gameOverMillis = 0;
int fiveMinutes = 300000;

long outputMillis = 0;
int outputInterval = 1000;

long structureMillis = 0;

boolean reset = false;
uint8_t stateReport;

long statePrintMillis = 0;


// ---------------------------------------------------------//
// ---------------------  XBee variables  ------------------//
// ---------------------------------------------------------//
//SoftwareSerial debugSerial (9, 8); //rx, tx
SoftwareSerial interfaceSerial(4, 3); // rx, tx

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
  
  // debugSerial.begin(9600);
  // debugSerial.println("Starting...");
  
  interfaceSerial.begin(9600);
  
  digitalWrite(gameModeOneButtonPin, HIGH);
  
  // necessary for randomization in colour selection
  randomSeed(analogRead(5));
  
  waitForReset();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  checkButtons();
  lookForMessages();
  gameStateCheck();
}




