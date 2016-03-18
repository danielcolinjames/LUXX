// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void startGame() {
  assignStartingColours();
  sendStartingColours();
  printOutStates();
}

// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void assignStartingColours() {
  // START GAME OF VIRAL TAG
  if (gameMode == 1) {
    for (int i = 0; i < 10; i++) {
      // assign half of the suits a 89 (blue)
      if (i % 2 == 0) {
        states[i] = 89;
      }
      // assign the other half a 88 (red)
      else {
        states[i] = 88;
      }
    }
  }

  // START GAME OF REGULAR TAG
  else if (gameMode == 2) {
    for (int i = 0; i < 10; i++) {
      // assign all of the suits a 89 (blue) except the last one
      if (i != 9) {
        states[i] = 89;
      }
      // the last one is "it," and assigned a 88 (red)
      else {
        states[i] = 88;
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
void sendStartingColours() {
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    suitID = i + 1;
    
    if (i == 1 || i == 2) {
      // debugSerial.print("----------------------- ");
      // debugSerial.print("INITIALIZING SUIT  >  ");
      // debugSerial.print(suitID);
      // debugSerial.println("  < ---------------------");
    }
    
    else {
      // debugSerial.print("Skipping suit ");
      // debugSerial.print(suitID);
      // debugSerial.println(" during testing phase.");
    }
    
    delay(100);
    
    address = addresses[suitID];
    
    payload[0] = gameStartByte;
    payload[1] = states[suitID];

    packetSize = 2;
    
    xbee.send(tx);
    
    confirmDelivery(
      "Starting colour message delivered successfully.",
      "Starting colour message not delivered successfully.",
      "Starting colour message timed out."
    );

    if (instructionReceived == false) {
      xbee.send(tx);
      confirmDelivery(
        "Instruction 97 to tagger successful on second attempt.",
        "Instruction 97 to tagger failed on second attempt.",
        "Instruction 97 to tagger timed out on second attempt."
        );
    }

    if (instructionReceived == false) {
      xbee.send(tx);
      confirmDelivery(
        "Instruction 97 to tagger successful on third attempt.",
        "Instruction 97 to tagger failed on third attempt.",
        "Instruction 97 to tagger timed out on third attempt."
        );
    }
      
    if (instructionReceived == true) {
      activeSuits[suitID] = true;
    }
    else {
      activeSuits[suitID] = false;
    }
  }
}


// ---------------------------------------------------------//
// ----------- Check to see if the game should end ---------//
// ---------------------------------------------------------//
void gameStateCheck() {
  int sum = 0;
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    if (activeSuits[i] == true) {
      sum = sum + states[i];
    }
  }
  
  if (sum == (89 * (sizeof(activeSuits)/sizeof(int)))) {
    // debugSerial.println("All suits are red. Red wins.");
    printOutStates();
    sendGameOver();
  }

  if (sum == (88 * (sizeof(activeSuits)/sizeof(int)))) {
    // debugSerial.println("All suits are blue. Blue wins.");
    printOutStates();
    sendGameOver();
  }
  
  if (millis() > 300000) {
    sendGameOver();
    // debugSerial.println("Game over, time limit reached.");
  }
}


// ---------------------------------------------------------//
// -----------   Tell each suit the game is over   ---------//
// ---------------------------------------------------------//
void sendGameOver() {
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    suitID = i + 1;

    // only turn off the suits that are active this round
    if (activeSuits[suitID] == true) {

      if (suitID == 1 || suitID == 2) {
        // debugSerial.print("----------------------- ");
        // debugSerial.print("DEACTIVATING SUIT  >  ");
        // debugSerial.print(suitID);
        // debugSerial.println("  < ---------------------");
      }
      
      else {
        // debugSerial.print("Skipping suit ");
        // debugSerial.print(suitID);
        // debugSerial.println(" during testing phase.");
      }
      
      delay(100);
      
      address = addresses[suitID];
      payload[0] = gameOverByte;
      packetSize = 1;
      
      xbee.send(tx);
      
      confirmDelivery(
        "Game over message delivered successfully.",
        "Game over message not delivered successfully.",
        "Game over message timed out."
      );

      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to tagger successful on second attempt.",
          "Instruction 97 to tagger failed on second attempt.",
          "Instruction 97 to tagger timed out on second attempt."
          );
      }

      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to tagger successful on third attempt.",
          "Instruction 97 to tagger failed on third attempt.",
          "Instruction 97 to tagger timed out on third attempt."
          );
      }
      
      if (instructionReceived == true) {
        activeSuits[suitID] = false;
      }
      else {
        activeSuits[suitID] = true;
      }
    }
  }
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {
  // debugSerial.println("GAME STATE: ");
  
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    // debugSerial.print("Suit ");
    // debugSerial.print(i + 1);
    // debugSerial.print(" = ");
    // debugSerial.println(states[i]);
  }
}

