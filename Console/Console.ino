#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>


// ---------------------------------------------------------//
// ------------  Game state and logic variables ------------//
// ---------------------------------------------------------//

// 90 for blue, 91 for red, 92 for purple/white

int stateArray[] = { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 };

uint8_t suit_ID;
uint8_t tagger_ID;

// admin messages are sent at the start of a new game
// and they tell each suit which colour it starts as

uint8_t suitAdminID;
uint8_t taggerAdminID;

int gameMode = 0;


// ---------------------------------------------------------//
// ---------------------  XBee variables  ------------------//
// ---------------------------------------------------------//

XBee xbee = XBee();

uint8_t payload[] =  {0};

Tx16Request tx = Tx16Request(0xFFFF, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

Rx16Response rx16 = Rx16Response();

uint8_t startBit = 99;

unsigned char colourChangeInstruction = 0;

int tempSuitState = 0;


// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {

  Serial.begin(9600);
  Serial.println("START GAME");

  delay(10);

  gameMode = 1;
  
  startGame();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForMessages();
}






