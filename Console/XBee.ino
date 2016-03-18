// ---------------------------------------------------------//
// -------------   Look for messages from suits   ----------//
// ---------------------------------------------------------//
void lookForMessages() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // debugSerial.print("Packet found: ");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      Rx16Response rx16 = Rx16Response();
      xbee.getResponse().getRx16Response(rx16);
      
      // debugSerial.print("{");
      for (int i = 0; i < rx16.getDataLength(); i++) {
        // debugSerial.print(rx16.getData(i));
        // debugSerial.print(" ");
        if (i != (rx16.getDataLength() - 1)) {
          // debugSerial.print(", ");
        }
      }
      // debugSerial.println("}");
      
      uint8_t packetType = rx16.getData(0);
      
      // debugSerial.print("Packet type: ");
      // debugSerial.println(packetType);
      
      if (packetType == taggedByte) {
        
        suitID = rx16.getData(1);
        taggerID = rx16.getData(2);
        
        // debugSerial.print("SUIT:     >  ");
        // debugSerial.print(suitID);
        
        // debugSerial.print("  <     WAS TAGGED BY SUIT:     >  ");
        // debugSerial.print(taggerID);
        // debugSerial.println("  <");

        sendInstruction();
      }
    }
  }
}


// ---------------------------------------------------------//
// ------- Tells suits if they should change colour --------//
// ---------------------------------------------------------//
void sendInstruction() {
  if (states[suitID] == states[taggerID]) {
    if (gameMode == 1 || gameMode == 2) {
      
      // both suits are the same colour > 96
      // tell suitID not to change colour
      // taggerID doesn't know anything happened
      // so there's no need to address it at all
      
      address = addresses[suitID];
      payload[0] = negativeResponseByte;
      packetSize = 1;
      
      xbee.send(tx);
      confirmDelivery(
        "Instruction 96 to suit successful.",
        "Instruction 96 to suit failed.",
        "Instruction 96 to suit timed out."
        );
      
      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 96 to suit successful on second attempt.",
          "Instruction 96 to suit failed on second attempt.",
          "Instruction 96 to suit timed out on second attempt."
          );
      }

      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 96 to suit successful on third attempt.",
          "Instruction 96 to suit failed on third attempt.",
          "Instruction 96 to suit timed out on third attempt."
          );
      }

      if (instructionReceived == true) {
        // debugSerial.print("Suit ");
        // debugSerial.print(suitID);
        // debugSerial.print(" didn't change colours ");
        // debugSerial.print("because it is the same colour (");
        // debugSerial.print(states[suitID]);
        // debugSerial.print(" as suit ");
        // debugSerial.print(taggerID);
        // debugSerial.print(" (");
        // debugSerial.print(states[taggerID]);
        // debugSerial.println(").");
      }
    }
  }

  // suits are different colours > 97
  else {
    if (gameMode == 1) {
      address = addresses[suitID];
      payload[0] = positiveResponseByte;
      payload[1] = states[taggerID];
      packetSize = 2;
      
      xbee.send(tx);
      confirmDelivery(
        "Instruction 97 to suit successful.",
        "Instruction 97 to suit failed.",
        "Instruction 97 to suit timed out."
        );
      
      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to suit successful on second attempt.",
          "Instruction 97 to suit failed on second attempt.",
          "Instruction 97 to suit timed out on second attempt."
          );
      }

      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to suit successful on third attempt.",
          "Instruction 97 to suit failed on third attempt.",
          "Instruction 97 to suit timed out on third attempt."
          );
      }

      // if the message was received, do this
      
      if (instructionReceived == true) {
        // debugSerial.print("Suit ");
        // debugSerial.print(suitID);
        // debugSerial.print(" changed from ");
        // debugSerial.print(states[suitID]);
        // debugSerial.print(" to ");
        // debugSerial.print(states[taggerID]);
        // debugSerial.println(".");
        
        states[suitID] = states[taggerID];
      }
    }
    else if (gameMode == 2) {
      address = addresses[suitID];
      payload[0] = positiveResponseByte;
      payload[1] = states[taggerID];
      packetSize = 2;
      
      xbee.send(tx);
      confirmDelivery(
        "Instruction 97 to suit successful.",
        "Instruction 97 to suit failed.",
        "Instruction 97 to suit timed out."
        );
      
      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to suit successful on second attempt.",
          "Instruction 97 to suit failed on second attempt.",
          "Instruction 97 to suit timed out on second attempt."
          );
      }

      if (instructionReceived == false) {
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to suit successful on third attempt.",
          "Instruction 97 to suit failed on third attempt.",
          "Instruction 97 to suit timed out on third attempt."
          );
      }

      if (instructionReceived == true) {

        // debugSerial.print("Suit ");
        // debugSerial.print(suitID);
        // debugSerial.print(" changed from ");
        // debugSerial.print(states[suitID]);
        // debugSerial.print(" to ");
        // debugSerial.print(states[taggerID]);
        // debugSerial.println(".");

        // update the array to reflect the changes
        // store suitID before it's changed
        
        uint8_t tempSuitState = states[suitID];
        states[suitID] = states[taggerID];
        
        // only send to taggerID if suitID changed colour
        
        address = addresses[taggerID];
        payload[0] = positiveResponseByte;
        payload[1] = tempSuitState;
        packetSize = 2;
        
        xbee.send(tx);
        confirmDelivery(
          "Instruction 97 to tagger successful.",
          "Instruction 97 to tagger failed.",
          "Instruction 97 to tagger timed out."
          );
        
        if (instructionReceived == false) {
          xbee.send(tx);
          confirmDelivery(
            "Instruction 97 to tagger successful on second attempt.",
            "Instruction 97 to tagger failed on second attempt.",
            "Instruction 97 to tagger timed out on second attempt."
            );
        }
  
        if (instructionReceived == false) {
          xbee.send(tx);
          confirmDelivery(
            "Instruction 97 to tagger successful on third attempt.",
            "Instruction 97 to tagger failed on third attempt.",
            "Instruction 97 to tagger timed out on third attempt."
            );
        }
        
        if (instructionReceived == true) {
          // debugSerial.print("Suit ");
          // debugSerial.print(taggerID);
          // debugSerial.print(" changed from ");
          // debugSerial.print(states[taggerID]);
          // debugSerial.print(" to ");
          // debugSerial.print(tempSuitState);
          // debugSerial.println(".");
          
          states[taggerID] = tempSuitState;
        }
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
void confirmDelivery(String success, String failure, String timeout) {
  instructionReceived = false;
  if (xbee.readPacket(250)) {

    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      TxStatusResponse txStatus = TxStatusResponse();
       xbee.getResponse().getTxStatusResponse(txStatus);
      
       if (txStatus.getStatus() == SUCCESS) {
          // debugSerial.println(success);
          instructionReceived = true;
          return;
       }
       
     } else {
        // debugSerial.println(failure);
        return;
     }
  } else if (xbee.getResponse().isError()) {
    // debugSerial.println("Error reading packet: ");
    // debugSerial.println(xbee.getResponse().getErrorCode());
    return;
  } else {
    // debugSerial.println(timeout);
    return;
  }
}



// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
 
  // debugSerial.print("{");
  for(int i = 0; i < sizeof(message); i++) {
    // debugSerial.print(message[i]);
    if(i != sizeof(message) - 1) {
      // debugSerial.print(", ");
    }
  }
  // debugSerial.println("} was transmitted via XBee.");
}
