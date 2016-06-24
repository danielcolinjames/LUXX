// ---------------------------------------------------------//
// -------------------- Start the game ---------------------//
// ---------------------------------------------------------//
void startGame() {
  // this is to avoid instant game overs.
  // this number will be subtracted from millis() later to determine
  // how many milliseconds have elapsed since the game started.
  long waitingTime = millis();
  
  pingSuits();
  assignStartingColours();
  delayForAudio();
  sendStartingColours();
  printOutStates();
}


// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void pingSuits() {
  
  numberOfActiveSuits = 0;
  
  // debugSerial.println("Pinging suits...");
  
  delay(1000);
  
  for (int i = 0; i < 10; i++) {
    
    suitID = i;
    
    address = addresses[suitID];
    
    payload[0] = pingByte;
    
    packetSize = 1;
    
    tx = Tx16Request(address, payload, packetSize);
    
    // first attempt
    xbee.send(tx);
    confirmPingDelivery();
    
    // second attempt
    if (suitReceivedPing == false) {
      xbee.send(tx);
      confirmPingDelivery();
    }
    
    // third attempt
    if (suitReceivedPing == false) {
      xbee.send(tx);
      confirmPingDelivery();
    }
    
    // fourth attempt
    if (suitReceivedPing == false) {
      xbee.send(tx);
      confirmPingDelivery();
    }
    
    if (suitReceivedPing == true) {
      
      // debugSerial.println("Suit received instruction.");
      
      numberOfActiveSuits++;
      activeSuits[suitID] = true;
      
      // tells the console that this suit is on and white
      stateReport = (130 + suitID);
      sendToInterface(stateReport);
      
      states[suitID] = 1;
      
      // debugSerial.print("Suit ");
      // debugSerial.print(suitID);
      // debugSerial.println(" is active.");
    }
    
    else {
      activeSuits[suitID] = false;
      states[suitID] = 0;
      
      // tells the console that these suits are off
      stateReport = (120 + suitID);
      sendToInterface(stateReport);
    }
    // wait a little bit between suits
    delay(500);
  }
  
  // a little longer than the max delay after a suit receives a ping
  // so suit 9 doesn't miss its initialization command
  delay(600);
}


// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void assignStartingColours() {
  // necessary for randomization in colour selection
  randomSeed(analogRead(1));
  
  // min is inclusive, max is exclusive
  // randomNum = a number from 0 - 4
  uint8_t randomNum = random(0, 5);
  
  // multiply this number by 2, to get 2, 4, 6 or 8
  randomNum *= 2;
  
  // add 80 to make it a colour code
  coolColour = colours[randomNum];
  
  // add 1 to get that colour's pair
  warmColour = colours[randomNum + 1];
  
  // debugSerial.print("Cool: ");
  // debugSerial.println(coolColour);
  // debugSerial.print("Warm: ");
  // debugSerial.println(warmColour);
  // debugSerial.println();
  
  // Viral Tag Original: one assigned warm, rest assigned cool
  if (gameMode == 0) {
    uint8_t counter = 0;
    // keep track of which suit numbers need to be assigned colours
    uint8_t suitsToAssign[10];
    uint8_t suitsAssigned[10];
    
    for (int i = 0; i < 10; i++) {
      // only assign colours to active suits
      if (activeSuits[i] == true) {
        // make an array of suit numbers that need colours
        suitsToAssign[counter] = i;
        // make an array to keep track of which ones have colours
        suitsAssigned[counter] = false;
        // counter is the number of active suits, to use in
        // the next for loops
        counter++;
      }
    }
    
    // assign all suits except one (counter - 1) a cool colour
    for (int i = 0; i < (counter - 1); i++) {
      boolean suitHasColour = false;
      // test a random suit to start
      uint8_t randomSuitPosition = random(0, counter);
      
      // keep trying random suits until we find one that hasn't
      // yet been assigned a colour
      while (suitHasColour == false) {
        // this suit has already been assigned a colour
        if (suitsAssigned[randomSuitPosition] == true) {
          // try another suit instead
          randomSuitPosition = random(0, counter);
        }
        // this suit has no colour yet
        else {
          // mark this suit as assigned
          suitsAssigned[randomSuitPosition] = true;
          // assign this suit a colour
          states[suitsToAssign[randomSuitPosition]] = coolColour;
          // end the while loop
          suitHasColour = true;
        }
      }
    }
    
    // assign the last suit (this person is "it") a warm colour
    for (int i = 0; i < counter; i++) {
      // all of them except one should have a colour: find that one
      if (suitsAssigned[i] == false) {
        // assign that one a warm colour
        states[suitsToAssign[i]] = warmColour;
      }
    }
  }
  
  // Viral Tag Split: half assigned warm, half assigned cool
  else if (gameMode == 1) {
    uint8_t counter = 0;
    uint8_t suitsToAssign[10];
    boolean suitsAssigned[10];
    
    for (int i = 0; i < 10; i++) {
      // only assign colours to active suits
      if (activeSuits[i] == true) {
        suitsToAssign[counter] = i;
        suitsAssigned[counter] = false;
        counter++;
      }
    }
    
    // assign the first half of suits a cool colour
    for (int i = 0; i < floor((double)counter/2); i++) {
      boolean suitHasColour = false;
      uint8_t randomSuitPosition = random(0, counter);
      
      while(suitHasColour == false) {
        if (suitsAssigned[randomSuitPosition] == true) {
          randomSuitPosition = random(0, counter);
        }
        else {
          suitsAssigned[randomSuitPosition] = true;
          states[suitsToAssign[randomSuitPosition]] = coolColour;
          suitHasColour = true;
        }
      }
    }
    
    // assign the second half of suits a warm colour
    for (int i = 0; i < ceil((double)counter/2); i++) {
      boolean suitHasColour = false;
      uint8_t randomSuitPosition = random(0, counter);
      
      while(suitHasColour == false) {
        if (suitsAssigned[randomSuitPosition] == true) {
          randomSuitPosition = random(0, counter);
        }
        else {
          suitsAssigned[randomSuitPosition] = true;
          states[suitsToAssign[randomSuitPosition]] = warmColour;
          suitHasColour = true;
        }
      }
    }
  }
  
  // Traditional Tag: one person is it (warm)
  else if (gameMode == 2) {
    
    uint8_t counter = 0;
    // keep track of which suit numbers need to be assigned colours
    uint8_t suitsToAssign[10];
    uint8_t suitsAssigned[10];

    for (int i = 0; i < 10; i++) {
      // only assign colours to active suits
      if (activeSuits[i] == true) {
        // make an array of suit numbers that need colours
        suitsToAssign[counter] = i;
        // make an array to keep track of which ones have colours
        suitsAssigned[counter] = false;
        // counter is the number of active suits, to use in
        // the next for loops
        counter++;
      }
    }

    // assign all suits except one (counter - 1) a cool colour
    for (int i = 0; i < (counter - 1); i++) {
      boolean suitHasColour = false;
      // test a random suit to start
      uint8_t randomSuitPosition = random(0, counter);

      // keep trying random suits until we find one that hasn't
      // yet been assigned a colour
      while (suitHasColour == false) {
        // this suit has already been assigned a colour
        if (suitsAssigned[randomSuitPosition] == true) {
          // try another suit instead
          randomSuitPosition = random(0, counter);
        }
        // this suit has no colour yet
        else {
          // mark this suit as assigned
          suitsAssigned[randomSuitPosition] = true;
          // assign this suit a colour
          states[suitsToAssign[randomSuitPosition]] = coolColour;
          // end the while loop
          suitHasColour = true;
        }
      }
    }
    // assign the last one a warm colour (TODO)
  }
  
  // Chaos Tag: everyone is a different colour
  else if (gameMode == 3) {
    
    boolean colourChosen[] = {false, false, false, false, false, 
      false, false, false, false, false };
    
    for (int i = 0; i < 10; i++) {
      if (activeSuits[i] == true) {
        boolean randomColourSelected = false;
        uint8_t randomColour = random(0, 10);
        
        while (randomColourSelected == false) {
          if (colourChosen[randomColour] == true) {
            randomColour = random(0, 10);
          }
          else {
            colourChosen[randomColour] = true;
            states[i] = colours[randomColour];
            randomColourSelected = true;
          }
        }
      }
    }
  }
}


