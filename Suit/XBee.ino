// ---------------------------------------------------------//
// ------  Send suit_ID and tagger_ID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee(uint8_t message[]) {
  // 1 is the receiving XBee's MY16 address
  // message is the payload array of bytes to send
  
  Tx16Request tx = Tx16Request(1, message, sizeof(message));
  xbee.send(tx);
  debugSerial.print("{");
  printOutArray(message);
  debugSerial.println("} was transmitted via XBee");
}


// ---------------------------------------------------------//
// ----  Print out the values in the outgoing payload  -----//
// ---------------------------------------------------------//
void printOutArray(uint8_t message[]) {
  for(int i = 0; i < sizeof(message); i++) {
    debugSerial.print(message[i]);
    if(i != sizeof(message) - 1) debugSerial.print(", ");
  }
}


// ---------------------------------------------------------//
// ----  Look for admin messages addressed to this suit ----//
// ---------------------------------------------------------//
void lookForAdminMessage() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
    // got a rx16 packet
    
      xbee.getResponse().getRx16Response(rx16);
      
      firstByte = rx16.getData(0);

      if (firstByte == suitAdminID) {
        // this is an admin message
        // the next value will be 90 or 91,
        // so colour this suit 90 or 91
        
        initializeSuitColour(rx16.getData(1));
        
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
  if (xbee.readPacket(1000)) {
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t incoming = rx16.getData(0);

      if (incoming == suit_ID) {
        uint8_t colourChangeInstruction = rx16.getData(1);
        
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
    debugSerial.println("Packet received from a remote XBee");
    
    // should be a znet tx status
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      // get the delivery status, the fifth byte
      
      if (txStatus.getStatus() == SUCCESS) {
        // message was sent successfully
        
        debugSerial.println("Transmission was successful");
        confirmation = true;
      } else {
        // the remote XBee did not receive our packet
        debugSerial.println("XBee didn't receive packet");
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
    
    debugSerial.print("Local XBee did not provide a timely TX status response");
  }
  return confirmation;
}



