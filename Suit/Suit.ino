// This code is to run from each suit.

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <SoftwareSerial.h>
#include <XBee.h>

#define NUMBER_OF_CARDS 10

#define PINONE 9
#define PINTWO 10

#define NUMPIXELSONE 9
#define NUMPIXELSTWO 9

uint8_t suitID = 1;

byte keyTag[NUMBER_OF_CARDS][5] = {
  {114, 0, 95, 44, 9},        // Tag 0
  {114, 0, 95, 73, 207},      // Tag 1
  {114, 0, 95, 43, 231},      // Tag 2
  {114, 0, 95, 44, 8},        // Tag 3
  {114, 0, 95, 44, 0},        // Tag 4
  {114, 0, 95, 126, 166},     // Tag 5
  {114, 0, 95, 109, 22},      // Tag 6
  {114, 0, 95, 98, 170},      // Tag 7
  {114, 0, 95, 43, 255},        // Tag 8
  {114, 0, 95, 81, 79}       // Tag 9
};

byte blocked = 255;

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

long waitingForStartMillis = 0;

boolean messageReceived = false;
boolean pingReceived = false;

boolean soundOn = false;
boolean soundOff = true;

uint8_t instruction;

int notes[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
  1100, 1200, 1300, 1400, 1500, 1600 };


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

  keyTag[suitID][0] = blocked;
  keyTag[suitID][1] = blocked;
  keyTag[suitID][2] = blocked;
  keyTag[suitID][3] = blocked;
  keyTag[suitID][4] = blocked;
  
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  debugSerial.begin(9600);
  debugSerial.println("Starting debugger from suit...");  
  
  pixelsOne.begin();
  pixelsTwo.begin();
  
  pixelsOne.setBrightness(255);
  pixelsTwo.setBrightness(255);
  
  gameOver();
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForTags();
  lookForMessages();
  stepThroughLights();
}


