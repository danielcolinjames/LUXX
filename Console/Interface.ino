// ---------------------------------------------------------//
// ---------- Check to see if a button is pressed ----------//
// ---------------------------------------------------------//
void checkButtons() {
  gameModeZeroButtonState = digitalRead(gameModeZeroButtonPin);
  if (reset == false && gameModeZeroButtonState == LOW) {
    debugSerial.println("Button 0 pressed.");
    debugSerial.println("Starting game of [VIRAL TAG ONE].");

    reset = true;
    
    gameMode = 0;
    
    stateReport = 100;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }
  
  gameModeOneButtonState = digitalRead(gameModeOneButtonPin);
  if (reset == false && gameModeOneButtonState == LOW) {
    debugSerial.println("Button 1 pressed.");
    debugSerial.println("Starting game of [VIRAL TAG SPLIT].");
    
    reset = true;
    
    gameMode = 1;
    
    stateReport = 101;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }
  
  gameModeTwoButtonState = digitalRead(gameModeTwoButtonPin);
  if (reset == false && gameModeTwoButtonState == LOW) {
    debugSerial.println("Button 2 pressed.");
    debugSerial.println("Starting game of [TRADITIONAL TAG].");
    
    reset = true;
    
    gameMode = 2;
    
    stateReport = 102;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }
  
  gameModeThreeButtonState = digitalRead(gameModeThreeButtonPin);
  if (reset == false && gameModeThreeButtonState == LOW) {
    debugSerial.println("Button 3 pressed.");
    debugSerial.println("Starting game of [CHAOS TAG].");
    
    reset = true;
    
    gameMode = 3;
    
    stateReport = 103;
    sendToInterface(stateReport);
    
    // delay(amount of time for audio);
    
    startGame();
  }
  
  gameOverButtonState = digitalRead(gameOverButtonPin);
  if (gameOverButtonState == LOW) {
    debugSerial.println("Button 4 pressed.");
    debugSerial.println("[GAME OVER] triggered manually.");
    
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






