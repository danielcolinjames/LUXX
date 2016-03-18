// ---------------------------------------------------------//
// ------  Send suitID and taggerID to the console  ------//
// ---------------------------------------------------------//
void sendIWasTagged() {
  
  payload[0] = taggedByte;
  payload[1] = suitID;
  payload[2] = taggerID;
  
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
    waitingForInstruction = true;
  }
}


// ---------------------------------------------------------//
// --------  Look for messages addressed to this suit ------//
// ---------------------------------------------------------//
void lookForMessages() {

  // if this suit recently sent a message to the console,
  // look for a response with instructions
  
  if (waitingForInstruction == true) {
    if (xbee.readPacket(250)) {
      if (xbee.getResponse().isAvailable()) {
        debugSerial.print("Packet found.");
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
          xbee.getResponse().getRx16Response(rx16);
          
          uint8_t packetType = rx16.getData(0);
          debugSerial.print(" Packet type = ");
          debugSerial.print(packetType);
          debugSerial.println(".");

          // if it's a 96, don't do anything (but still look
          // for game start or end messages
          if (packetType != 96) {
            // if it's a 97, change to the instruction's colour
            if (packetType == 97) {
              instruction = rx16.getData(1);
              setColour(instruction);
              debugSerial.print("Setting colour to ");
              debugSerial.println(instruction);
            }
          }
        }
      }
    }
  }

  // look for game start or end commands
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    debugSerial.print("Packet found.");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t packetType = rx16.getData(0);
      
      debugSerial.print(" Packet type = ");
      debugSerial.print(packetType);
      debugSerial.println(".");

      // game over command
      if (packetType == 95) {
        rVal = 255;
        gVal = 255;
        bVal = 255;

        digitalWrite(rfiduino.led1, LOW); // red off
        digitalWrite(rfiduino.led2, LOW); // green off

        gameOver();
      }

      // game start command, next byte in payload is
      // going to be the starting colour
      else if (packetType == 98) {
        uint8_t colour = rx16.getData(1);
        debugSerial.print("Starting colour received: ");
        debugSerial.print(colour);
        debugSerial.println(".");
        
        initializeSuitColour(colour);
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
void confirmDelivery(uint8_t packetType, uint8_t attempt) {
  messageReceived = false;
  if (xbee.readPacket(250)) {

    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        debugSerial.print(packetType);
        debugSerial.print(" sent successfully to console on attempt ");
        debugSerial.print(attempt);
        debugSerial.println(".");
        
        messageReceived = true;
        return;
      }
    } else {
        debugSerial.print(packetType);
        debugSerial.print(" sent unsuccessfully to console on attempt ");
        debugSerial.print(attempt);
        debugSerial.println(".");
        return;
    }
  } else if (xbee.getResponse().isError()) {
    debugSerial.println("Error reading packet: ");
    debugSerial.println(xbee.getResponse().getErrorCode());
    return;
  } else {
      debugSerial.print(packetType);
      debugSerial.print(" message to console on attempt ");
      debugSerial.print(attempt);
      debugSerial.println(".");
      return;
  }
}


