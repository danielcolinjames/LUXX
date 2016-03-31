/*
 * 
 * 79 - Game state:
 *      Number of active suits,
 *      Number of warm suits,
 *      Number of cool suits
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
    
    interfaceSerial.listen();
    
    while (interfaceSerial.available() && listeningBoolean == true) {
      uint8_t reading = interfaceSerial.read();
      debugSerial.print("Reading: ");
      debugSerial.println(reading);
      
      if (reading == 1) {
        listeningBoolean = false;
        reset = true;
        
        debugSerial.println("RESTARTING GAME");
        
        startGame();
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------- Send information into the interface ----------//
// ---------------------------------------------------------//
void sendStateToInterface() {
  if (millis() - outputMillis > outputInterval) {
    outputMillis = millis();
    
    outputSerial.print((uint8_t)79);
    outputSerial.print((uint8_t)numberOfActiveSuits);
    outputSerial.print((uint8_t)numberOfWarmSuits);
    outputSerial.print((uint8_t)numberOfCoolSuits);
  }
}


// ---------------------------------------------------------//
// ---------- Send information into the interface ----------//
// ---------------------------------------------------------//
void sendMessageToInterface(uint8_t messageType, uint8_t message) {  
  interfaceSerial.write(messageType);
  interfaceSerial.write(message);
}

// ---------------------------------------------------------//
// ---------- Listen for messages sent through Max ---------//
// ---------------------------------------------------------//
void listenToInterface() {
  interfaceSerial.listen();
  while (interfaceSerial.available() && listeningBoolean == true) {
    int reading = interfaceSerial.read();
    debugSerial.print("Reading: ");
    debugSerial.println(reading);
    
    if (reading == 1) {
      listeningBoolean = false;
      
      debugSerial.println("STARTING GAME");
      
      startGame();
    }
    else if (reading == 2) {
      listeningBoolean = false;
      
      debugSerial.println("ENDING GAME");
      
      gameOver();
    }
  }
}


// ---------------------------------------------------------//
// ---------------- Send a message into Max ----------------//
// ---------------------------------------------------------//
void sendToInterface(uint8_t value) {  
  interfaceSerial.write(value);
}






