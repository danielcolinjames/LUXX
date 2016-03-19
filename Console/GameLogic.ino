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
// ----------- Check to see if the game should end ---------//
// ---------------------------------------------------------//
void gameStateCheck() {
  
  if (states[0] == states[1] == states[2] == states[3] == states[4]
    == states[5] == states[6] == states[7] == states[8] == states[9]) {
      debugSerial.println();
      debugSerial.println();
      debugSerial.println("Game over, everyone is the same colour.");

      gameOver();
  }
  
  if (millis() > 300000) {
    debugSerial.println();
    debugSerial.println();
    debugSerial.println("Game over, time limit reached.");
    gameOver();
  }
}


// ---------------------------------------------------------//
// ----------- Check to see if the game should end ---------//
// ---------------------------------------------------------//
void gameOver() {
  sendGameOver();
  while(1) {
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

