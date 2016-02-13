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

//unsigned char colourChangeInstruction = 0;

boolean waitingToBeAddressed = true;
boolean readyToReceive = true;
boolean receivingInstruction = true;

int suit_ID = 5;

int suitAdminID = suit_ID + 80;

int suitReadyID = suit_ID + 10;

int suitConfirmationID = suit_ID + 20;

int tagger_ID = 0;


// ---------------------------------------------------------//
// ----------------------   Setup   ------------------------//
// ---------------------------------------------------------//
void setup() {
  Serial.begin(9600);
  pixels.begin();
  setSuitColour(255, 255, 255);
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForAdminMessage();
  lookForTag();
//  oneByOne(255, 255, 255);
}


void oneByOne(int r, int g, int b) {
   for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    
    if (i > 1) {
      pixels.setPixelColor(i - 1, pixels.Color(0, 0, 0));
    }
    delay(25);
    
    pixels.show();
  }
}


// ---------------------------------------------------------//
// ----  Look for admin messages addressed to this suit ----//
// ---------------------------------------------------------//
void lookForAdminMessage() {
  
  boolean confirmationReceived = false;
  
  waitingToBeAddressed = true;
  receivingInstruction = true;

  
  if (Serial.read() == (unsigned char)suitAdminID) {
    
    while (receivingInstruction == true) {
      Serial.write((unsigned char)suitReadyID);
      
      unsigned char instruction = Serial.read();
      
      if (instruction == (unsigned char)90 || instruction == (unsigned char)91) {
        receivingInstruction = false;
        
        while (confirmationReceived == false) {
          Serial.write((unsigned char)suitConfirmationID);

          if (Serial.read() == (unsigned char)77) {
            confirmationReceived = true;

          }
        }
        
        // instruction received.        
        initializeSuitColour(instruction);
      }
    }
  }
}



// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTag() {
  isThereATag();

  if (readCount > 1) {
    whoTaggedMe();
  }
}


// ---------------------------------------------------------//
// ------  Send suit_ID and tagger_ID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee() {

  Serial.write(99);
  Serial.write((unsigned char)suit_ID);
  Serial.write((unsigned char)tagger_ID);
}


// ---------------------------------------------------------//
// -----------  Light up the LEDs on the suit  -------------//
// ---------------------------------------------------------//
void setSuitColour(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    
//    if (i > 1) {
//      pixels.setPixelColor(i - 1, pixels.Color(0, 0, 0));
//    }
    
    delay(100);

    pixels.show();
  }

  debug(25, 75);
//  rfiduino.successSound();

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
  
//  if (debugging) Serial.print("RFID Tag ID:");
  
  for (int n = 0; n < 5; n++) {
//    if (debugging) Serial.print(tagData[n], DEC);
    if (n < 4) {
//      if (debugging) Serial.print(", ");
    }
  }
//  if (debugging) Serial.print("\n\r");
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
      
      tagger_ID = i + 1;
      sendToXBee();
      awaitInstruction();

      verifyKey = true;
    }
    i++;
  }
}


// ---------------------------------------------------------//
// ---------  Initialize suit colour at game start  --------//
// ---------------------------------------------------------//
void initializeSuitColour(int colour) {
  
  // 90 = blue
  // 91 = red

  if (colour == 90) {
    currentColour = 90;
    setSuitColour(0, 0, 255);
  }
  else if (colour == 91) {
    currentColour = 91;
    setSuitColour(255, 0, 0);
  }
}
  
  
// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void changeSuitColour() {
  
  if (currentColour == 90) {
    setSuitColour(255, 0, 0);
    currentColour = 91;
  }
  else if (currentColour == 91) {
    setSuitColour(0, 0, 255);
    currentColour = 90;
  }
}


// ---------------------------------------------------------//
// ---------  Wait for a colour change instruction  --------//
// ---------------------------------------------------------//
void awaitInstruction() {
  
  waitingToBeAddressed = true;
  receivingInstruction = true;
  
  while (waitingToBeAddressed == true) {
    
    if (Serial.read() == suit_ID) {
      waitingToBeAddressed = false;

      while (receivingInstruction == true) {
        Serial.write(suitReadyID);

        unsigned char instruction = Serial.read();
        if (instruction == 50 || instruction == 55) {
          receivingInstruction = false;

          // instruction received.
          Serial.write(suitConfirmationID);
          if (instruction == 50) {
            changeSuitColour();
          }
        }
      }
    }
  }
}



// ---------------------------------------------------------//
// ---------  For debugging with the RFIDuino LEDs  --------//
// ---------------------------------------------------------//
void debug(int flickers, int gap) {
  for (int i = 0; i < flickers; i++) {  
    digitalWrite(rfiduino.led1,HIGH);
    delay(gap);
    digitalWrite(rfiduino.led1,LOW);
    delay(gap);
  }
}
