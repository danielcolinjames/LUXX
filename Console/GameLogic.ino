// ---------------------------------------------------------//
// -------------------- Start the game ---------------------//
// ---------------------------------------------------------//
void startGame() {
  
  pingSuits();
  
  if (numberOfActiveSuits > 1) {
    assignStartingColours();
    sendStartingColours();
    printOutStates();
  }
  else {
    debugSerial.println("There must be more than 1 suit to play.");
  }
}


// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void pingSuits() {
  for (int i = 0; i < 10; i++) {
    suitID = i;
    
    debugSerial.println();
    debugSerial.print("-------------- ");
    debugSerial.print("PINGING SUIT  >  ");
    debugSerial.print(suitID);
    debugSerial.println("  < ------------");
    debugSerial.println();
        
    address = addresses[suitID];
    
    payload[0] = pingByte;
    
    packetSize = 1;
    
    tx = Tx16Request(address, payload, packetSize);
          
    // first attempt
    xbee.send(tx);
    confirmDelivery(pingByte, 1, suitID);
    
    // second attempt
    if (suitReceivedInstruction == false) {
      xbee.send(tx);
      confirmDelivery(pingByte, 2, suitID);
    }
    
    // third attempt
    if (suitReceivedInstruction == false) {
      xbee.send(tx);
      confirmDelivery(pingByte, 3, suitID);
    }
    
    // the suit got the message, do the following
    if (suitReceivedInstruction == true) {
      numberOfActiveSuits++;
      activeSuits[suitID] = true;
      
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is ACTIVE.");
    }
    
    else {
      activeSuits[suitID] = false;
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is inactive.");
    }

    // sends a colour change report from 0 - 99 to the interface
    stateReport = (suitID * 10) + (states[suitID] - 80);
    interfaceSerial.write(stateReport);
  }
}


// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void assignStartingColours() {

  // min is inclusive, max is exclusive
  // randomNum = a number from 1 - 4
  uint8_t randomNum = random(1, 5);
  
  debugSerial.print("Random number: ");
  debugSerial.println(randomNum);
  
  // multiply this number by 2, to get one of: (2, 4, 6, 8)
  randomNum *= 2;
  
  // add 80 to make it a colour code
  coolColour = 80 + randomNum;
  
  // add 1 to get that colour's pair
  warmColour = 80 + randomNum + 1;
  
  debugSerial.print("Cool: ");
  debugSerial.println(coolColour);
  debugSerial.print("Warm: ");
  debugSerial.println(warmColour);
  
  if (gameMode == 0) {
    
    // Viral Tag Original: one assigned warm, rest assigned cool
    for (int i = 0; i < 10; i++) {
      
      uint8_t counter = 0;
      
      if (activeSuits[i] == true) {
        
        counter++;
        
        // the last suit is warm
        if (counter == numberOfActiveSuits) {
          states[i] = warmColour;
        }

        // all other suits are cool
        else {
          states[i] = coolColour;
        }
      }
    }
  }
  
  else if (gameMode == 1) {
    
    // Viral Tag Split: half assigned warm, half assigned cool
    
    for (int i = 0; i < 10; i++) {

      uint8_t counter = 0;

      if (activeSuits[i] == true) {
        
        counter++;
      
        // assign half of the suits a cool colour
        if (counter <= (numberOfActiveSuits/2)) {
          states[i] = coolColour;
        }
        // assign the other half a warm colour
        else {
          states[i] = warmColour;
        }
      }
    }
  }
  
  else if (gameMode == 2) {
    
    // Traditional Tag: one person is it (warm)
    
    for (int i = 0; i < 10; i++) {
      
      uint8_t counter = 0;
      
      if (activeSuits[i] == true) {
        counter++;
        
        // the last suit is warm
        if (counter == numberOfActiveSuits) {
          states[i] = warmColour;
        }
        
        // all other suits are cool
        else {
          states[i] = coolColour;
        }
      }
    }
  }

  // START GAME OF OTHER TAG
  else if (gameMode == 3) {
  //TODO
  }
}


