// This code is to run from suit 4.

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <SoftwareSerial.h>
#include <XBee.h>

#define NUMBER_OF_CARDS 10

#define PINONE 9
#define PINTWO 10

#define NUMPIXELSONE 8
#define NUMPIXELSTWO 8

uint8_t suitID = 4;

byte keyTag[NUMBER_OF_CARDS][5] = {
  {114, 0, 95, 44, 9},        // Tag 1
  {114, 0, 95, 73, 207},      // Tag 2
  {114, 0, 95, 43, 231},      // Tag 3
  {255, 255, 255, 255, 255},       // Tag 4
  {114, 0, 95, 44, 0},        // Tag 5
  {114, 0, 95, 126, 166},     // Tag 6
  {114, 0, 95, 109, 22},      // Tag 7
  {114, 0, 95, 98, 170},      // Tag 8
  {114, 0, 95, 44, 7},        // Tag 9
  {114, 0, 95, 67, 234}       // Tag 10
};

/*
 * Suit 1: 114, 0, 95, 44, 9
 * Suit 2: 114, 0, 95, 73, 207
 * Suit 3: 114, 0, 95, 43, 231
 * Suit 4: 114, 0, 95, 38, 99
 * Suit 5: 114, 0, 95, 44, 0
 * Suit 6: 114, 0, 95, 126, 166
 * Suit 7: 114, 0, 95, 109, 22
 * Suit 8: 114, 0, 95, 98, 170
 * Suit 9: 114, 0, 95, 44, 7
 * Suit 10: 114, 0, 95, 67, 234
 */


// ---------------------------------------------------------//
// ---------------   Instantiate libraries  ----------------//
// ---------------------------------------------------------//
Adafruit_NeoPixel pixelsOne = Adafruit_NeoPixel(NUMPIXELSONE, 
  PINONE, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixelsTwo = Adafruit_NeoPixel(NUMPIXELSTWO, 
  PINTWO, NEO_GRB + NEO_KHZ800);
  
RFIDuino rfiduino(1.1);

SoftwareSerial debugSerial(12, 11); // (Rx, Tx)

XBee xbee = XBee();


// ---------------------------------------------------------//
// -------------------- Global variables -------------------//
// ---------------------------------------------------------//
uint8_t taggerID = 0;

long prevMillis = 0;
long prevMillisOne = 0;
long prevMillisTwo = 0;

long gameOverMillis = 0;

boolean messageReceived = false;

boolean soundOn = false;
boolean soundOff = true;

uint8_t instruction;

int notes[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
  1100, 1200, 1300, 1400, 1500, 1600 };

// ---------------------------------------------------------//
// --------------------- Packet types  ---------------------//
// ---------------------------------------------------------//
uint8_t taggedByte = 99;
uint8_t gameStartByte = 98;
uint8_t positiveResponseByte = 97;
uint8_t negativeResponseByte = 96;
uint8_t gameOverByte = 95;


// ---------------------------------------------------------//
// -------------------   XBee variables  -------------------//
// ---------------------------------------------------------//
uint16_t address = 0x20;
uint8_t payload[] = { 0, 0, 0 };
uint8_t packetSize = 3;

Tx16Request tx = Tx16Request(address, payload, packetSize);

TxStatusResponse txStatus = TxStatusResponse();
Rx16Response rx16 = Rx16Response();


// ---------------------------------------------------------//
// ------------------    RFID variables   ------------------//
// ---------------------------------------------------------//
byte tagData[5]; // holds the ID numbers from the tag
byte tagDataBuffer[5]; // a buffer for verifying the tag data

int readCount = 0;
boolean tagCheck = false;
boolean verifyKey = false;
int i;


// ---------------------------------------------------------//
// --------------------   LED variables  -------------------//
// ---------------------------------------------------------//
uint8_t rVal = 0;
uint8_t gVal = 0;
uint8_t bVal = 0;

long lightMillis = 0;
long lightMillisOne = 0;
long lightMillisTwo = 0;

int loopCounter = 0;
int loopCounterOne = 0;
int loopCounterTwo = 0;


// ---------------------------------------------------------//
// ----------------------   Setup   ------------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  debugSerial.begin(9600);
  debugSerial.println("Starting debugger from suit...");  
  
  pixelsOne.begin();
  pixelsTwo.begin();
  
  rVal = 255;
  gVal = 255;
  bVal = 255;
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForTags();
  lookForMessages();
  stepThroughLights();
}


// ---------------------------------------------------------//
// ----------------------- Game over -----------------------//
// ---------------------------------------------------------//
void gameOver() {
  boolean gameRestartDetected = false;
  debugSerial.println("----- Game over until 98 received -----");
  
  while (gameRestartDetected == false) {
    gameOverBeep();
    stepThroughLights();
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      debugSerial.print("Packet found.");
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        xbee.getResponse().getRx16Response(rx16);
        
        uint8_t packetType = rx16.getData(0);

        if (packetType == 98) {
          gameRestartDetected = true;
          uint8_t colour = rx16.getData(1);
          
          setColour(colour);
          activateSuit(rVal, gVal, bVal);
        }
      }
    }
  }
}