// ---------------------------------------------------------//
// -- Wait for the audio to play before starting the game --//
// ---------------------------------------------------------//
void delayForAudio() {
  if (gameMode == 0) {
    // tell the console to start playing the appropriate audio
    stateReport = 100;
    sendToInterface(stateReport);
    // debugSerial.println("Button 0 pressed.");
    // pause for the amount of time it takes for that audio to play
    delay(30000);
  }
  else if (gameMode == 1) {
    stateReport = 101;
    sendToInterface(stateReport);
    // debugSerial.println("Button 1 pressed.");
    delay(31000);
  }
  else if (gameMode == 3) {
    stateReport = 103;
    sendToInterface(stateReport);
    // debugSerial.println("Button 2 pressed.");
    delay(32000);
  }
}


// ---------------------------------------------------------//
// -------- Tell each suit which colour it starts as  ------//
// ---------------------------------------------------------//
void sendStartingColours() {
  
  // debugSerial.println("Initializing suits...");
  
  for (int i = 0; i < 10; i++) {
    
    suitID = i;
    
    if (activeSuits[suitID] == true) {
      
      // debugSerial.print("Sending to suit ");
      // debugSerial.println(i);
      
      address = addresses[suitID];
      
      payload[0] = gameStartByte;
      payload[1] = states[suitID];
      
      packetSize = 2;
      
      tx = Tx16Request(address, payload, packetSize);
      
      // first attempt
      xbee.send(tx);
      confirmDelivery();
      
      // second attempt
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery();
      }
      
      // third attempt
      if (suitReceivedInstruction == false) {
        delay(100);
        xbee.send(tx);
        confirmDelivery();
      }
      
      // fourth attempt
      if (suitReceivedInstruction == false) {
        delay(200);
        xbee.send(tx);
        confirmDelivery();
      }
      
      // fifth attempt
      if (suitReceivedInstruction == false) {
        delay(300);
        xbee.send(tx);
        confirmDelivery();
      }
      
      // the suit got the message, do the following
      if (suitReceivedInstruction == true) {
        // debugSerial.print("Suit ");
        // debugSerial.print(suitID);
        // debugSerial.println(" started.");
      }
      
      // sends a colour change report from 0 - 99 to the interface
      stateReport = (suitID * 10) + (states[suitID] - 80);
      sendToInterface(stateReport);
    }
  }
  delay(10);
  stateReport = 199;
  sendToInterface(stateReport);
}


