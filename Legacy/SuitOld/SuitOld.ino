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


// ---------------------------------------------------------//
// -------------------   Global variables  -----------------//
// ---------------------------------------------------------//
SoftwareSerial rfid(7, 8);
SoftwareSerial xbee(10, 9);

int currentTag[11];

int tagFoundFlag = 0;

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

  xbee.begin(9600);
  rfid.begin(19200);

  halt();
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
  if (xbee.read() == suit_ID) {
    // there is a message for this suit
    colourChangeInstruction = xbee.read();
    
    Serial.print("General message received: ");
    Serial.println(colourChangeInstruction);

    if (colourChangeInstruction == 1) {
      // confirm that the colour was changed
      changeSuitColour(colourChangeInstruction);
      xbee.write(suitConfirmationID);
    }
  }
}


// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTag() {
  
  // tell the RFID reader to look for RFID tags
  rfid.listen();
  seek();
  
  delay(10);
  
  // write the RFID value into the currentTag[] array
  parse();
    
  // set tagFoundFlag
  isThereATag();
    
  if (tagFoundFlag == 1) {

    // print out RFID tag values
    printTagNumber();
  
    // figure out which suit the current tag belongs to
    whoTaggedMe();
  
    delay(10);
  }
}


// ---------------------------------------------------------//
// -------- Tells the RFID reader to look for a tag --------//
// ---------------------------------------------------------//
void seek() {

  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)130);
  rfid.write((uint8_t)131);
}


// ---------------------------------------------------------//
// ----- Tells the RFID reader NOT to look for a tag -------//
// ---------------------------------------------------------//
void halt() {

  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)147);
  rfid.write((uint8_t)148);
}


// ---------------------------------------------------------//
// ----  Put the RFID tag number's digits into an array ----//
// ---------------------------------------------------------//
void parse() {

  while (rfid.available()) {
    if (rfid.read() == 255) {
      for (int i = 1; i < 11; i++) {
        currentTag[i] = rfid.read();
      }
    }
  }
}


// ---------------------------------------------------------//
// ---  Says whether or not there is a tag on the reader ---//
// ---------------------------------------------------------//
void isThereATag() {

  // tagFoundFlag increments when true instead of using a boolean value so that when a tag
  // is held down on the RFID reader, it is only read once, until the tag is removed.

  if (currentTag[2] == 6) {
    tagFoundFlag++;
  }
  else {
    tagFoundFlag = 0;
  }
}


// ---------------------------------------------------------//
// -------  Prints out the tag number (for debugging) ------//
// ---------------------------------------------------------//
void printTagNumber() {

  if (tagFoundFlag == 1) {
    /*
    Serial.println((unsigned char)currentTag[5]);
    Serial.println((unsigned char)currentTag[7]);
    Serial.println((unsigned char)currentTag[8]);
    Serial.println((unsigned char)currentTag[9]);
    */
  }
}


// ---------------------------------------------------------//
// ------  Send suit_ID and tagger_ID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee() {

  xbee.write(25);
  Serial.print("Start: ");
  Serial.println(25);

  xbee.write((unsigned char) suit_ID);
  Serial.print("sID = ");
  Serial.println(suit_ID);

  xbee.write((unsigned char) tagger_ID);
  Serial.print("tID = ");
  Serial.println(tagger_ID);
}


// ---------------------------------------------------------//
// --------  Figure out which suit tagged this one  --------//
// ---------------------------------------------------------//
void whoTaggedMe() {

  if (((unsigned char)currentTag[5] == 18) &&
    ((unsigned char)currentTag[7] == 175) &&
    ((unsigned char)currentTag[8] == 202) &&
    ((unsigned char)currentTag[9] == 239)) {

    tagger_ID = 2;
    sendToXBee();
    awaitInstruction();
  }

  else if (((unsigned char)currentTag[5] == 65) &&
    ((unsigned char)currentTag[7] == 13) &&
    ((unsigned char)currentTag[8] == 11) &&
    ((unsigned char)currentTag[9] == 189)) {

    tagger_ID = 3;
    sendToXBee();
    awaitInstruction();
  }
  
  // TODO: else if for suits 4 - 9

}


// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void changeSuitColour(int instruction) {

  // change LEDs to the other colour.
  // if red, make them blue
  // if blue, make them red.
  Serial.print("Colour change: ");
  Serial.println(instruction);

  // TODO: add light change code here
}


// ---------------------------------------------------------//
// ---------  Wait for a colour change instruction  --------//
// ---------------------------------------------------------//
void awaitInstruction() {
  
  instructionsHaveBeenReceived = 0;

  xbee.listen();
  
  while (instructionsHaveBeenReceived == 0) {
    
    Serial.print("Awaiting instructions... xbee.read() = ");
    
    unsigned char incoming = xbee.read();
    
    Serial.println(incoming);
  
    if (incoming == ((unsigned char)suit_ID)) {
      
      colourChangeInstruction = xbee.read();
      
      Serial.print("Message received: ");
      Serial.println(colourChangeInstruction);
      
      if (colourChangeInstruction == 1) {
        changeSuitColour(colourChangeInstruction);
      }
      
      // send confirmation to console that colour has changed
      xbee.write(suitConfirmationID);

      instructionsHaveBeenReceived = 1;
      Serial.println("Exiting while loop");
    }
  }
}
