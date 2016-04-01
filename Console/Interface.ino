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
// ---------- Listen for messages sent through Max ---------//
// ---------------------------------------------------------//
void listenToInterface() {
  interfaceSerial.listen();
  while (interfaceSerial.available() && listeningBoolean == true) {
    int reading = interfaceSerial.read();
    debugSerial.print("Reading: ");
    debugSerial.println(reading);
    
    if (reading == 100) {
      listeningBoolean = false;
      debugSerial.println("STARTING GAME, MODE 0");
      gameMode = 0;
      startGame();
    }
    else if (reading == 101) {
      listeningBoolean = false;
      debugSerial.println("STARTING GAME, MODE 1");
      gameMode = 1;
      startGame();
    }
    else if (reading == 102) {
      listeningBoolean = false;
      debugSerial.println("STARTING GAME, MODE 2");
      gameMode = 2;
      startGame();
    }
    else if (reading == 111) {
      listeningBoolean = false;
      debugSerial.println("MANUAL GAME OVER TRIGGERED");
      gameOver();
    }
    else {
      listeningBoolean = false;
      uint8_t tempColour = 80 + (reading % 10);
      uint8_t tempAddress = (reading - tempColour) / 10;
      debugSerial.println("MANUALLY TARGETTING SUIT");
      manualColourAssignment(tempAddress, tempColour);
    }
  }
}


// ---------------------------------------------------------//
// ---------------- Send a message into Max ----------------//
// ---------------------------------------------------------//
void sendToInterface(uint8_t value) {  
  interfaceSerial.write(value);
}