// ---------------------------------------------------------//
// -----------   Tell each suit the game is over   ---------//
// ---------------------------------------------------------//
void sendGameOver() {
  // debugSerial.println("Sending game over...");
  
  for (int i = 0; i < 10; i++) {
    
    suitID = i;
    
    // only turn off the suits that are active this round
    if (activeSuits[suitID] == true) {
      
      address = addresses[suitID];
      payload[0] = gameOverByte;
      packetSize = 1;
      
      tx = Tx16Request(address, payload, packetSize);
      
      // check 5 times instead of 3, because game over
      // is more important than a regular message
      xbee.send(tx);
      confirmDelivery();
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery();
      }
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery();
      }
      
      if (suitReceivedInstruction == false) {
        // wait a little bit if it still hasn't got the message after 3 times
        delay(500);
        xbee.send(tx);
        confirmDelivery();
      }
      
      if (suitReceivedInstruction == false) {
        // wait a little bit if it still hasn't got the message after 4 times
        delay(500);
        xbee.send(tx);
        confirmDelivery();
      }
      
      // if it has gotten the message, set it as inactive
      if (suitReceivedInstruction == true) {
        activeSuits[suitID] = false;
        
        // tells the interface this suit is blinking white again
        stateReport = 130 + suitID;
        sendToInterface(stateReport);
      }
      else {
        // if it doesn't get the message, it's still active
        activeSuits[suitID] = true;
      }
    }
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
   *    3 = chaos tag
   */
  
  if ((millis() - stateMillis) > stateCheckInterval) {
    
    stateMillis = millis();
    
    numberOfActiveSuits = 0;
    numberOfCoolSuits = 0;
    numberOfWarmSuits = 0;

    // game modes 0 and 1 both only have one set of colours,
    // chaos tag calculates its own stuff
    if (gameMode == 0 || gameMode == 1) {
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
    }
    
    // Viral Tag original:
    if (gameMode == 0) {
      // if there's only one suit left that's cool (uninfected), check the state quicker
      if (numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        
        lastPlayer = true;
        
        stateReport = 105;
        sendToInterface(stateReport);
        
        stateCheckInterval = 10;
        outputInterval = 1000;
      }
      else {
        if (lastPlayer == true) {
          // if we enter this else statement AND this if statement, that means
          // it was down to one player, and that player recovered, so send a 106
          // to the interface to slow the music back down again
          lastPlayer = false;
          stateReport = 106;
          sendToInterface(stateReport);
        }
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are warm (infected)
      if (numberOfWarmSuits == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is a warm colour.");
        
        stateReport = 110 + (warmColour - 80);
        sendToInterface(stateReport);
        
        gameOver();
      }
    }
    
    // Viral Tag split:
    else if (gameMode == 1) {
      // if there's only one suit left that's a different colour, check the state quicker
      if (numberOfCoolSuits == (numberOfActiveSuits - 1) || numberOfWarmSuits == (numberOfActiveSuits - 1)) {
        
        lastPlayer = true;
        
        stateReport = 105;
        sendToInterface(stateReport);
        
        stateCheckInterval = 10;
        outputInterval = 1000;
      }
      else {
        if (lastPlayer == true) {
          // if we enter this else statement AND this if statement, that means
          // it was down to one player, and that player recovered, so send a 106
          // to the interface to slow the music back down again
          lastPlayer = false;
          stateReport = 106;
          sendToInterface(stateReport);
        }
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are inactive
      if (numberOfActiveSuits == 0) {
        // debugSerial.println();
        // debugSerial.println("Game over: no suits are active.");

        stateReport = 107;
        sendToInterface(stateReport);
        waitForReset();
      }
      
      // all the suits are cool colours
      else if (numberOfCoolSuits == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is a cool colour.");
        
        stateReport = 110 + (coolColour - 80);
        sendToInterface(stateReport);
        
        delay(100);
        gameOver();
      }
      
      // all the suits are warm
      else if (numberOfWarmSuits == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is a warm colour.");
        
        stateReport = 110 + (warmColour - 80);
        sendToInterface(stateReport);
        
        delay(100);
        gameOver();
      }
    }
    
    // else if (gameMode == 2) {
    // not sure what to do for traditional tag, but it's kind of boring anyway
    
    // Chaos Tag:
    else if (gameMode == 3) {
      
      uint8_t numberOfSuits0 = 0;
      uint8_t numberOfSuits1 = 0;
      uint8_t numberOfSuits2 = 0;
      uint8_t numberOfSuits3 = 0;
      uint8_t numberOfSuits4 = 0;
      uint8_t numberOfSuits5 = 0;
      uint8_t numberOfSuits6 = 0;
      uint8_t numberOfSuits7 = 0;
      uint8_t numberOfSuits8 = 0;
      uint8_t numberOfSuits9 = 0;
      
      for (int i = 0; i < 10; i++) {
        if (activeSuits[i] == true) {
          numberOfActiveSuits++;
          
          if (states[i] == 80) {
            numberOfSuits0++;
          }
          else if (states[i] == 81) {
            numberOfSuits1++;
          }
          else if (states[i] == 82) {
            numberOfSuits2++;
          }
          else if (states[i] == 83) {
            numberOfSuits3++;
          }
          else if (states[i] == 84) {
            numberOfSuits4++;
          }
          else if (states[i] == 85) {
            numberOfSuits5++;
          }
          else if (states[i] == 86) {
            numberOfSuits6++;
          }
          else if (states[i] == 87) {
            numberOfSuits7++;
          }
          else if (states[i] == 88) {
            numberOfSuits8++;
          }
          else if (states[i] == 89) {
            numberOfSuits9++;
          }
        }
      }
      
      // if there's only one suit left that's a different colour, check the state quicker
      if (
        numberOfSuits9 == (numberOfActiveSuits - 1) ||
        numberOfSuits8 == (numberOfActiveSuits - 1) ||
        numberOfSuits7 == (numberOfActiveSuits - 1) ||
        numberOfSuits6 == (numberOfActiveSuits - 1) ||
        numberOfSuits5 == (numberOfActiveSuits - 1) ||
        numberOfSuits4 == (numberOfActiveSuits - 1) ||
        numberOfSuits3 == (numberOfActiveSuits - 1) ||
        numberOfSuits2 == (numberOfActiveSuits - 1) ||
        numberOfSuits1 == (numberOfActiveSuits - 1) ||
        numberOfSuits0 == (numberOfActiveSuits - 1)
      ) {
        
        lastPlayer = true;
        
        stateReport = 105;
        sendToInterface(105);
        
        stateCheckInterval = 10;
        outputInterval = 1000;
      }
      else {
        if (lastPlayer == true) {
          // if we enter this else statement AND this if statement, that means
          // it was down to one player, and that player recovered, so send a 106
          // to the interface to slow the music back down again
          lastPlayer = false;
          stateReport = 106;
          sendToInterface(stateReport);
        }
        stateCheckInterval = 1000;
        outputInterval = 1500;
      }
      
      // all the suits are inactive
      if (numberOfActiveSuits == 0) {
        // debugSerial.println();
        // debugSerial.println("Game over: no suits are active.");
        
        stateReport = 120;
        sendToInterface(stateReport);
        waitForReset();
      }
      
      // all the suits are orange
      else if (numberOfSuits9 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is orange.");
        
        stateReport = 119;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are blue
      else if (numberOfSuits8 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is blue.");
        
        stateReport = 118;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are light orange
      else if (numberOfSuits7 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is light orange.");
        
        stateReport = 117;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are turquoise
      else if (numberOfSuits6 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is turquoise.");
        
        stateReport = 116;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are red
      else if (numberOfSuits5 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is red.");
        
        stateReport = 115;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are green
      else if (numberOfSuits4 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is green.");
        
        stateReport = 114;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are pink
      else if (numberOfSuits3 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is pink.");
        
        stateReport = 113;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are light green
      else if (numberOfSuits2 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is light green.");
        
        stateReport = 112;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are yellow
      else if (numberOfSuits1 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is yellow.");
        
        stateReport = 111;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
      // all the suits are purple
      else if (numberOfSuits0 == numberOfActiveSuits) {
        // debugSerial.println();
        // debugSerial.println("Game over: everyone is purple.");
        
        stateReport = 110;
        sendToInterface(stateReport);
        delay(100);
        gameOver();
      }
    }
    
    // this number represents how long a game has been running
    // by subtracting the amount of time that elapsed before a
    // game was started
    gameLengthMillis = millis() - waitingTime;
    
    // timeout check for all game modes
    if (gameLengthMillis > 600000) {
      
      // debugSerial.println();
      // debugSerial.println("Game over: time limit reached.");
      
      stateReport = 108;
      sendToInterface(stateReport);
      delay(100);
      gameOver();
    }
  }
}


// ---------------------------------------------------------//
// ----- Call game over methods and reset the buttons ------//
// ---------------------------------------------------------//
void gameOver() {
  digitalWrite(gameModeZeroButtonPin, HIGH);
  digitalWrite(gameModeOneButtonPin, HIGH);
  digitalWrite(gameModeTwoButtonPin, HIGH);
  digitalWrite(gameModeThreeButtonPin, HIGH);
  digitalWrite(gameOverButtonPin, HIGH);
  
  lastPlayer = false;
  
  suitReceivedInstruction = false;
  suitReceivedPing = false;
  
  // otherwise the next game could get shut down 5 min
  // after the first one started
  gameLengthMillis = 0;
  
  sendGameOver();
  waitForReset();
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {
  
  // debugSerial.println();
  // debugSerial.print("GAME STATE: (Game Mode ");
  // debugSerial.print(gameMode);
  // debugSerial.println(")");
  
  for (int i = 0; i < 10; i++) {
    if (activeSuits[i] == true) {
      // debugSerial.print("Suit ");
      // debugSerial.print(i);
      // debugSerial.print(" = ");
      // debugSerial.println(states[i]);
    }
  }
}

