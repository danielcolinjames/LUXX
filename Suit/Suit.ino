// This code runs on the suit and visualizes its wearer's heartbeat
// by constantly adjusting the brightness of the LEDs to match

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <SoftwareSerial.h>
#include <XBee.h>

#define PINONE 9
#define PINTWO 10

#define NUMPIXELSONE 9
#define NUMPIXELSTWO 9

uint8_t suitID = 9;


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
Rx16Response rx16 = Rx16Response();

uint8_t instruction;

int notes[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
  1100, 1200, 1300, 1400, 1500, 1600 };

int intervalR;
int intervalG;
int intervalB;


// ---------------------------------------------------------//
// --------------------   LED variables  -------------------//
// ---------------------------------------------------------//
uint8_t rValue = 255;
uint8_t gValue = 0;
uint8_t bValue = 0;

long lightMillis = 0;
long lightMillisOne = 0;
long lightMillisTwo = 0;

int loopCounter = 0;
int loopCounterOne = 0;
int loopCounterTwo = 0;

uint8_t incomingBrightness = 255;

uint8_t incomingR = 0;
uint8_t incomingG = 0;
uint8_t incomingB = 0;

int incomingNote = 0;


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
  
  pixelsOne.setBrightness(255);
  pixelsTwo.setBrightness(255);
  
  changeColour();
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForMessages();
  adjustBrightness();
  //stepThroughLights();
}


