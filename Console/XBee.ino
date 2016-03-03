// ---------------------------------------------------------//
// -------------   Look for messages from suits   ----------//
// ---------------------------------------------------------//
void lookForMessages() {

  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    
    Serial.println("Packet received...");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      // got a rx16 packet
      
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t firstByte = rx16.getData(0);
      
      if (firstByte == startBit) {
        // a suit has been tagged
        
        Serial.println();
        Serial.print("----------------------- SUIT ");
        Serial.println("TAGGED ---------------------");
        
        suit_ID = rx16.getData(1);
        Serial.print("SUIT:     >  ");
        Serial.print(suit_ID);
        
        tagger_ID = rx16.getData(2);
        Serial.print("  <     WAS TAGGED BY SUIT:     >  ");
        Serial.print(tagger_ID);
        Serial.println("  <");
        
        suitAdminID = suit_ID + 80;
        taggerAdminID = tagger_ID + 80;
        
        sendInstruction();
      }
    }
  }
}


// ---------------------------------------------------------//
// ------- Determines if the suit should change colour -----//
// ---------------------------------------------------------//
void sendInstruction() {
  if (stateArray[suit_ID - 1] != stateArray[tagger_ID - 1]) {
    colourChangeInstruction = 50;
  }
  else {
    colourChangeInstruction = 55;
  }

  if (gameMode == 1) {
    
    // send instructions to suit_ID (the one who got tagged)
    
    uint8_t payload[] = {(uint8_t)suit_ID, (uint8_t)colourChangeInstruction};
    
    tx = Tx16Request(0xFFFF, payload, sizeof(payload));
    xbee.send(tx);
    if (confirmDelivery() == true) {
      Serial.println("Instruction transmitted successfully");
      
      if (colourChangeInstruction == 50){
        // update the array if suit_ID's colour changed
        stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
      }
    }
  }
  
  else if (gameMode == 2) {
    
    // change both suit_ID and tagger_ID's colours
    
    // suit_ID first
    // ----------------
    uint8_t payload[] = {(uint8_t)suit_ID, (uint8_t)colourChangeInstruction};
    
    tx = Tx16Request(0xFFFF, payload, sizeof(payload));
    xbee.send(tx);
    printOutArray(payload);

    if (confirmDelivery() == true) {
      Serial.println("Instruction transmitted successfully");
      
      if (colourChangeInstruction == 50) {
        
        // store suit_ID's value before we change it
        tempSuitState = stateArray[suit_ID - 1];
        
        stateArray[suit_ID - 1] = stateArray[tagger_ID - 1];
      }
    
    
      // the below code is inside the if statement above because
      // if suit_ID didn't get the instruction, we don't want to
      // send it to tagger_ID either.
      
      
      // tagger_ID second
      // ----------------
      uint8_t payload[] = {(uint8_t)tagger_ID, (uint8_t)colourChangeInstruction};
      
      tx = Tx16Request(0xFFFF, payload, sizeof(payload));
      xbee.send(tx);
      printOutArray(payload);
      
      if (confirmDelivery() == true) {
        Serial.println("Instruction transmitted successfully");
        
        if (colourChangeInstruction == 50) {
          
          // assign tagger_ID suit_ID's stored value
          stateArray[tagger_ID - 1] = tempSuitState;
        }
      }
    }
  }
}


// ---------------------------------------------------------//
// -------  Confirms reception of transmitted packet  ------//
// ---------------------------------------------------------//
boolean confirmDelivery() {
  boolean confirmation = false;
  
  if (xbee.readPacket(1000)) {
    Serial.println("Packet received from a remote XBee");
    
    // should be a znet tx status
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      // get the delivery status, the fifth byte
      
      if (txStatus.getStatus() == SUCCESS) {
        // message was sent successfully
        
        // we celebrate in the console inside the code that called
        // this method because it's called in different contexts
        
        confirmation = true;
        
      } else {
        // the remote XBee did not receive our packet
        Serial.println("XBee didn't receive packet");
      }
    }
  }
  
  else if (xbee.getResponse().isError()) {
    
    Serial.print("Error reading packet.  Error code: ");
    Serial.println(xbee.getResponse().getErrorCode());
  }
  
  else {
    
    // local XBee did not provide a timely TX Status Response. 
    // Radio is not configured properly or connected.
    
    Serial.print("Local XBee did not provide a timely TX status response");
  }
  return confirmation;
}


// ---------------------------------------------------------//
// --------------- Send out an admin message  --------------//
// ---------------------------------------------------------//
void sendAdminMessage() {

  uint8_t payload[] = {(uint8_t)suitAdminID, (uint8_t)colourChangeInstruction};
  
  tx = Tx16Request(0xFFFF, payload, sizeof(payload));
  xbee.send(tx);
  printOutArray(payload);
  
  if (confirmDelivery() == true) {
    Serial.println("Admin command completed");
  }
}


// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
  Serial.print("{");
  for(int i = 0; i < sizeof(message); i++) {
    Serial.print(message[i]);
    if(i != sizeof(message) - 1) Serial.print(", ");
  }
  Serial.println("} was transmitted via XBee");
}
