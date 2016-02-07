#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // (RX, TX)

// 90 for blue, 91 for red, 92 for purple/white
int stateArray[] = { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 };

unsigned char suit_ID = 0;
unsigned char tagger_ID = 0;

int suitConfirmationID = suit_ID + 10;
int taggerConfirmationID = tagger_ID + 10;

int suitAdminID = suit_ID + 20;
int taggerAdminID = tagger_ID + 20;

boolean allConfirmationsReceived = false;

boolean suitConfirmed = false;
boolean taggerConfirmed = false;

int confirmationReceived = 0;

// 1 = viral tag
int gameMode = 1;

unsigned char colourChangeInstruction = 0;


// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {

  Serial.begin(9600);
  Serial.println("START");

  xbee.begin(9600);
  delay(10);

  gameMode = 1;

  startGame();
}


// ---------------------------------------------------------//
// -------------------------  Loop  ------------------------//
// ---------------------------------------------------------//
void loop() {
 
  unsigned char incoming = xbee.read();
  boolean startNumberFound = false;
  
// Serial.println((unsigned char)incoming);
  
  if (incoming == 99) {
    startNumberFound = true;
    Serial.println();
    Serial.println("----------------------- MESSAGE RECEIVED ---------------------");
  }
  else {
    startNumberFound = false;
//    Serial.println("Searching for message...");
  }

  if (xbee.available() > 0 && startNumberFound) {

    suit_ID = xbee.read();
    Serial.print("SUIT:     >  ");
    Serial.print(suit_ID);

    tagger_ID = xbee.read();
    Serial.print("  <     WAS TAGGED BY SUIT:     >  ");
    Serial.print(tagger_ID);
    Serial.println("  <");
    
    // determine instructions
    shouldSuitChangeColour();
    
    // send instructions and then await response
    awaitConfirmation();
    
    // tell us what colour each suit is now
    printOutStates();
  }
}


// ---------------------------------------------------------//
//  Determines whether or not the suit should change colour //
// ---------------------------------------------------------//
void shouldSuitChangeColour() {
  if (stateArray[suit_ID - 1] != stateArray[tagger_ID - 1]) {
    colourChangeInstruction = 50;
  }
  else {
    colourChangeInstruction = 55;
  }
}


// ---------------------------------------------------------//
// ----- Tell the suit whether or not to change colour -----//
// ---------------------------------------------------------//
void sendInstructions(int recepient, unsigned char message) {

  // suit_ID is currently equal to the suit tagged so
  // we need to address it in our instructions message
  xbee.write((unsigned char)recepient);
//  Serial.print("Sending to: ");
//  Serial.println(recepient);
  
  xbee.write((unsigned char)message);
//  Serial.print("Message sent: ");
//  Serial.println(message);
}


