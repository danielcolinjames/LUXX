// This code will receive the sensor values from Max and send them to the suit(s)

#include <XBee.h>
#include <SoftwareSerial.h>

// ---------------------------------------------------------//
// ------------------  Global variables --------------------//
// ---------------------------------------------------------//
// raw number
uint8_t incomingValue = 0;

byte incomingReading;
byte incomingInterval;

// final numbers for the suit
uint8_t outgoingBrightness = 0;

uint8_t outgoingR = 255;
uint8_t outgoingG = 255;
uint8_t outgoingB = 255;

int outgoingNote = 0;

uint8_t suitID = 0;

boolean reset = false;


// ---------------------------------------------------------//
// ---------------------  XBee variables  ------------------//
// ---------------------------------------------------------//
//SoftwareSerial debugSerial (13, 12); //rx, tx
//SoftwareSerial interfaceSerial(13, 12); // rx, tx

SoftwareSerial xbeeSerial(3, 2);
  
XBee xbee = XBee();

uint16_t address = 0x9;
uint8_t payload[] = {0};
uint8_t packetSize = 0;

Tx16Request tx = Tx16Request(address, payload, packetSize);


// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {
  
  Serial.begin(9600);

  // SoftwareSerial so that we can send information 
  // through the hardware serial from Max
  xbee.setSerial(xbeeSerial);
  
  // debugSerial.begin(9600);
  // debugSerial.println("Starting...");
  
  xbeeSerial.begin(9600);
  //interfaceSerial.begin(9600);
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
  listenForSensorData();
  mapValues();
  sendToSuit();
}
