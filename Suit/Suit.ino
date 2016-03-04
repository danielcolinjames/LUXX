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
#define NUMBER_OF_CARDS 4

// ---------------------------------------------------------//
// ---------------   Instantiate libraries  ----------------//
// ---------------------------------------------------------//
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

RFIDuino rfiduino(1.1);

SoftwareSerial debugSerial(12, 11); // (Rx, Tx)



XBee xbee = XBee();


// ---------------------------------------------------------//
// -------------------   XBee variables  -------------------//
// ---------------------------------------------------------//
//uint8_t payload[] =  {0};
//
//Tx16Request txPacket = Tx16Request(0x1, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

Rx16Response rx16 = Rx16Response();

uint8_t firstByte;
uint8_t startBit = 99;


int suitID = 5;

int suitAdminID = suitID + 80;

int taggerID = 0;

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
  {62, 0, 183, 134, 238},   //Tag 1
  {69, 0, 247, 211, 210},   //Tag 2
  {71, 0, 48, 85, 67},      //Tag 3
  {69, 0, 124, 57, 143}     //Tag 4
};


// ---------------------------------------------------------//
// --------------------   LED variables  -------------------//
// ---------------------------------------------------------//
int currentColour = 0;

int rVal = 0;
int gVal = 0;
int bVal = 0;

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



