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

  // min is inclusive, max is exclusive
  // randomNum = a number from 1 - 4
  uint8_t randomNum = random(1, 5);
  
  // multiply this number by 2, to get one of: (2, 4, 6, 8)
  randomNum *= 2;
  
  // add 80 to make it a colour code
  // add 1 to get that colour's pair
  coolColour = 80 + randomNum;
  warmColour = 80 + randomNum + 1;
  
  if (gameMode == 0) {
    
    // Viral Tag Original: one assigned warm, rest assigned cool
    
    for (int i = 0; i < 10; i++) {
      // assign all of the suits except one as a cool colour
      if (i != 0) {
        states[i] = coolColour;
      }
      // assign the first one as warm (first because we don't know
      // how many suits are active yet)
      else {
        states[i] = warmColour;
      }
    }
  }
  
  else if (gameMode == 1) {
    
    // Viral Tag Split: half assigned warm, half assigned cool
    
    for (int i = 0; i < 10; i++) {
      // assign half of the suits a cool colour
      if (i % 2 == 0) {
        states[i] = warmColour;
      }
      // assign the other half a warm colour
      else {
        states[i] = coolColour;
      }
    }
  }
  
  else if (gameMode == 2) {
    
    // Traditional Tag: one person is it (warm)
    
    for (int i = 0; i < 10; i++) {
      // assign all of the suits a cool colour except the first one
      if (i != 0) {
        states[i] = coolColour;
      }
      // the first one is "it," and assigned a warm colour
      else {
        states[i] = warmColour;
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
    numberOfCoolSuits = 0;
    numberOfWarmSuits = 0;
    
    for (int i = 0; i < 10; i++) {
      if (activeSuits[i] == true) {
        numberOfActiveSuits++;
        if (states[i] % 2 == 0) {
          numberOfCoolSuits++;
        }
        else if (states[i] % 2 != 0) {
          numberOfWarmSuits++;
        }
      }
    }

    // Viral Tag original:
    if (gameMode == 0) {
      // if there's only one suit left that's cool (uninfected), check the state quicker
      if (numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are warm (infected)
      if (numberOfWarmSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a warm colour.");
        
        sendMessageToTouch(75, warmColour);
        
        gameOver();
      }
    }
    
    // Viral Tag split:
    else if (gameMode == 1) {
      // if there's only one suit left that's a different colour, check the state quicker
      if (numberOfCoolSuits == (numberOfActiveSuits - 1) || numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are cool
      if (numberOfCoolSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a cool colour.");
        
        sendMessageToTouch(75, coolColour);
        
        gameOver();
      }
  
      // all the suits are warm
      else if (numberOfWarmSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a warm colour.");
        
        sendMessageToTouch(75, warmColour);
        
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

