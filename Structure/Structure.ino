// This code is for Rick and Anas' structure

#include <Adafruit_NeoPixel.h>
#include <RFIDuino.h>
#include <XBee.h>

#define NUMBER_OF_PIXELS 75
#define PINONE 4

states[] = { 80, 80, 80, 80, 80, 80, 80, 80, 80, 80 };


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
  
  pixels.begin();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  
}


