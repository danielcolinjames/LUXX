// ---------------------------------------------------------//
// -----------   Send RGB and brightness to suits ----------//
// ---------------------------------------------------------//
void sendToSuit() {
  
  payload[0] = outgoingBrightness;
  
  payload[1] = outgoingR;
  payload[2] = outgoingG;
  payload[3] = outgoingB;
  
  payload[4] = outgoingNote;
  
  packetSize = 5;
  
  tx = Tx16Request(address, payload, packetSize);
  
  xbee.send(tx);
}
