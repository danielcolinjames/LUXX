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
    suitID = i + 1;
    
    debugSerial.println();
    debugSerial.print("-------------- ");
    debugSerial.print("PINGING SUIT  >  ");
    debugSerial.print(suitID);
    debugSerial.println("  < ------------");
    debugSerial.println();
        
    address = addresses[suitID - 1];
    
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
      activeSuits[suitID - 1] = true;
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is ACTIVE.");
    }
    
    else {
      activeSuits[suitID - 1] = false;
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is inactive.");
    }
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
  // add 1 to get that colour's pair
  coolColour = 80 + randomNum;
  warmColour = 80 + randomNum + 1;
  
  debugSerial.print("Cool: ");
  debugSerial.println(coolColour);
  debugSerial.print("Warm: ");
  debugSerial.println(warmColour);
  
  if (gameMode == 0) {
    
    // Viral Tag Original: one assigned warm, rest assigned cool
    for (int i = 0; i < 10; i++) {
      
      uint8_t temp = 0;
      
      if (activeSuits[i] == true) {
        
        temp++;

        // the last suit is warm
        if (temp == numberOfActiveSuits) {
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

      uint8_t temp = 0;

      if (activeSuits[i] == true) {
        temp++;
      
        // assign half of the suits a cool colour
        if (temp <= (numberOfActiveSuits/2)) {
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
      // assign all of the suits a cool colour except the first one
      if (i != 0) {
        states[i] = coolColour;
      }
      // the first one is "it," and assigned a warm colour
      else {
        states[i] = warmColour;
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
    suitID = i + 1;
    
    debugSerial.println();
    debugSerial.print("-------------- ");
    debugSerial.print("INITIALIZING SUIT  >  ");
    debugSerial.print(suitID);
    debugSerial.println("  < ------------");
    debugSerial.println();
    
    address = addresses[suitID - 1];
    
    payload[0] = gameStartByte;
    payload[1] = states[suitID - 1];
    
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
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is ACTIVE.");
    }
    else {
      debugSerial.print("Suit ");
      debugSerial.print(suitID);
      debugSerial.println(" is inactive.");
    }
  }
}


// ---------------------------------------------------------//
// -----------   Tell each suit the game is over   ---------//
// ---------------------------------------------------------//
void sendGameOver() {
  for (int i = 0; i < 10; i++) {
    suitID = i + 1;
    
    // only turn off the suits that are active this round
    if (activeSuits[suitID - 1] == true) {
      
//      if (suitID == 3) {
        debugSerial.println();
        debugSerial.print("---------- ");
        debugSerial.print("DEACTIVATING SUIT  >  ");
        debugSerial.print(suitID);
        debugSerial.println("  < ----------");
        debugSerial.println();
        
        delay(50);
        
        address = addresses[suitID - 1];
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
          activeSuits[suitID - 1] = false;
        }
        else {
          // if it doesn't get the message, it's still active
          activeSuits[suitID - 1] = true;
        }
//      }
    }
    
//    else {
//        debugSerial.print("----- Skipping deactivation of suit ");
//        debugSerial.print(suitID);
//        debugSerial.println(" during testing phase. -----");
//    }
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
        
        sendMessageToTouch(75, warmColour);
        
        gameOver();
      }
    }
    
    // Viral Tag split:
    else if (gameMode == 1) {
      // if there's only one suit left that's a different colour, check the state quicker
      if (numberOfCoolSuits == (numberOfActiveSuits - 1) || numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        stateCheckInterval = 10;
        outputInterval = 10;
      }
      else {
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are cool
      if (numberOfCoolSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a cool colour.");
        
        sendMessageToTouch(75, coolColour);
        
        gameOver();
      }
  
      // all the suits are warm
      else if (numberOfWarmSuits == numberOfActiveSuits) {
        debugSerial.println();
        debugSerial.println("Game over: everyone is a warm colour.");
        
        sendMessageToTouch(75, warmColour);
        
        gameOver();
      }
    }
    
    // timeout check for all game modes
    if (millis() > 600000) {
      debugSerial.println();
      debugSerial.println("Game over: time limit reached.");
      
      sendMessageToTouch(75, 0);
      
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
      debugSerial.print(i + 1);
      debugSerial.print(" = ");
      debugSerial.println(states[i]);
    }
  }
}

