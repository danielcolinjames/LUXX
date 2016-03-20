// ---------------------------------------------------------//
// -------------------- Start the game ---------------------//
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
  if (gameMode == 0) {
    
    // Viral Tag Original: one assigned red, rest assigned blue
    
    for (int i = 0; i < 10; i++) {
      // assign all of the suits except one as 88 (blue)
      if (i != 9) {
        states[i] = 88;
      }
      // assign the last one as red
      else {
        states[i] = 89;
      }
    }
  }
  
  else if (gameMode == 1) {
    
    // Viral Tag Split: half assigned red, half assigned blue
    
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
  
  else if (gameMode == 2) {
    
    // Traditional Tag: one person is it (red)
    
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
// ----------- Check to see if the game should end ---------//
// ---------------------------------------------------------//
void gameStateCheck() {
  
  if ((millis() - stateMillis) > stateCheckInterval) {
    
    stateMillis = millis();
    
    numberOfActiveSuits = 0;
    numberOfBlueSuits = 0;
    numberOfRedSuits = 0;
    
    for (int i = 0; i < 10; i++) {
      if (activeSuits[i] == true) {
        numberOfActiveSuits++;
        if (states[i] == 88) {
          numberOfBlueSuits++;
        }
        else if (states[i] == 89) {
          numberOfRedSuits++;
        }
      }
    }
    
    // if there's only one suit left that's a different colour, check the state quicker
    if (numberOfBlueSuits == (numberOfActiveSuits - 1) || numberOfRedSuits == (numberOfActiveSuits - 1)) {
      stateCheckInterval = 10;
    }
    else {
      stateCheckInterval = 1000;
    }
    
    // all the suits are blue
    if (numberOfBlueSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is blue.");
        
        gameOver();
    }

    // all the suits are red
    if (numberOfRedSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is red.");
        
        gameOver();
    }

    // game has timed out
    if (millis() > tenMinutes) {
      debugSerial.println();
      debugSerial.println("Game over: time limit reached.");
      
      gameOver();
    }
  }
}


// ---------------------------------------------------------//
// ----------- Check to see if the game should end ---------//
// ---------------------------------------------------------//
void gameOver() {
  sendGameOver();
  waitForReset();
}


// ---------------------------------------------------------//
// ------------- Wait until game is reset by us ------------//
// ---------------------------------------------------------//
void waitForReset() {
  debugSerial.println("Waiting for reset...");
  while(1 == 1) {
    // game over, wait for manual reset (from Max?)
  }
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {
  debugSerial.println();
  debugSerial.println("GAME STATE: ");
  
  for (int i = 0; i < 10; i++) {
    debugSerial.print("Suit ");
    debugSerial.print(i + 1);
    debugSerial.print(" = ");
    debugSerial.println(states[i]);
  }
}

