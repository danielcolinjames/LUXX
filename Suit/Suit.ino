// This code is to run from each suit.
/*
 CHART OF VALUES
 
 99 = start bit for "I've been tagged" message to console
 
 90 = blue
 91 = red

 50 = change colour
 55 = don't change colour
 
 suit_ID + 10 (11 to 20) = confirmation that instruction was received
 suit_ID + 20 (21 to 30) = admin messages addressed to this suit
 
*/
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <RFIDuino.h>

#define PIN 9
#define NUMPIXELS 16

// ---------------------------------------------------------//
// -------------------   Global variables  -----------------//
// ---------------------------------------------------------//
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

RFIDuino rfiduino(1.1);

#define NUMBER_OF_CARDS 4


byte tagData[5]; // holds the ID numbers from the tag
byte tagDataBuffer[5]; // a buffer for verifying the tag data

int loopCounter = 0;

int readCount = 0;
boolean tagCheck = false;
boolean verifyKey = false;
int i;

byte keyTag[NUMBER_OF_CARDS][5] ={
  {62, 0, 183, 134, 238},   //Tag 1
  {69, 0, 247, 211, 210},   //Tag 2
  {71, 0, 48, 85, 67},      //Tag 3
  {69, 0, 124, 57, 143},    //Tag 4
};

int currentColour = 0;

int rVal = 0;
int gVal = 0;
int bVal = 0;

//unsigned char colourChangeInstruction = 0;

boolean waitingToBeAddressed = true;
boolean readyToReceive = true;
boolean receivingInstruction = true;

int suit_ID = 5;

int suitAdminID = suit_ID + 80;

int suitReadyID = suit_ID + 10;

int suitConfirmationID = suit_ID + 20;

int tagger_ID = 0;

unsigned long prevMillis = millis();

unsigned long timeoutMillis = millis();

// ---------------------------------------------------------//
// ----------------------   Setup   ------------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
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
















