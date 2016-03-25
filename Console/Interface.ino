/*
 * 
 * 79 - Game state:
 *      Number of active suits,
 *      Number of red suits,
 *      Number of blue suits
 *      
 * 78 - Start game:
 *      Number of active suits
 *      
 * 77 - Suit was tagged:
 *      New colour (0 if only 1 person is it)
 *      
 * 76 - Friendly fire:
 *      Suit colour
 *      
 * 75 - Game over:
 *      Winning colour (or 0 if timeout)
 *      
 */


// ---------------------------------------------------------//
// ------------- Wait until game is reset by us ------------//
// ---------------------------------------------------------//
void waitForReset() {
  debugSerial.println("Waiting for reset...");
  boolean reset = false;
  
  while(reset == false) {
    // game over, wait for manual reset from TouchDesigner
    if (inputSerial.available() > 0) {
      debugSerial.println("Message received from TouchDesigner");
      String incoming = inputSerial.readString();
      if (incoming.toInt() == 98) {
        debugSerial.println("98 received from TouchDesigner. Starting new game...");
        reset = true;
        startGame();
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------- Send information into TouchDesigner ----------//
// ---------------------------------------------------------//
void sendStateToTouch() {
  if (millis() - outputMillis > outputInterval) {
    outputMillis = millis();
    
    outputSerial.print((uint8_t)79);
    outputSerial.print((uint8_t)numberOfActiveSuits);
    outputSerial.print((uint8_t)numberOfRedSuits);
    outputSerial.print((uint8_t)numberOfBlueSuits);
  }
}


// ---------------------------------------------------------//
// ---------- Send information into TouchDesigner ----------//
// ---------------------------------------------------------//
void sendMessageToTouch(uint8_t messageType, uint8_t message) {
  if (millis() - outputMillis > outputInterval) {
    outputMillis = millis();
    
    outputSerial.print((uint8_t)messageType);
    outputSerial.print((uint8_t)message);
  }
}
