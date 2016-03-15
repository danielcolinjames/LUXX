// This code is to run from each suit.
/*
 CHART OF VALUES
 
 99 = start bit for "I've been tagged" message to console
 
 90 = blue
 91 = red

 50 = change colour
 55 = don't change colour

*/

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <SoftwareSerial.h>
#include <XBee.h>

#define PIN 9
#define NUMPIXELS 16
#define NUMBER_OF_CARDS 10

int suitID = 1;

// ---------------------------------------------------------//
// ---------------   Instantiate libraries  ----------------//
// ---------------------------------------------------------//
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 
  PIN, NEO_GRB + NEO_KHZ800);

RFIDuino rfiduino(1.1);

SoftwareSerial debugSerial(12, 11); // (Rx, Tx)

XBee xbee = XBee();

// ---------------------------------------------------------//
// -------------------   XBee variables  -------------------//
// ---------------------------------------------------------//
uint8_t payload[] =  {0, 0, 0};
//
Tx16Request tx = Tx16Request(0x1, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

Rx16Response rx16 = Rx16Response();

uint8_t firstByte;
uint8_t startBit = 99;


int suitAdminID = suitID + 80;

int taggerID = 0;

boolean confirmation = false;

unsigned long prevMillis = millis();


// ---------------------------------------------------------//
// ------------------    RFID variables   ------------------//
// ---------------------------------------------------------//
byte tagData[5]; // holds the ID numbers from the tag
byte tagDataBuffer[5]; // a buffer for verifying the tag data

int loopCounter = 0;

int readCount = 0;
boolean tagCheck = false;
boolean verifyKey = false;
int i;

byte keyTag[NUMBER_OF_CARDS][5] = {
  {0, 0, 0, 0, 0},          //Tag 1 - THIS SUIT
  {114, 0, 95, 73, 207},    //Tag 2
  {114, 0, 95, 43, 231},    //Tag 3
  {114, 0, 95, 38, 99},     //Tag 4
  {114, 0, 95, 44, 0},      //Tag 5
  {114, 0, 95, 126, 166},   //Tag 6
  {114, 0, 95, 109, 22},    //Tag 7
  {114, 0, 95, 98, 170},    //Tag 8
  {114, 0, 95, 44, 7},      //Tag 9
  {114, 0, 95, 67, 234}     //Tag 10
};

/*
 * 
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
// --------------------   LED variables  -------------------//
// ---------------------------------------------------------//
int currentColour = 0;

int rVal = 0;
int gVal = 0;
int bVal = 0;

long lightMillis = 0;

//unsigned char colourChangeInstruction = 0;


// ---------------------------------------------------------//
// ----------------------   Setup   ------------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  debugSerial.begin(9600);
  debugSerial.println("Starting debugger from suit...");  
  
  pixels.begin();
  rVal = 255;
  gVal = 255;
  bVal = 255;
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForAdminMessage();
  lookForTag();
  stepThroughLights();
}



