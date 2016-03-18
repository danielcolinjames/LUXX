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
  int sum = 0;
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    if (activeSuits[i] == true) {
      sum = sum + states[i];
    }
  }
  
  if (sum == (89 * (sizeof(activeSuits)/sizeof(int)))) {
    debugSerial.println("All suits are red. Red wins.");
    printOutStates();
    sendGameOver();
  }

  if (sum == (88 * (sizeof(activeSuits)/sizeof(int)))) {
    debugSerial.println("All suits are blue. Blue wins.");
    printOutStates();
    sendGameOver();
  }
  
  if (millis() > 300000) {
    sendGameOver();
    debugSerial.println("Game over, time limit reached.");
  }
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {
  debugSerial.println("GAME STATE: ");
  
  for (int i = 0; i < (sizeof(states)/sizeof(int)); i++) {
    debugSerial.print("Suit ");
    debugSerial.print(i + 1);
    debugSerial.print(" = ");
    debugSerial.println(states[i]);
  }
}

