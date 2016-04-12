// ---------------------------------------------------------//
// --------- Check the XBee for updates and messages -------//
// ---------------------------------------------------------//
void lookForMessages() {
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      Rx16Response rx16 = Rx16Response();
      xbee.getResponse().getRx16Response(rx16);
      
      uint8_t packetType = rx16.getData(0);
      
      if (packetType == gameStartPacket) {

        numberOfActiveSuits = 0;
        
        states[0] = rx16.getData(1);
        states[1] = rx16.getData(2);
        states[2] = rx16.getData(3);
        states[3] = rx16.getData(4);
        states[4] = rx16.getData(5);
        states[5] = rx16.getData(6);
        states[6] = rx16.getData(7);
        states[7] = rx16.getData(8);
        states[8] = rx16.getData(9);
        states[9] = rx16.getData(10);
        
        for (int i = 0; i < 10; i++) {
          
          uint8_t suitID = i;
          
          if (states[suitID] != 80) {
            activeSuits[suitID] = true;
            numberOfActiveSuits++;
          }
          else {
            activeSuits[suitID] = false;
          }
        }
        
        // segment = how many lights each suit represents
        segment = floor(NUMBER_OF_PIXELS / numberOfActiveSuits);
        
        // remainder = the pixels leftover by rounding
        remainder = NUMBER_OF_PIXELS - (segment * numberOfActiveSuits);
        
        divideColours();
        lightUp();
      }
      
      else if (packetType == tagPacket) {
        uint8_t colour = rx16.getData(1);
        uint8_t taggedSuit = rx16.getData(2);
        
        states[taggedSuit] = colour;
        
        setColour(colour);
        beep();
        
        divideColours();
        lightUp();
      }
    }
  }
}
