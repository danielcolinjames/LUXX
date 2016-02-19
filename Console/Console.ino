#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // (RX, TX)

// 90 for blue, 91 for red, 92 for purple/white
int stateArray[] = { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 };

unsigned char suit_ID = 0;
unsigned char tagger_ID = 0;

int gameMode = 1;

unsigned char colourChangeInstruction = 0;

boolean suitIsReadyToReceive = false;
boolean suitHasReceivedInstruction = false;

boolean taggerIsReadyToReceive = false;
boolean taggerHasReceivedInstruction = false;

unsigned long timeoutMillis = millis();


/*
 The numbers being added to the suit IDs below are arbitrary. 

 They correspond to different stages in the communication.
 
 This is done to block off sets of 10 numbers that are different 
 from the suit IDs themselves, but still unique to each suit. This
 is ultimately done to avoid any repeated numbers being sent.
*/


// admin messages are sent at the start of a new game
// and they tell each suit which colour it starts as
// ---------------------------------------------------
int suitAdminID = suit_ID + 80;
int taggerAdminID = tagger_ID + 80;



// the readyID tells the console this suit is ready to
// receive a colour instruction.
// ---------------------------------------------------
int suitReadyID = suit_ID + 10;
int taggerReadyID = tagger_ID + 10;



// the confirmationID tells the console that this suit
// has received the instruction and updated itself.
// ---------------------------------------------------
int suitConfirmationID = suit_ID + 20;
int taggerConfirmationID = tagger_ID + 20;



// These booleans are conditions for the receiving loop.
// ---------------------------------------------------
boolean suitConfirmed = false;
boolean taggerConfirmed = false;

boolean allConfirmationsReceived = false;




