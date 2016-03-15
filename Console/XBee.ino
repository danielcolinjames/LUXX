// ---------------------------------------------------------//
// -------------   Look for messages from suits   ----------//
// ---------------------------------------------------------//
void lookForMessages() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    
    debugSerial.print("Packet received: ");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
//      // got a rx16 packet
      
      xbee.getResponse().getRx16Response(rx16);
      
      for (int i = 0; i < rx16.getDataLength(); i++) {
        debugSerial.print(rx16.getData(i));
        debugSerial.print(" ");
      }
      
      debugSerial.println();
      
      uint8_t firstByte = rx16.getData(0);
      
  
      debugSerial.print("First byte of incoming message: ");
      debugSerial.println(firstByte);
      
      if (firstByte == startBit) {
        // a suit has been tagged
        
        debugSerial.println();
        debugSerial.print("----------------------- SUIT ");
        debugSerial.println("TAGGED ---------------------");
        
        suitID = rx16.getData(1);
        debugSerial.print("SUIT:     >  ");
        debugSerial.print(suitID);
        
        taggerID = rx16.getData(2);
        debugSerial.print("  <     WAS TAGGED BY SUIT:     >  ");
        debugSerial.print(taggerID);
        debugSerial.println("  <");
        
        suitAdminID = suitID + 80;
        taggerAdminID = taggerID + 80;
        
        sendInstruction();
      }
    }
  }
}


// ---------------------------------------------------------//
// ------- Tells suits if they should change colour --------//
// ---------------------------------------------------------//
void sendInstruction() {
  if (stateArray[suitID - 1] != stateArray[taggerID - 1]) {
    colourChangeInstruction = 50;
  }
  else {
    colourChangeInstruction = 55;
  }

  if (gameMode == 1) {
    
    // send instructions to suitID (the one who got tagged)
    
    payload[0] = (uint8_t)suitID;
    payload[1] = (uint8_t)colourChangeInstruction;
    
    tx = Tx16Request(0x2, payload, 2);
    xbee.send(tx);
    
    debugSerial.println("Instruction transmitted.");
    confirmDelivery();
    
    if (confirmation == true) {
      debugSerial.println("Instruction transmitted successfully.");
      
      if (colourChangeInstruction == 50){
        // update the array if suitID's colour changed
        stateArray[suitID - 1] = stateArray[taggerID - 1];
      }
    }
    printOutStates();
  }
  
  else if (gameMode == 2) {

    boolean confirmed = false;
    
    // change both suitID and taggerID's colours
    
    // suitID first
    // ----------------
    payload[0] = (uint8_t)suitID;
    payload[1] = (uint8_t)colourChangeInstruction;
    
    tx = Tx16Request(0x2, payload, 2);

    xbee.send(tx);
    
    if (xbee.readPacket(250)) {

      if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
         xbee.getResponse().getTxStatusResponse(txStatus);
        
         if (txStatus.getStatus() == SUCCESS) {
            confirmed = true;
            
            debugSerial.println("SUCCESS");
            debugSerial.println("Instruction transmitted successfully to suitID");
            
            if (colourChangeInstruction == 50) {
              
              // store suitID's value before we change it
              tempSuitState = stateArray[suitID - 1];
              
              stateArray[suitID - 1] = stateArray[taggerID - 1];
            }
         } else {
            debugSerial.println("FAILURE");
         }
       }
    } else if (xbee.getResponse().isError()) {
      debugSerial.println("ERROR");
    } else {
      debugSerial.println("TIMEOUT");
    }

    if (confirmation == true) {
      payload[0] = (uint8_t)taggerID;
      payload[1] = (uint8_t)colourChangeInstruction;
      
      tx = Tx16Request(0x2, payload, 2);
      
      xbee.send(tx);
      
      if (xbee.readPacket(250)) {
        
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
          xbee.getResponse().getTxStatusResponse(txStatus);
          
          if (txStatus.getStatus() == SUCCESS) {
            debugSerial.println("SUCCESS");
            debugSerial.println("Instruction transmitted successfully to taggerID.");
      
            if (colourChangeInstruction == 50) {
                
              // assign taggerID suitID's stored value
              stateArray[taggerID - 1] = tempSuitState;
            }
          } else {
              debugSerial.println("FAILURE");
          }
        }
      } else if (xbee.getResponse().isError()) {
        debugSerial.println("ERROR");
      } else {
        debugSerial.println("TIMEOUT");
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
void confirmDelivery() {
  confirmation = false;
  
  // if (xbee.getResponse().isAvailable()) {
  
  if (xbee.readPacket(250)) {
    debugSerial.print("RECEIVING MESSAGE... millis() = ");
    debugSerial.println(millis());
    
    debugSerial.println("Packet received from a remote XBee.");
    
    // should be a znet tx status
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      debugSerial.println("Packet is a TX status response.");
      
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      // get the delivery status, the fifth byte
      if (txStatus.getStatus() == SUCCESS) {
        // message was sent successfully
        
        debugSerial.println("SUCCESS");
        confirmation = true;
        
      } else {
        // the remote XBee did not receive our packet
        debugSerial.println("XBee didn't receive packet.");
      }
    }
  }
  
  else if (xbee.getResponse().isError()) {
    
    debugSerial.print("Error reading packet.  Error code: ");
    debugSerial.println(xbee.getResponse().getErrorCode());
  }
  
  else {
    
    // local XBee did not provide a timely TX Status Response. 
    // Radio is not configured properly or connected.
    
    debugSerial.println("No TX status response received.");
  }
}


// ---------------------------------------------------------//
// --------------- Send out an admin message  --------------//
// ---------------------------------------------------------//
void sendAdminMessage() {

  payload[0] = (uint8_t)suitAdminID;
  payload[1] = (uint8_t)colourChangeInstruction;
    
  tx = Tx16Request(0x2, payload, 2);
  
  xbee.send(tx);
  
  debugSerial.println("SENDING ADMIN MESSAGE...");
    
  if (xbee.readPacket(250)) {
    
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        
        debugSerial.println("SUCCESSFUL ADMIN MESSAGE");
        
      } else {
          debugSerial.println("FAILURE");
      }
    }
  } else if (xbee.getResponse().isError()) {
    debugSerial.println("ERROR");
  } else {
    debugSerial.println("TIMEOUT");
  }
}


// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
 
  debugSerial.print("{");
  for(int i = 0; i < sizeof(message); i++) {
    debugSerial.print(message[i]);
    if(i != sizeof(message) - 1) debugSerial.print(", ");
  }
  debugSerial.println("} was transmitted via XBee.");
}
