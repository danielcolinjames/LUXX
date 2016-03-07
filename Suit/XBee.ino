// ---------------------------------------------------------//
// ------  Send suitID and taggerID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee(uint8_t message[]) {
  // 1 is the receiving XBee's MY16 address
  // message is the payload array of bytes to send
  
  
  Tx16Request tx = Tx16Request(0x1, message, 3);
  xbee.send(tx);
  printOutArray(message);
}


// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
  debugSerial.print("{");
  for(int i = 0; i < sizeof(message) + 1; i++) {
    debugSerial.print(message[i]);
    if(i != sizeof(message)) debugSerial.print(", ");
  }
  debugSerial.println("} was transmitted via XBee.");
}


// ---------------------------------------------------------//
// ----  Look for admin messages addressed to this suit ----//
// ---------------------------------------------------------//
void lookForAdminMessage() {

  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    debugSerial.println("Packet found by lookForAdminMessage()");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
    // got a rx16 packet
    
      xbee.getResponse().getRx16Response(rx16);
      
      firstByte = rx16.getData(6);

      if (firstByte == suitAdminID) {
        
        debugSerial.print("Admin message found: ");
        // this is an admin message
        // the next value will be 90 or 91,
        // so colour this suit 90 or 91

        uint8_t colour = rx16.getData(7);

        debugSerial.print(colour);
        debugSerial.println(", initializing suit.");
        initializeSuitColour(colour);
        
        // TODO:
        // colour instructions (R, G, B) = getData(1, 2, 3)
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------  Look for a colour change instruction  --------//
// ---------------------------------------------------------//
void lookForInstruction() {
  
  debugSerial.println("Looking for instruction...");
  
  if (xbee.readPacket(1000)) {
    
    debugSerial.println("Packet found");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      debugSerial.print("Instruction found: ");

      for (int i = 0; i < rx16.getDataLength(); i++) {
        Serial.print(rx16.getData(i));
        Serial.print(" ");
      }
      
      uint8_t incoming = rx16.getData(6);
      
      if (incoming == suitID) {
        
        uint8_t colourChangeInstruction = rx16.getData(7);
        
        debugSerial.println(colourChangeInstruction);
        
        // TODO:
        // colourR = getData(1)
        // colourG = getData(2)
        // colourB = getData(3)
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------  Confirm message's successful delivery --------//
// ---------------------------------------------------------//
boolean confirmDelivery() {
  boolean confirmation = false;
  
  if (xbee.readPacket(1000)) {
    debugSerial.println("Packet received from a remote XBee.");
    
    debugSerial.print("millis() = ");
    debugSerial.println(millis());
    
    // should be a znet tx status
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      // get the delivery status, the fifth byte
      
      if (txStatus.getStatus() == SUCCESS) {
        // message was sent successfully
        
        debugSerial.println("Transmission was successful.");
        debugSerial.println();
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
    
    debugSerial.println("Timeout: no TX status response received.");
  }
  return confirmation;
}



