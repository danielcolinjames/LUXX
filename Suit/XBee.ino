// ---------------------------------------------------------//
// ---  Wait until the console tells this suit to start  ---//
// ---------------------------------------------------------//
void waitForStartCommand() {
  boolean waiting = true;
  rVal = 255;
  gVal = 255;
  bVal = 255;
  
  activateSuit(rVal, gVal, bVal);
  
  while (waiting == true) {
    
    if (millis() - waitingForStartMillis > 3000) {
      waitingForStartMillis = millis();
      
      payload[0] = confusedByte;
      payload[1] = suitID;
      
      Tx16Request tx = Tx16Request(address, payload, 2);
      
      xbee.send(tx);
    }
    
    stepThroughLights();
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      
      // debugSerial.print("Packet found.");
      
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        xbee.getResponse().getRx16Response(rx16);
        
        uint8_t packetType = rx16.getData(0);
        
        // debugSerial.print(" Packet type = ");
        // debugSerial.print(packetType);
        // debugSerial.println(".");
        
        // ping received
        if (packetType == pingByte) {
          delay(500);
        }
        
        // game start command, next byte in payload is
        // going to be the starting colour
        else if (packetType == gameStartByte) {
          
          waiting = false;
          
          uint8_t colour = rx16.getData(1);
          // debugSerial.print("Starting colour received: ");
          // debugSerial.print(colour);
          // debugSerial.println(".");
          
          setColour(colour);
          activateSuit(rVal, gVal, bVal);
          delay(2000);
        }
        
        // manual colour change message detected
        else if (packetType == manualChangeByte) {
          instruction = rx16.getData(1);
          
          waiting = false;
          
          setColour(instruction);
          changeColour(rVal, gVal, bVal);
          
          // debugSerial.print("Setting colour to ");
          // debugSerial.println(instruction);
        }
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Send suitID and taggerID to the console  -------//
// ---------------------------------------------------------//
void sendIWasTagged() {
  
  payload[0] = taggedByte;
  payload[1] = suitID;
  payload[2] = taggerID;
  
  packetSize = 3;
  
  Tx16Request tx = Tx16Request(address, payload, packetSize);
  
  xbee.send(tx);
  confirmDelivery(taggedByte, 1);
  
  if (messageReceived == false) {
    xbee.send(tx);
    confirmDelivery(taggedByte, 2);
  }
  
  if (messageReceived == false) {
    xbee.send(tx);
    confirmDelivery(taggedByte, 3);
  }
  
  if (messageReceived == true) {
    lookForInstruction();
  }
}


// ---------------------------------------------------------//
// ----  Look for a response to "I was tagged" message  ----//
// ---------------------------------------------------------//
void lookForInstruction() {
  // if this suit recently sent a message to the console,
  // look for a response with instructions
  if (xbee.readPacket(300)) {
    
    // debugSerial.print("Instruction found.");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t packetType = rx16.getData(0);
      
      // debugSerial.print(" Packet type = ");
      // debugSerial.print(packetType);
      // debugSerial.println(".");
      
      // check if we shouldn't do anything first, in the name of efficiency
      if (packetType != negativeResponseByte) {
        
        // if it's a 97, change to the instruction's colour
        if (packetType == positiveResponseByte) {
          instruction = rx16.getData(1);
          
          setColour(instruction);
          changeColour(rVal, gVal, bVal);
          
          // debugSerial.print("Setting colour to ");
          // debugSerial.println(instruction);
          rfiduino.successSound();
          delay(1000);
        }
        
        // game over command
        else if (packetType == gameOverByte) {
          
          changeColour(rVal, gVal, bVal);
          rfiduino.successSound();
          delay(2000);

          deactivateSuit();
          
          setColour(81);
          
          digitalWrite(rfiduino.led1, LOW); // red off
          digitalWrite(rfiduino.led2, LOW); // green off
          
          gameOver();
        }
      }
      
      else {
        // if the message WAS 96
        changeColour(rVal, gVal, bVal);
        // debugSerial.println("Keeping colour the same.");
        rfiduino.errorSound();
        delay(500);
      }
    }
  }
}


// ---------------------------------------------------------//
// --------  Look for messages addressed to this suit ------//
// ---------------------------------------------------------//
void lookForMessages() {
  
  // look for game over or manual colour assignment
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // debugSerial.print("Packet found.");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t packetType = rx16.getData(0);
      
      // debugSerial.print(" Packet type = ");
      // debugSerial.print(packetType);
      // debugSerial.println(".");
      
      // game over
      if (packetType == gameOverByte) {
        
        changeColour(rVal, gVal, bVal);
        rfiduino.successSound();
        delay(2000);
        
        deactivateSuit();
        
        setColour(81);
        
        digitalWrite(rfiduino.led1, LOW); // red off
        digitalWrite(rfiduino.led2, LOW); // green off
        
        gameOver();
      }
      
      else if (packetType == manualChangeByte) {
        instruction = rx16.getData(1);
        
        setColour(instruction);
        changeColour(rVal, gVal, bVal);
        
        // debugSerial.print("Setting colour to ");
        // debugSerial.println(instruction);
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
void confirmDelivery(uint8_t packetType, uint8_t attempt) {
  messageReceived = false;
  if (xbee.readPacket(300)) {
    
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        // debugSerial.print(packetType);
        // debugSerial.print(" sent successfully to console on attempt ");
        // debugSerial.print(attempt);
        // debugSerial.println(".");
        
        messageReceived = true;
      }
    } else {
        // debugSerial.print(packetType);
        // debugSerial.print(" sent unsuccessfully to console on attempt ");
        // debugSerial.print(attempt);
        // debugSerial.println(".");
    }
  } else if (xbee.getResponse().isError()) {
    // debugSerial.println("Error reading packet: ");
    // debugSerial.println(xbee.getResponse().getErrorCode());
  } else {
      // debugSerial.print(packetType);
      // debugSerial.print(" message to console timed out on attempt ");
      // debugSerial.print(attempt);
      // debugSerial.println(".");
  }
}



// ---------------------------------------------------------//
// ---- Confirms reception of transmitted ping response ----//
// ---------------------------------------------------------//
void confirmPingDelivery() {
  pingReceived = false;
  if (xbee.readPacket(500)) {
    
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        pingReceived = true;
      }
    }
  }
}


