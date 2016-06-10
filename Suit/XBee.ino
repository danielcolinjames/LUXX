// ---------------------------------------------------------//
// --------  Look for messages addressed to this suit ------//
// ---------------------------------------------------------//
void lookForMessages() {
  
  // look for incoming brightness, RGB, and note values
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // debugSerial.print("Packet found.");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      incomingBrightness = rx16.getData(0);
      
      incomingR = rx16.getData(1);
      incomingG = rx16.getData(2);
      incomingB = rx16.getData(3);
      
      incomingNote = rx16.getData(4);
      
      changeColour();
      
      digitalWrite(rfiduino.led1, LOW); // red off
      digitalWrite(rfiduino.led2, LOW); // green off
    }
  }
}

