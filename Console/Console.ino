#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // (RX, TX)

// 0 for blue, 1 for red, 2 for purple/white
int stateArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char suit_ID = 0;
unsigned char tagger_ID = 0;

int confirmationReceived = 0;

boolean debugging = 1;

boolean allConfirmationsReceived = false;

// 1 = viral tag
int gameMode = 1;

int colourChangeInstruction = 0;


// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {

  Serial.begin(9600);
  Serial.println("Start");

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

  if (incoming == 15) {
    startNumberFound = true;
    Serial.println("Start number found. Next two values are SUIT_ID and TAGGER_ID.");
  }
  else {
    startNumberFound = false;
//      Serial.println("Start number not found.");
  }

  if (xbee.available() > 0 && startNumberFound) {

    suit_ID = xbee.read();
    Serial.println(suit_ID);

    tagger_ID = xbee.read();
    Serial.println(tagger_ID);

    // determine instructions
    

    // send instructions
    

    // await response
    awaitConfirmation();
    
  }
}


// ---------------------------------------------------------//
//  Determines whether or not the suit should change colour //
// ---------------------------------------------------------//
int shouldSuitChangeColour() {
  
    if (stateArray[suit_ID - 1] != stateArray[tagger_ID - 1]) {
    // the tagger is a different colour than the person being
    // tagged, so the person being tagged (the one who sent
    // the message to the console) should change colour.

    colourChangeInstruction = 50;
    // 50 means it should change colour.
  } else {
    colourChangeInstruction = 55;
    // 55 means it should not.
  }
}


// ---------------------------------------------------------//
// ----- Tell the suit whether or not to change colour -----//
// ---------------------------------------------------------//
void sendInstructions(int recepient, int message) {

  // suit_ID is currently equal to the suit tagged so
  // we need to address it in our instructions message
  xbee.write((unsigned char)recepient);
  Serial.print("Sending to: :");
  Serial.println(recepient);
  
  xbee.write((unsigned char)message);
  Serial.print("Message sent: ");
  Serial.println(message);
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
  int suitConfirmationID = suit_ID + 10;
  int taggerConfirmationID = tagger_ID + 10;
  
  boolean suitConfirmed = false;
  boolean taggerConfirmed = false;
  
  while (!allConfirmationsReceived) {

    // Step 1: send instructions until the loop is exited
    
    // VIRAL TAG
    // only the tagged suit needs to send a confirmation
    if (gameMode == 1) {
      sendInstructions(suit_ID, colourChangeInstruction);
    }

    // NORMAL TAG
    // the person who was "it" is no longer "it," so they
    // have to change colour as well.
    else if (gameMode == 2) {
      sendInstructions(suit_ID, colourChangeInstruction);
      sendInstructions(tagger_ID, colourChangeInstruction);
    }

    // read from the XBee (into variable)
    unsigned char incoming = xbee.read();
    
    Serial.print("Incoming message: ");
    Serial.println(incoming);

    // if it's viral tag, we only need confirmation
    // from the suit that was tagged.
    
    if (gameMode == 1) {      
      if (incoming == (unsigned char)suitConfirmationID) {
        if (colourChangeInstruction == 50) {
        // update the state array to record the colour change.
        // - 1 because the suit_IDs go from 1-10 instead of 0-9
        stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
        allConfirmationsReceived = true;
        }
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

      // assign half of the suits a 1 (red)
      if (i % 2 == 0) {
        stateArray[i] = 1;
      }

      // assign the other half a 0 (blue)
      else {
        stateArray[i] = 0;
      }
    }
    printOutStates();
  }

  // START GAME OF REGULAR TAG
  else if (gameMode == 2) {
    
    for (int i = 0; i < 10; i++) {
      
      // assign all of the suits a 0 (blue) except the last one
      if (i != 9) {
        stateArray[i] = 0;
      }

      // the last one is "it," and assigned a 1 (red)
      else {
        stateArray[i] = 1;
      }
    }
  }

  // START GAME OF OTHER TAG
  else if (gameMode == 3) {
  //TODO
  }
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {

  // sizeof is weird and needs to be converted to int first
  for (int i = 0; i < (sizeof(stateArray)/sizeof(int)); i++) {
    Serial.print("Suit ");
    Serial.print(i + 1);
    Serial.print(" = ");
    Serial.println(stateArray[i]);
  }
}
