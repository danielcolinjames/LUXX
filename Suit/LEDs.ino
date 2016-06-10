// ---------------------------------------------------------//
// ---------- Adjust the brightness of the LEDs to ---------//
// --------------- match the wearer's heartbeat ------------//
// ---------------------------------------------------------//
void adjustBrightness() {
  pixelsOne.setBrightness(incomingBrightness);
  pixelsTwo.setBrightness(incomingBrightness);
  
//  tone(rfiduino.buzzer, incomingNote);
}


// ---------------------------------------------------------//
// ------ Change the colour of the LEDs incrementally ------//
// ---------------------------------------------------------//
void changeColour() {

//  // determine red interval
//  if(rValue < incomingR) {
//    intervalR = incomingR - rValue;
//  }
//  else if(rValue > incomingR) {
//    intervalR = rValue - incomingR;
//  }
//  
//  // determine green interval
//  if(gValue < incomingG) {
//    intervalG = incomingG - gValue;
//  }
//  else if(gValue > incomingG) {
//    intervalG = gValue - incomingG;
//  }
//  
//  // determine blue interval
//  if(bValue < incomingB) {
//    intervalB = incomingB - bValue;
//  }
//  else if(bValue > incomingB) {
//    intervalB = bValue - incomingB;
//  }
//  
//  
//  // slowly adjust rValue to match
//  if (incomingR != rValue && incomingR > rValue) {
//    rValue = incomingR - (intervalR/10);
//  }
//  if (incomingR != rValue && incomingR < rValue) {
//    rValue = incomingR + (intervalR/10);
//  }
//
//  // slowly adjust gValue to match
//  if (incomingG != gValue && incomingG > gValue) {
//    gValue = incomingG - (intervalG/10);
//  }
//  if (incomingG != gValue && incomingG < gValue) {
//    gValue = incomingG + (intervalG/10);
//  }
//
//  // slowly adjust bValue to match
//  if (incomingB != bValue && incomingB > bValue) {
//    bValue = incomingB - intervalB/10;
//  }
//  if (incomingG != gValue && incomingG < gValue) {
//    gValue = incomingG + (intervalG/10);
//  }
//  
  // send the updated RGB values to the LEDs
  for (int i = 0; i < NUMPIXELSONE; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(rValue, gValue, bValue));
    pixelsOne.show();
  }
  
  for (int i = 0; i < NUMPIXELSTWO; i++) {
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(rValue, gValue, bValue));
    pixelsTwo.show();
  }
}