// ---------------------------------------------------------//
// ---  Wait for confirmation that suit received message ---//
// ---------------------------------------------------------//
void awaitConfirmation() {
  
  // we have determined whether or not the suits are the same colour
  // and we have sent instructions to one or both suits telling them
  // whether or not they have to change colour.

  // now we have to wait for their confirmation codes so that we can
  // update the state array, knowing that they have changed colours.

  // confirmationIDs are another block of numbers so there are no 
  // repeated numbers being transmitted between XBees.
  suitConfirmationID = suit_ID + 10;
  taggerConfirmationID = tagger_ID + 10;
  
  suitAdminID = suit_ID;
  taggerAdminID = tagger_ID;
  
  allConfirmationsReceived = false;
  
  suitConfirmed = false;
  taggerConfirmed = false;
  
  while (allConfirmationsReceived == false) {

    // Step 1: send instructions until the loop is exited
    
    // VIRAL TAG
    // only the tagged suit needs to send a confirmation
    if (gameMode == 1) {
      sendInstructions(suit_ID, (unsigned char) colourChangeInstruction);
      Serial.print("Sending: ");
      Serial.print(colourChangeInstruction);
      Serial.print(" to suit: ");
      Serial.println(suit_ID);
    }

    // NORMAL TAG
    // the person who was "it" is no longer "it," so they
    // have to change colour as well.
    else if (gameMode == 2) {
      sendInstructions(suit_ID, colourChangeInstruction);
      sendInstructions(tagger_ID, colourChangeInstruction);
      Serial.print("Sending: ");
      Serial.print(colourChangeInstruction);
      Serial.print(" to (suit_ID) suit: ");
      Serial.print(suit_ID);
      Serial.print(", and (tagger_ID) suit: ");
      Serial.println(tagger_ID);
    }
    

    // read from the XBee (into variable)
    unsigned char incoming = xbee.read();
    
    Serial.print("Incoming message: ");
    Serial.println(incoming);

    // if it's viral tag, we only need confirmation
    // from the suit that was tagged.
    
    if (gameMode == 1) {
      if (incoming == (unsigned char)suitConfirmationID || incoming == (unsigned char)suitAdminID) {
        if (colourChangeInstruction == 50) {
          // this statement is skipped when awaitConfirmation()
          // is called from initializeColours()
          
          // update the state array to record the colour change.
          // - 1 because the suit_IDs go from 1-10 instead of 0-9
          stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
        }
        allConfirmationsReceived = true;
        Serial.println("---------- CONFIRMATION RECEIVED, EXITING WHILE LOOP ---------");
        Serial.println();
      }
    }

    // but in "normal" tag, the person who was "it" needs
    // to send a confirmation as well.
    
    if (gameMode == 2) {
      if (suitConfirmed == false && incoming == suitConfirmationID) {
        if (colourChangeInstruction == 50) {
        // update the state array to record the colour change.
        // - 1 because the suit_IDs go from 1-10 instead of 0-9
        stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
        }
      }
      
      if (taggerConfirmed == false && incoming == taggerConfirmationID) {
        if (colourChangeInstruction == 50) {

          // store the suit's state in a temporary variable before we change it
          int suitCurrentColour = stateArray[suit_ID - 1];
        
          stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
          stateArray[tagger_ID - 1] = suitCurrentColour;
        }
      }
      
      if (suitConfirmed == true && taggerConfirmed == true) {
        allConfirmationsReceived == true;
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void startGame() {
  
  // START GAME OF VIRAL TAG
  if (gameMode == 1) {
    
    for (int i = 0; i < 10; i++) {

      // assign half of the suits a 90 (blue)
      if (i % 2 == 0) {
        stateArray[i] = 90;
      }

      // assign the other half a 91 (red)
      else {
        stateArray[i] = 91;
      }
    }
    initializeColours();
  }

  // START GAME OF REGULAR TAG
  else if (gameMode == 2) {
    
    for (int i = 0; i < 10; i++) {
      
      // assign all of the suits a 90 (blue) except the last one
      if (i != 9) {
        stateArray[i] = 90;
      }

      // the last one is "it," and assigned a 91 (red)
      else {
        stateArray[i] = 91;
      }
    }
  }

  // START GAME OF OTHER TAG
  else if (gameMode == 3) {
  //TODO
  }
}


// ---------------------------------------------------------//
// -------- Tell each suit which colour it starts as  ------//
// ---------------------------------------------------------//
void initializeColours() {
  for (int i = 0; i < (sizeof(stateArray)/sizeof(int)); i++) {
    // if (i == 4){} is for testing only, the suit I'm testing 
    // with is suit 5 (i + 1)
    
   Serial.print("----------------------- INITIALIZING SUIT  >  ");
   Serial.print(i + 1);
   Serial.println("  < ---------------------");

    if (i == 4) {
      suit_ID = i + 1 + 20;
      tagger_ID = i + 1 + 20;
      colourChangeInstruction = stateArray[i];
      awaitConfirmation();
    }
  }
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {

  // sizeof is weird and needs to be converted to int first
  Serial.println();
  Serial.println("GAME STATE: ");

  for (int i = 0; i < (sizeof(stateArray)/sizeof(int)); i++) {
    Serial.print("Suit ");
    Serial.print(i + 1);
    Serial.print(" = ");
    Serial.println(stateArray[i]);
  }
  
  Serial.println();
}
