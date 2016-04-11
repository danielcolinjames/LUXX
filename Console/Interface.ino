// ---------------------------------------------------------//
// ---------- Check to see if a button is pressed ----------//
// ---------------------------------------------------------//
void checkButtons() {
  gameModeOneButtonState = digitalRead(gameModeOneButtonPin);
  if (gameModeOneButtonState == LOW) {
    reset = true;
    
    gameMode = 1;
    
    stateReport = 101;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }

  gameModeTwoButtonState = digitalRead(gameModeTwoButtonPin);
  if (gameModeTwoButtonState == LOW) {
    reset = true;
    
    gameMode = 2;
    
    stateReport = 102;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }
  
  gameModeThreeButtonState = digitalRead(gameModeThreeButtonPin);
  if (gameModeThreeButtonState == LOW) {
    reset = true;
    
    gameMode = 3;
    
    stateReport = 103;
    sendToInterface(stateReport);

    // delay(amount of time for audio);
    
    startGame();
  }

  gameOverButtonState = digitalRead(gameOverButtonPin);
  if (gameOverButtonState == LOW) {
    reset = true;
    
    gameOver();
  }
}


// ---------------------------------------------------------//
// ------------- Wait until game is reset by us ------------//
// ---------------------------------------------------------//
void waitForReset() {
  reset = false;
  
  while(reset == false) {
    checkButtons();
  }
}


// ---------------------------------------------------------//
// ---------------- Send a message into Max ----------------//
// ---------------------------------------------------------//
void sendToInterface(uint8_t value) {  
  interfaceSerial.write(value);
}






