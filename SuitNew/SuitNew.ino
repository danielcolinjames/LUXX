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

#include <SoftwareSerial.h>
#include <RFIDuino.h>

// ---------------------------------------------------------//
// -------------------   Global variables  -----------------//
// ---------------------------------------------------------//

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

int lightColour = 0;

//unsigned char colourChangeInstruction = 0;

int waitingToBeAddressed = 0;
int readyToReceive = 0;
int receivingInstruction = 0;

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
}


// ---------------------------------------------------------//
// -----------------------   Loop   ------------------------//
// ---------------------------------------------------------//
void loop() {
  lookForAdminMessage();
  lookForTag();
}


// ---------------------------------------------------------//
// ----  Look for admin messages addressed to this suit ----//
// ---------------------------------------------------------//
void lookForAdminMessage() {
  if (Serial.read() == (unsigned char)suitAdminID) {
    // there is an admin message for this suit
    int colourSetInstruction = Serial.read();
    
    initializeSuitColour(colourSetInstruction);
    
    // confirm that instruction was received
    Serial.write((unsigned char)suitAdminID);
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

  digitalWrite(rfiduino.led1, LOW);
  digitalWrite(rfiduino.led2, LOW);

  if (colour == 90) {
    digitalWrite(rfiduino.led2, HIGH);
  }
  else if (colour == 91) {
    digitalWrite(rfiduino.led1, HIGH);
  }
}
  
  
// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void changeSuitColour(unsigned instruction) {
  rfiduino.errorSound();
  debug(3, 2000);

  if (instruction == 50) {
    // TODO: change colour
    rfiduino.successSound();
  }
  else if (instruction == 55) {
    rfiduino.errorSound();
  }
}


// ---------------------------------------------------------//
// ---------  Wait for a colour change instruction  --------//
// ---------------------------------------------------------//
void awaitInstruction() {
  
  waitingToBeAddressed = 0;
  readyToReceive = 0;
  receivingInstruction = 0;
  
  // wait until this suit is addressed
  while (waitingToBeAddressed == 0) {
    
    // keep checking to see what's being received
    unsigned char addressID = Serial.read();
    
    // this suit has been addressed
    if (addressID == ((unsigned char)suit_ID)) {
      
      // wait until the console knows this suit is
      // ready to receive a message
      while (readyToReceive == 0) {
        
        // keep writing the suitReadyID until it receives
        // confirmation that the next number is an instruction
        Serial.write((unsigned char)suitReadyID);
        
        // keep checking to see what's being received
        unsigned char readyID = Serial.read();
        
        // the console is saying the next number will be an instruction
        if (readyID == (unsigned char)suitReadyID) {
          
          // wait until an instruction has been received
          while (receivingInstruction == 0) {
          
            // keep checking to see what's being received
            unsigned char colourChangeInstruction = Serial.read();
            
            // an instruction has been received
            if (colourChangeInstruction == (unsigned char)50 || colourChangeInstruction == (unsigned char) 55) {
              
              // pass the instruction on to the suit's LEDs
              changeSuitColour(colourChangeInstruction);
              
              // tell the console it can stop sending instruction
              Serial.write((unsigned char)suitConfirmationID);
              
              // exit both while loops
              readyToReceive = 1;
              receivingInstruction = 1;
            }
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
