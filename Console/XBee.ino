// ---------------------------------------------------------//
// -------------   Look for messages from suits   ----------//
// ---------------------------------------------------------//
void lookForMessages() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    debugSerial.print("Packet found: ");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      Rx16Response rx16 = Rx16Response();
      xbee.getResponse().getRx16Response(rx16);
      
      debugSerial.print("{");
      for (int i = 0; i < rx16.getDataLength(); i++) {
        debugSerial.print(rx16.getData(i));
        debugSerial.print(" ");
        if (i != (rx16.getDataLength() - 1)) {
          debugSerial.print(", ");
        }
      }
      debugSerial.println("}");
      
      uint8_t packetType = rx16.getData(0);
      
      debugSerial.print("Packet type: ");
      debugSerial.println(packetType);
      
      if (packetType == taggedByte) {
        
        suitID = rx16.getData(1);
        taggerID = rx16.getData(2);

        debugSerial.println();
        debugSerial.print("Suit  ");
        debugSerial.print(suitID);
        
        debugSerial.print(" was tagged by suit ");
        debugSerial.print(taggerID);
        debugSerial.println(".");
        
        sendInstruction();
        printOutStates();
      }
    }
  }
}


// ---------------------------------------------------------//
// -------- Tell each suit which colour it starts as  ------//
// ---------------------------------------------------------//
void sendStartingColours() {
  for (int i = 0; i < 10; i++) {    
    suitID = i + 1;

    // in the final version, this if statement will be gone
   // if (suitID == 3) {
      debugSerial.println();
      debugSerial.print("-------------- ");
      debugSerial.print("INITIALIZING SUIT  >  ");
      debugSerial.print(suitID);
      debugSerial.println("  < ------------");
      debugSerial.println();
      
      delay(50);
      
      address = addresses[suitID - 1];
      
      payload[0] = gameStartByte;
      payload[1] = states[suitID - 1];
      
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
//    }
    
//    else {
//      debugSerial.print("----- Skipping suit ");
//      debugSerial.print(suitID);
//      debugSerial.println(" during testing phase. -----");
//    }
  }
  sendMessageToTouch(78, numberOfActiveSuits);
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
// ------- Tells suits if they should change colour --------//
// ---------------------------------------------------------//
void sendInstruction() {
  if (states[suitID - 1] == states[taggerID - 1]) {
    if (gameMode == 0 || gameMode == 1 || gameMode == 2) {
      
      // both suits are the same colour > 96
      // tell suitID not to change colour
      // taggerID doesn't know anything happened
      // so there's no need to address it at all
      
      address = addresses[suitID - 1];
      payload[0] = negativeResponseByte;
      packetSize = 1;
      
      tx = Tx16Request(address, payload, packetSize);

      // first attempt
      xbee.send(tx);
      confirmDelivery(negativeResponseByte, 1, suitID);
     
      // second attempt
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(negativeResponseByte, 2, suitID);
      }

      // third attempt
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(negativeResponseByte, 3, suitID);
      }

      // if the suit got the message, we don't need to do anything,
      // but it's good to know what the console is doing
      if (suitReceivedInstruction == true) {

        sendMessageToTouch(76, states[suitID - 1]);
        
        debugSerial.print("Suit ");
        debugSerial.print(suitID);
        debugSerial.print(" didn't change colours ");
        debugSerial.print("because it is the same colour (");
        debugSerial.print(states[suitID - 1]);
        debugSerial.print(") as suit ");
        debugSerial.print(taggerID);
        debugSerial.print(" (");
        debugSerial.print(states[taggerID - 1]);
        debugSerial.println(").");
      }
    }
  }

  // suits are different colours > 97
  else {
    if (gameMode == 0) {

      // Viral Tag Original: one person starts red, tries to make
      // everyone red. Only a red suit can tag a blue suit.
      
      if (states[suitID - 1] == 88 && states[taggerID - 1] == 89) {
        address = addresses[suitID - 1];
        payload[0] = positiveResponseByte;
        payload[1] = states[taggerID - 1];
        packetSize = 2;
        
        tx = Tx16Request(address, payload, packetSize);
        
        // first attempt
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 1, suitID);

        // second attempt
        if (suitReceivedInstruction == false) {
          xbee.send(tx);
          confirmDelivery(positiveResponseByte, 2, suitID);
        }
        
        // third attempt
        if (suitReceivedInstruction == false) {
          xbee.send(tx);
          confirmDelivery(positiveResponseByte, 3, suitID);
        }
        
        // if the message was received, do this
        if (suitReceivedInstruction == true) {

          sendMessageToTouch(77, states[suitID - 1]);
          
          debugSerial.print("Suit ");
          debugSerial.print(suitID);
          debugSerial.print(" changed from ");
          debugSerial.print(states[suitID - 1]);
          debugSerial.print(" to ");
          debugSerial.print(states[taggerID - 1]);
          debugSerial.println(".");
          
          states[suitID - 1] = states[taggerID - 1];
        }
      }
    }

    else if (gameMode == 1) {

      // Viral Tag Split: half start blue, half start red.
      // If someone tags anyone of the opposite colour,
      // the person tagged changes to the tagger's colour.
      
      address = addresses[suitID - 1];
      payload[0] = positiveResponseByte;
      payload[1] = states[taggerID - 1];
      packetSize = 2;

      tx = Tx16Request(address, payload, packetSize);
      
      xbee.send(tx);
      confirmDelivery(positiveResponseByte, 1, suitID);
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 2, suitID);
      }

      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 3, suitID);
      }

      // if the message was received, do this
      if (suitReceivedInstruction == true) {
        
        sendMessageToTouch(77, states[suitID - 1]);
        
        debugSerial.print("Suit ");
        debugSerial.print(suitID);
        debugSerial.print(" changed from ");
        debugSerial.print(states[suitID - 1]);
        debugSerial.print(" to ");
        debugSerial.print(states[taggerID - 1]);
        debugSerial.println(".");
        
        states[suitID - 1] = states[taggerID - 1];
      }
    }

    else if (gameMode == 2) {

      // Traditional Tag: both suits need to change. The
      // colour is "transferred" from the tagger to the 
      // person who was tagged.
      
      address = addresses[suitID - 1];
      payload[0] = positiveResponseByte;
      payload[1] = states[taggerID - 1];
      packetSize = 2;
      
      tx = Tx16Request(address, payload, packetSize);
      
      xbee.send(tx);
      confirmDelivery(positiveResponseByte, 1, suitID);
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 2, suitID);
      }
      
      if (suitReceivedInstruction == false) {
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 3, suitID);
      }
      
      if (suitReceivedInstruction == true) {

        // don't send anything to Touch yet,
        // wait until both suits have confirmed
        
        debugSerial.print("Suit ");
        debugSerial.print(suitID);
        debugSerial.print(" changed from ");
        debugSerial.print(states[suitID - 1]);
        debugSerial.print(" to ");
        debugSerial.print(states[taggerID - 1]);
        debugSerial.println(".");
        
        // store suitID before it's changed
        uint8_t tempSuitState = states[suitID - 1];

        // update the array to reflect the changes
        states[suitID - 1] = states[taggerID - 1];
        
        // only send to taggerID if suitID changed colour
        address = addresses[taggerID - 1];
        payload[0] = positiveResponseByte;
        payload[1] = tempSuitState;
        packetSize = 2;

        tx = Tx16Request(address, payload, packetSize);
        
        xbee.send(tx);
        confirmDelivery(positiveResponseByte, 1, taggerID);
        
        if (suitReceivedInstruction == false) {
          xbee.send(tx);
          confirmDelivery(positiveResponseByte, 2, taggerID);
        }
        
        if (suitReceivedInstruction == false) {
          xbee.send(tx);
          confirmDelivery(positiveResponseByte, 3, taggerID);
        }
        
        if (suitReceivedInstruction == true) {

          // tell Touch the colour was transferred
          sendMessageToTouch(77, 0);
          
          debugSerial.print("Suit ");
          debugSerial.print(taggerID);
          debugSerial.print(" changed from ");
          debugSerial.print(states[taggerID - 1]);
          debugSerial.print(" to ");
          debugSerial.print(tempSuitState);
          debugSerial.println(".");
          
          states[taggerID - 1] = tempSuitState;
        }
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
void confirmDelivery(uint8_t packetType, uint8_t attempt, uint8_t recepient) {
  suitReceivedInstruction = false;
  
  if (xbee.readPacket(300)) {

    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        debugSerial.print(packetType);
        debugSerial.print(" sent successfully to suit ");
        debugSerial.print(recepient);
        debugSerial.print(" on attempt ");
        debugSerial.print(attempt);
        debugSerial.println(".");
        
        suitReceivedInstruction = true;
      }
    } else {
        debugSerial.print(packetType);
        debugSerial.print(" sent unsuccessfully to suit ");
        debugSerial.print(recepient);
        debugSerial.print(" on attempt ");
        debugSerial.print(attempt);
        debugSerial.println(".");
    }
  } else if (xbee.getResponse().isError()) {
    debugSerial.println("Error reading packet: ");
    debugSerial.println(xbee.getResponse().getErrorCode());
  } else {
      debugSerial.print(packetType);
      debugSerial.print(" message to suit ");
      debugSerial.print(recepient);
      debugSerial.print(" timed out on attempt ");
      debugSerial.print(attempt);
      debugSerial.println(".");
  }
}


// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
  debugSerial.print("{");
  for(int i = 0; i < sizeof(message); i++) {
    debugSerial.print(message[i]);
    if(i != sizeof(message) - 1) {
      debugSerial.print(", ");
    }
  }
  debugSerial.println("} was transmitted via XBee.");
}
