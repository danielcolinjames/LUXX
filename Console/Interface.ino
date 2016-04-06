// ---------------------------------------------------------//
// ------------- Wait until game is reset by us ------------//
// ---------------------------------------------------------//
void waitForReset() {
  boolean reset = false;
  listeningBoolean = true;
  
  debugSerial.println("Waiting for reset...");
  
  while(reset == false) {
    
    interfaceSerial.listen();
    
    while (interfaceSerial.available() && listeningBoolean == true) {
      uint8_t reading = interfaceSerial.read();
      debugSerial.print("Reading: ");
      debugSerial.println(reading);
      
      if (reading == 100) {
        listeningBoolean = false;
        reset = true;
        debugSerial.println("STARTING GAME, MODE 0");
        gameMode = 0;
        startGame();
      }
      else if (reading == 101) {
        listeningBoolean = false;
        reset = true;
        debugSerial.println("STARTING GAME, MODE 1");
        gameMode = 1;
        startGame();
      }
      else if (reading == 102) {
        listeningBoolean = false;
        reset = true;
        debugSerial.println("STARTING GAME, MODE 2");
        gameMode = 2;
        startGame();
      }
      else if (reading == 111) {
        listeningBoolean = false;
        reset = true;
        debugSerial.println("MANUAL GAME OVER TRIGGERED");
        gameOver();
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------- Listen for messages sent through Max ---------//
// ---------------------------------------------------------//
void listenToInterface() {
  interfaceSerial.listen();
  listeningBoolean = true;
  
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
    else if (reading < 100 && reading >= 0) {
      debugSerial.println("MANUAL COLOUR INSTRUCTION");
      listeningBoolean = false;
      uint8_t tempColour = 80 + (reading % 10);
      uint8_t tempRecepient = (reading - (reading % 10)) / 10;
      debugSerial.println("MANUALLY TARGETTING SUIT");
      manualColourAssignment(tempRecepient, tempColour);
    }
  }
}


// ---------------------------------------------------------//
// ---------------- Send a message into Max ----------------//
// ---------------------------------------------------------//
void sendToInterface(uint8_t value) {  
  interfaceSerial.write(value);
}