// ---------------------------------------------------------//
// -------- Tell each suit which colour it starts as  ------//
// ---------------------------------------------------------//
void sendStartingColours() {
  for (int i = 0; i < 10; i++) {    
    suitID = i;
    
    debugSerial.println();
    debugSerial.print("-------------- ");
    debugSerial.print("INITIALIZING SUIT  >  ");
    debugSerial.print(suitID);
    debugSerial.println("  < ------------");
    debugSerial.println();
    
    address = addresses[suitID];
    
    payload[0] = gameStartByte;
    payload[1] = states[suitID];
    
    packetSize = 2;
    
    tx = Tx16Request(address, payload, packetSize);
          
    // first attempt
    xbee.send(tx);
    confirmDelivery(gameStartByte, 1, suitID);
    
    // second attempt
    if (suitReceivedInstruction == false) {
      xbee.send(tx);
      confirmDelivery(gameStartByte, 2, suitID);
    }
    
    // third attempt
    if (suitReceivedInstruction == false) {
      xbee.send(tx);
      confirmDelivery(gameStartByte, 3, suitID);
    }
    
    // the suit got the message, do the following
    if (suitReceivedInstruction == true) {
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println("started.");
    }
    else {
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println("not started.");
    }

    // sends a colour change report from 0 - 99 to the interface
    stateReport = (suitID * 10) + (states[suitID] - 80);
    interfaceSerial.write(stateReport);
  }
}


// ---------------------------------------------------------//
// -----------   Tell each suit the game is over   ---------//
// ---------------------------------------------------------//
void sendGameOver() {
  for (int i = 0; i < 10; i++) {
    suitID = i;
    
    // only turn off the suits that are active this round
    if (activeSuits[suitID] == true) {
      
      debugSerial.println();
      debugSerial.print("---------- ");
      debugSerial.print("DEACTIVATING SUIT  >  ");
      debugSerial.print(suitID);
      debugSerial.println("  < ----------");
      debugSerial.println();
      
      address = addresses[suitID];
      payload[0] = gameOverByte;
      packetSize = 1;
      
      tx = Tx16Request(address, payload, packetSize);
      
      // check 5 times instead of 3, because game over
      // is more important than a regular message
      xbee.send(tx);
      confirmDelivery(gameOverByte, 1, suitID);
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(gameOverByte, 2, suitID);
      }
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(gameOverByte, 3, suitID);
      }
      
      if (suitReceivedInstruction == false) {
        // wait a little bit if it still hasn't got the message after 3 times
        delay(500);
        xbee.send(tx);
        confirmDelivery(gameOverByte, 4, suitID);
      }
      
      if (suitReceivedInstruction == false) {
        // wait a little bit if it still hasn't got the message after 4 times
        delay(500);
        xbee.send(tx);
        confirmDelivery(gameOverByte, 5, suitID);
      }
      
      // if it has gotten the message, set it as inactive
      if (suitReceivedInstruction == true) {
        activeSuits[suitID] = false;
      }
      else {
        // if it doesn't get the message, it's still active
        activeSuits[suitID] = true;
      }
    }
    // sends a colour change report from 0 - 99 to the interface
    stateReport = (suitID * 10) + (states[suitID] - 80);
    interfaceSerial.write(stateReport);
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
                
        gameOver();
      }
    }
    
    // Viral Tag split:
    else if (gameMode == 1) {
      // if there's only one suit left that's a different colour, check the state quicker
      if (numberOfCoolSuits == (numberOfActiveSuits - 1) || numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        
        stateReport = 105;
        sendToInterface(105);
        
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are cool colours
      if (numberOfCoolSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a cool colour.");

        stateReport = 110 + (coolColour - 80);
        sendToInterface(stateReport);
        gameOver();
      }
      
      // all the suits are warm
      else if (numberOfWarmSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a warm colour.");
        
        stateReport = 110 + (warmColour - 80);
        sendToInterface(stateReport);
        gameOver();
      }
    }
    
    // timeout check for all game modes
    if (millis() > 600000) {
      debugSerial.println();
      debugSerial.println("Game over: time limit reached.");

      stateReport = 111;
      sendToInterface(stateReport);
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
      debugSerial.print(i);
      debugSerial.print(" = ");
      debugSerial.println(states[i]);
    }
  }
}

