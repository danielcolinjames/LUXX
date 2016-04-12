// This code is for Rick and Anas' structure

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <XBee.h>


// ---------------------------------------------------------//
// ---------------   Instantiate variables  ----------------//
// ---------------------------------------------------------//
#define NUMBER_OF_PIXELS 78
#define PINONE 4

int rVal = 255;
int gVal = 255;
int bVal = 255;

int r = 0;
int g = 1;
int b = 2;

uint8_t colours [NUMBER_OF_PIXELS][3];

uint8_t states[] = { 80, 80, 80, 80, 80, 80, 80, 80, 80, 80 };
boolean activeSuits[] = { false, false, false, false, false, false,
  false, false, false, false };
  
uint8_t numberOfActiveSuits = 0;

uint8_t segment;
uint8_t remainder;

long lightMillis = 0;

// ---------------------------------------------------------//
// ---------------------  Packet types  --------------------//
// ---------------------------------------------------------//
uint8_t tagPacket = 79;
uint8_t gameStartPacket = 78;
uint8_t gameOverPacket = 77;


// ---------------------------------------------------------//
// ---------------   Instantiate libraries  ----------------//
// ---------------------------------------------------------//
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBER_OF_PIXELS, 
  PINONE, NEO_GRB + NEO_KHZ800);
  
RFIDuino rfiduino(1.1);

XBee xbee = XBee();


// ---------------------------------------------------------//
// -------------------------  Setup  -----------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  // set all the pixels to black except the first
  for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
    if (i != 0) {
      colours[i][r] = 0;
      colours[i][g] = 0;
      colours[i][b] = 0;
    }
    else {
      colours[i][r] = 255;
      colours[i][g] = 255;
      colours[i][b] = 255;
    }
  }
  pixels.begin();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForMessages();
  stepThroughLights();
  lightUp();
}



