// ---------------------------------------------------------//
// ---------- Check to see if a button is pressed ----------//
// ---------------------------------------------------------//
void checkButtons() {
  gameModeZeroButtonState = digitalRead(gameModeZeroButtonPin);
  if (reset == false && gameModeZeroButtonState == LOW) {
    // debugSerial.println("Button 0 pressed.");
    // debugSerial.println("Starting game of [VIRAL TAG ONE].");
    
    reset = true;
    
    gameMode = 0;
    
    stateReport = 150;
    // this number (and 151, 152, 153) are to tell me through the interface
    // that the button press was read
    
    // the interface actually responds to the numbers 100 through 103, but 
    // we want to do that after the ping has completed because the number 
    // of suits will make the ping test variable in length.
    sendToInterface(stateReport);
    
    startGame();
  }
  
  gameModeOneButtonState = digitalRead(gameModeOneButtonPin);
  if (reset == false && gameModeOneButtonState == LOW) {
    // debugSerial.println("Button 1 pressed.");
    // debugSerial.println("Starting game of [VIRAL TAG SPLIT].");
    
    reset = true;
    
    gameMode = 1;
    
    stateReport = 151;
    sendToInterface(stateReport);
    
    startGame();
  }
  
  gameModeTwoButtonState = digitalRead(gameModeTwoButtonPin);
  if (reset == false && gameModeTwoButtonState == LOW) {
    // debugSerial.println("Button 2 pressed.");
    // debugSerial.println("Starting game of [TRADITIONAL TAG].");
    
    reset = true;
    
    gameMode = 2;
    
    stateReport = 152;
    sendToInterface(stateReport);
    
    startGame();
  }
  
  gameModeThreeButtonState = digitalRead(gameModeThreeButtonPin);
  if (reset == false && gameModeThreeButtonState == LOW) {
    // debugSerial.println("Button 3 pressed.");
    // debugSerial.println("Starting game of [CHAOS TAG].");
    
    reset = true;
    
    gameMode = 3;
    
    stateReport = 153;
    sendToInterface(stateReport);
    
    startGame();
  }
  
  gameOverButtonState = digitalRead(gameOverButtonPin);
  if (gameOverButtonState == LOW) {
    // debugSerial.println("Button 4 pressed.");
    // debugSerial.println("[GAME OVER] triggered manually.");
    
    delay(100);
    
    stateReport = 107;
    sendToInterface(stateReport);
    
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






