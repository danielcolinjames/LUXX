// This code is to run from each suit.
/*
 CHART OF VALUES
 
 25 = start bit for SUIT TAGGED message to console
 45 = error
 50 = change colour
 55 = don't change colour
 suit_ID + 10 = confirmation that [suit_ID] has changed colour
 
 */
#include <SoftwareSerial.h>
#include <RFIDuino.h>

boolean debugging = false;

// ---------------------------------------------------------//
// -------------------   Global variables  -----------------//
// ---------------------------------------------------------//

//SoftwareSerial rfid(6, 8);
//SoftwareSerial xbee(0, 1); // RX, TX

RFIDuino rfiduino(1.1);


#define NUMBER_OF_CARDS 4


byte tagData[5]; // holds the ID numbers from the tag
byte tagDataBuffer[5]; // a buffer for verifying the tag data


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


int colourChangeInstruction = 0;

int instructionsHaveBeenReceived = 0;

int suit_ID = 1;
int suitConfirmationID = suit_ID + 10;

int tagger_ID = 0;


// ---------------------------------------------------------//
// ----------------------   Setup   ------------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
//  isThereAMessageAddressedToThisSuit();
  lookForTag();
}


// ---------------------------------------------------------//
// ---  Look for general messages addressed to this suit  --//
// ---------------------------------------------------------//
void isThereAMessageAddressedToThisSuit() {
  if (Serial.read() == suit_ID) {
    // there is a message for this suit
    colourChangeInstruction = Serial.read();
    
    if (debugging) Serial.print("General message received: ");
    if (debugging) Serial.println(colourChangeInstruction);

    if (colourChangeInstruction == 1) {
      // confirm that the colour was changed
      changeSuitColour(colourChangeInstruction);
      Serial.write((unsigned char)suitConfirmationID);
    }
  }
}


// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTag() {
  isThereATag();

  if (readCount > 1) {
    // printCurrentTag();
    whoTaggedMe();
    if (debugging) Serial.println("TAG FOUND");
  }
}


// ---------------------------------------------------------//
// ------  Send suit_ID and tagger_ID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee() {

  Serial.write(25);
  Serial.write((unsigned char)suit_ID);
  Serial.write((unsigned char)tagger_ID);
}


// ---------------------------------------------------------//
// -----  Figure out if there is a tag on the reader  ------//
// ---------------------------------------------------------//
void isThereATag() {

  tagCheck = rfiduino.decodeTag(tagData);

  if (tagCheck) {
    readCount++;
  }
  else {
    readCount = 0;
  }
}


// ---------------------------------------------------------//
// --------  Print out the current tag's ID number  --------//
// ---------------------------------------------------------//
void printCurrentTag() {
  
  if (debugging) Serial.print("RFID Tag ID:");
  
  for (int n = 0; n < 5; n++) {
    if (debugging) Serial.print(tagData[n], DEC);
    if (n < 4) {
      if (debugging) Serial.print(", ");
    }
  }
  if (debugging) Serial.print("\n\r");
}


// ---------------------------------------------------------//
// --------  Figure out which suit tagged this one  --------//
// ---------------------------------------------------------//
void whoTaggedMe() {
  
  verifyKey = false;
    
  i = 0;
    
  while (i < NUMBER_OF_CARDS && verifyKey == false) {
    
    verifyKey = rfiduino.compareTagData(tagData, keyTag[i]);
    
    if (verifyKey == true) {
      if (debugging) Serial.print("Tagger = ");
      if (debugging) Serial.println(i + 1);
      
      tagger_ID = i + 1;
      sendToXBee();
      awaitInstruction();

      verifyKey = true;
    }
    i++;
  }
}


// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void changeSuitColour(int instruction) {

  // change LEDs to the other colour.
  // if red, make them blue
  // if blue, make them red.
  if (debugging) Serial.print("Colour change: ");
  if (debugging) Serial.println(instruction);

  // TODO: add light change code here
}


// ---------------------------------------------------------//
// ---------  Wait for a colour change instruction  --------//
// ---------------------------------------------------------//
void awaitInstruction() {
  
  instructionsHaveBeenReceived = 0;
  
  while (instructionsHaveBeenReceived == 0) {

    if (debugging) Serial.print("Awaiting instructions... xbee.read() = ");
    
    unsigned char incoming = Serial.read();


    if (debugging) Serial.println(incoming);
  
    if (incoming == ((unsigned char)suit_ID)) {
      
      colourChangeInstruction = Serial.read();
      
      if (debugging) Serial.print("Message received: ");
      if (debugging) Serial.println(colourChangeInstruction);
      
      if (colourChangeInstruction == 50) {
        changeSuitColour(colourChangeInstruction);
      }
      
      // send confirmation to console that colour has changed
      Serial.write((unsigned char)suitConfirmationID);

      instructionsHaveBeenReceived = 1;
      if (debugging) Serial.println("Exiting while loop");
    }
  }
}
