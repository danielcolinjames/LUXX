// ---------------------------------------------------------//
// -------------   Look for messages from suits   ----------//
// ---------------------------------------------------------//
void lookForMessages() {

  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    
    debugSerial.println("Packet received...");
    
//    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
//      // got a rx16 packet
      
      xbee.getResponse().getRx16Response(rx16);
      
      for (int i = 0; i < rx16.getDataLength(); i++) {
        debugSerial.print(rx16.getData(i));
        debugSerial.print(" ");
      }
      
      debugSerial.println();
      
      uint8_t firstByte = rx16.getData(6);
      

      debugSerial.print("First byte of incoming message: ");
      debugSerial.println(firstByte);
      
      if (firstByte == startBit) {
        // a suit has been tagged
        
        debugSerial.println();
        debugSerial.print("----------------------- SUIT ");
        debugSerial.println("TAGGED ---------------------");
        
        suitID = rx16.getData(7);
        debugSerial.print("SUIT:     >  ");
        debugSerial.print(suitID);
        
        taggerID = rx16.getData(8);
        debugSerial.print("  <     WAS TAGGED BY SUIT:     >  ");
        debugSerial.print(taggerID);
        debugSerial.println("  <");
        
        suitAdminID = suitID + 80;
        taggerAdminID = taggerID + 80;
        
        sendInstruction();
      }
//    }
  }
}


// ---------------------------------------------------------//
// ------- Determines if the suit should change colour -----//
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
    
    uint8_t payload[2];
    payload[0] = (uint8_t)suitID;
    payload[1] = (uint8_t)colourChangeInstruction;
    
    tx = Tx16Request(0xFFFF, payload, 2);
    xbee.send(tx);
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
    
    // change both suitID and taggerID's colours
    
    // suitID first
    // ----------------
    uint8_t payload[2];
    payload[0] = (uint8_t)suitID;
    payload[1] = (uint8_t)colourChangeInstruction;
    
    tx = Tx16Request(0xFFFF, payload, 2);
    xbee.send(tx);
    printOutArray(payload);

    confirmDelivery();
    
    if (confirmation == true) {
      debugSerial.println("Instruction transmitted successfully.");
      
      if (colourChangeInstruction == 50) {
        
        // store suitID's value before we change it
        tempSuitState = stateArray[suitID - 1];
        
        stateArray[suitID - 1] = stateArray[taggerID - 1];
      }
      
    
    
      // the below code is inside the if statement above because
      // if suitID didn't get the instruction, we don't want to
      // send it to taggerID either.
      
      
      // taggerID second
      // ----------------
      
      payload[0] = (uint8_t)taggerID;
      payload[1] = (uint8_t)colourChangeInstruction;
      
      tx = Tx16Request(0xFFFF, payload, 2);
      xbee.send(tx);
      printOutArray(payload);

      confirmDelivery();
      
      if (confirmation == true) {
        debugSerial.println("Instruction transmitted successfully.");
        
        if (colourChangeInstruction == 50) {
          
          // assign taggerID suitID's stored value
          stateArray[taggerID - 1] = tempSuitState;
        }
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

  if (xbee.readPacket(5000)) {
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
        
        txStatus = TxStatusResponse();

        debugSerial.println("After txStatus = TxStatusResponse();");
        
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

  uint8_t payload[2];
  payload[0] = (uint8_t)suitAdminID;
  payload[1] = (uint8_t)colourChangeInstruction;
  
  debugSerial.println("Sending...");
  tx = Tx16Request(0xFFFF, payload, 2);

  
  
  xbee.send(tx);

  debugSerial.print("SENDING ADMIN MESSAGE... millis() = ");
  debugSerial.println(millis());
  
  printOutArray(payload);
  
  confirmDelivery();
  if (confirmation == true) {
    debugSerial.println("Admin message delivered.");
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
