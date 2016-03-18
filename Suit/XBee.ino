// ---------------------------------------------------------//
// ------  Send suitID and taggerID to the console  ------//
// ---------------------------------------------------------//
void sendIWasTagged() {
  
  payload[0] = (uint8_t)taggedByte;
  payload[1] = (uint8_t)suitID;
  payload[2] = (uint8_t)taggerID;
  
  xbee.send(tx);
  debugSerial.println("Sending...");

  xbee.send(tx);
  confirmDelivery(
    "Message 99 to console successful.",
    "Message 99 to console failed.",
    "Message 99 to console timed out."
    );
  
  if (messageReceived == false) {
    xbee.send(tx);
    confirmDelivery(
      "Message 99 to console successful on second attempt.",
      "Message 99 to console failed on second attempt.",
      "Message 99 to console timed out on second attempt."
      );
  }
  
  if (messageReceived == false) {
    xbee.send(tx);
    confirmDelivery(
      "Message 99 to console successful on third attempt.",
      "Message 99 to console failed on third attempt.",
      "Message 99 to console timed out on third attempt."
      );
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
void confirmDelivery(String success, String failure, String timeout) {
  messageReceived = false;
  if (xbee.readPacket(250)) {

    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
       xbee.getResponse().getTxStatusResponse(txStatus);
      
       if (txStatus.getStatus() == SUCCESS) {
          debugSerial.println(success);
          messageReceived = true;
          return;
       }
       
     } else {
        debugSerial.println(failure);
        return;
     }
  } else if (xbee.getResponse().isError()) {
    debugSerial.println("Error reading packet: ");
    debugSerial.println(xbee.getResponse().getErrorCode());
    return;
  } else {
    debugSerial.println(timeout);
    return;
  }
}


