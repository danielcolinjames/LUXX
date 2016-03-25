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
  
  /*
   * Game modes:
   *    0 = viral tag original
   *    1 = viral tag split
   *    2 = traditional tag
   */
   
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

    // Viral Tag original:
    if (gameMode == 0) {
      // if there's only one suit left that's blue (uninfected), check the state quicker
      if (numberOfRedSuits == (numberOfActiveSuits - 1)) {
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are red (infected)
      if (numberOfRedSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is red.");
        
        sendMessageToTouch(75, 89);
        
        gameOver();
      }
    }
    
    // Viral Tag split:
    else if (gameMode == 1) {
      // if there's only one suit left that's a different colour, check the state quicker
      if (numberOfBlueSuits == (numberOfActiveSuits - 1) || numberOfRedSuits == (numberOfActiveSuits - 1)) {
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are blue
      if (numberOfBlueSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is blue.");
        
        sendMessageToTouch(75, 89);
        
        gameOver();
      }
  
      // all the suits are red
      else if (numberOfRedSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is red.");
        
        sendMessageToTouch(75, 89);
        
        gameOver();
      }
    }

    // timeout check for all game modes
    if (millis() > 600000) {
      debugSerial.println();
      debugSerial.println("Game over: time limit reached.");
      
      sendMessageToTouch(75, 0);
      
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
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {
  debugSerial.println();
  debugSerial.println("GAME STATE: ");
  
  for (int i = 0; i < 10; i++) {
    if (activeSuits[i] == true) {
      debugSerial.print("Suit ");
      debugSerial.print(i + 1);
      debugSerial.print(" = ");
      debugSerial.println(states[i]);
    }
  }
}

