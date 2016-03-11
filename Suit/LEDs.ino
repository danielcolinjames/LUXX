// ---------------------------------------------------------//
// --------  Turn off last light, turn on next one  --------//
// ---------------------------------------------------------//
void stepThroughLights() {

  // compare current millis() reading to the last reading.
  // if the difference is larger than 100 milliseconds, we
  // need to move ahead one light.
  
  if ((millis() - prevMillis) > 50) {
    
    // assign the current value to a variable so that we
    // can compare it the next time we enter this method
    prevMillis = millis();

    // as long as it's not light 0, turn off the one before it
    if (loopCounter > 0) {
      turnOffPreviousLight(loopCounter - 1);
    }

    // if we reach the last light, start back at 0 and turn
    // off the last light.
    if (loopCounter > 15) {
      loopCounter = 0;
      turnOffPreviousLight(15);
    }

    // turn on the next light in the sequence
    turnOnNextLight(loopCounter);

    // increment loopCounter to go to the next light
    // the next time (millis() - prevMillis) > 100
    loopCounter++;
  }
}


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
void turnOnNextLight(int currLight) {
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
// ---  For debugging with the RFIDuino's built-in LEDs  ---//
// ---------------------------------------------------------//

void debug(int flickers, int gap) {
//  for (int i = 0; i < flickers; i++) {
//    if (millis() - lightMillis > gap) {
//      prevMillis = 0;
//      digitalWrite(rfiduino.led1,HIGH);
//    }
//    else {
//      digitalWrite(rfiduino.led1,LOW);
//    }
//  }
}


