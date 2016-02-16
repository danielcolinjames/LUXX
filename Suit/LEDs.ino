// ---------------------------------------------------------//
// -----------   Turn off the previous light   -------------//
// ---------------------------------------------------------//
void turnOffPreviousLight(int prevLight) {
  pixels.setPixelColor(prevLight, pixels.Color(0, 0, 0));
  pixels.show();
}


// ---------------------------------------------------------//
// --------- Turn on the next light in the sequence --------//
// ---------------------------------------------------------//
void turnOnSingleLight(int currLight) {
  pixels.setPixelColor(currLight, pixels.Color(rVal, gVal, bVal));
  pixels.show();
}


// ---------------------------------------------------------//
// ---------  Initialize suit colour at game start  --------//
// ---------------------------------------------------------//
void initializeSuitColour(int colour) {
  
  // 90 = blue
  // 91 = red

  if (colour == 90) {
    currentColour = 90;
    rVal = 0;
    gVal = 0;
    bVal = 255;
    setSuitColour(rVal, gVal, bVal);
  }
  else if (colour == 91) {
    currentColour = 91;
    rVal = 255;
    gVal = 0;
    bVal = 0;
    setSuitColour(rVal, gVal, bVal);
  }
}


// ---------------------------------------------------------//
// -----------  Light up the LEDs on the suit  -------------//
// ---------------------------------------------------------//
void setSuitColour(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}


// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void changeSuitColour() {
  
  if (currentColour == 90) {
    rVal = 255;
    gVal = 0;
    bVal = 0;
    setSuitColour(rVal, gVal, bVal);
    currentColour = 91;
  }
  else if (currentColour == 91) {
    rVal = 0;
    gVal = 0;
    bVal = 255;
    setSuitColour(rVal, gVal, bVal);
    currentColour = 90;
  }
}


// ---------------------------------------------------------//
// ---------  For debugging with the RFIDuino LEDs  --------//
// ---------------------------------------------------------//
void debug(int flickers, int gap) {
  for (int i = 0; i < flickers; i++) {  
    digitalWrite(rfiduino.led1,HIGH);
    delay(gap);
    digitalWrite(rfiduino.led1,LOW);
    delay(gap);
  }
}