// ---------------------------------------------------------//
// ------------------------  Setup  ------------------------//
// ---------------------------------------------------------//
void setup() {

  Serial.begin(9600);
  Serial.println("START GAME");

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
    Serial.print("----------------------- MESSAGE ");
    Serial.println("RECEIVED ---------------------");
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

    suitReadyID = suit_ID + 10;
    suitConfirmationID = suit_ID + 20;
    
    taggerReadyID = tagger_ID + 10;
    taggerConfirmationID = tagger_ID + 20;
    
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
// ---  Sends instruction to suit and confirms reception ---//
// ---------------------------------------------------------//
void awaitConfirmation() {

  suitIsReadyToReceive = false;
  suitHasReceivedInstruction = false;

  taggerIsReadyToReceive = false;
  taggerHasReceivedInstruction = false;

  if (gameMode == 1) {
    while (suitIsReadyToReceive == false) {
      xbee.write(suit_ID);
      
      Serial.print("Sending: ");
      Serial.print(suit_ID);
      Serial.print(", looking for: ");
      Serial.print(suitReadyID);
      
      unsigned char incomingReadyID = 0;

      while (xbee.available() > 0) {
        incomingReadyID = xbee.read();
      }
      
      Serial.print(", receiving: ");
      Serial.println(incomingReadyID);
      
      if (incomingReadyID == suitReadyID) {
        suitIsReadyToReceive = true;
        
        while (suitHasReceivedInstruction == false) {
          xbee.write(colourChangeInstruction);
          
          Serial.print("Sending: ");
          Serial.print(colourChangeInstruction);
          Serial.print(", looking for: ");
          Serial.print(suitConfirmationID);
      
          unsigned char incomingConfirmationID = 0;

          while (xbee.available() > 0) {
            incomingConfirmationID = xbee.read();
          }
          
          Serial.print(", receiving: ");
          Serial.println(incomingConfirmationID);
      
          if (incomingConfirmationID == suitConfirmationID) {
            
            Serial.println("------------------ SUIT HAS RECEIVED MESSAGE -----------------");

            suitHasReceivedInstruction = true;
            if (colourChangeInstruction == 50) {
                            
              stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
            }
          }
        }
      }
    }
  }
  
  else if (gameMode == 2) {
    unsigned char incoming = xbee.read();

    int suitCurrentColour = 0;
    
    allConfirmationsReceived = false;
    
    while (allConfirmationsReceived == false) {
      
      while (suitIsReadyToReceive == false) {
        xbee.write(suit_ID);
        
        if (xbee.read() == suitReadyID) {
          suitIsReadyToReceive = true;
          
          while (suitHasReceivedInstruction == false) {
            xbee.write(colourChangeInstruction);

            if (xbee.read() == suitConfirmationID) {
              
              suitHasReceivedInstruction = true;

              if (colourChangeInstruction == 50) {
                suitCurrentColour = stateArray[suit_ID - 1];
            
                stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
              }
            }
          }
        }
      }
      
      while (taggerIsReadyToReceive == false) {
        xbee.write(tagger_ID);
        
        if (xbee.read() == taggerReadyID) {
          taggerIsReadyToReceive = true;
          
          while (taggerHasReceivedInstruction == false) {
            xbee.write(colourChangeInstruction);

            if (xbee.read() == taggerConfirmationID) {
              taggerHasReceivedInstruction = true;

              if (colourChangeInstruction == 50) {
                stateArray[tagger_ID - 1] = suitCurrentColour;
              }
            }
          }
        }
      }
      
      if (suitHasReceivedInstruction && taggerHasReceivedInstruction) {
        allConfirmationsReceived = true;
        
        Serial.print("---------- CONFIRMATION RECEIVED");
        Serial.println(", EXITING WHILE LOOP ---------");
        Serial.println();
      }
    }
  }
}


// ---------------------------------------------------------//
// --------------- Send out an admin message  --------------//
// ---------------------------------------------------------//
void sendAdminMessage() {
  
  suitIsReadyToReceive = false;
  suitHasReceivedInstruction = false;
  
  while (suitIsReadyToReceive == false) {
//    if (checkForTimeout()) break;
    
    xbee.write(suitAdminID);
    
    Serial.print("STEP 1 - Sending: ");
    Serial.print(suitAdminID);
    Serial.print(", looking for: ");
    Serial.print(suitReadyID);

    xbee.listen();
    
    Serial.print(", receiving: ");
    unsigned char incomingReadyID = 0;
    
    while (xbee.available() > 0) {
//      if (checkForTimeout()) break;
      
      incomingReadyID = xbee.read();
    }
    
    Serial.println(incomingReadyID);
    
    xbee.listen();
    
    if (incomingReadyID == suitReadyID) {
      suitIsReadyToReceive = true;
      
      while (suitHasReceivedInstruction == false) {
//        if (checkForTimeout()) break;
        
        xbee.write(colourChangeInstruction);

        Serial.print("STEP 2 - Sending: ");
        Serial.print(colourChangeInstruction);
        Serial.print(", looking for: ");
        Serial.print(suitConfirmationID);

        xbee.listen();

        unsigned char incomingConfirmation = 0;
        
        while (xbee.available() > 0) {
            incomingConfirmation = xbee.read();
        }
        
        Serial.print(", receiving: ");
        Serial.println(incomingConfirmation);

        xbee.listen();
        
        if (incomingConfirmation == suitConfirmationID) {
          
          xbee.write((unsigned char)77);
          
          Serial.print("------------------------- ");
          Serial.println("ADMIN COMMAND COMPLETE -----------------------");
          Serial.println();
          suitHasReceivedInstruction = true;
          if (colourChangeInstruction == 50) {
            stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
          }
        }
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
   
    suit_ID = i + 1;
    suitAdminID = suit_ID + 80;
    
    suitReadyID = suit_ID + 10;
    suitConfirmationID = suit_ID + 20;
    
    if (i == 4) {
      
      colourChangeInstruction = (unsigned char)stateArray[i];
      sendAdminMessage();
    }
  }
  printOutStates();
}



// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {;

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



// ---------------------------------------------------------//
// ------------------  Check for a timeout  ----------------//
// ---------------------------------------------------------//
boolean checkForTimeout() {
  if ((millis() - timeoutMillis) > 2000) {
    timeoutMillis = 0;
    Serial.print("------------------!!!!!!!! XBEE HAS ");
    Serial.println("TIMED OUT !!!!!!!!!-----------------");
    Serial.println();
    return true;
  }
  else {
    return false;
  }
}

