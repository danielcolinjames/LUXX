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
  pixelsOne.setPixelColor(prevLight, pixelsOne.Color(0, 0, 0));
  pixelsOne.show();
  pixelsTwo.setPixelColor(prevLight, pixelsTwo.Color(0, 0, 0));
  pixelsTwo.show();
}


// ---------------------------------------------------------//
// --------- Turn on the next light in the sequence --------//
// ---------------------------------------------------------//
void turnOnNextLight(int currLight) {
  pixelsOne.setPixelColor(currLight, pixelsOne.Color(rVal, gVal, bVal));
  pixelsOne.show();
  pixelsTwo.setPixelColor(currLight, pixelsTwo.Color(rVal, gVal, bVal));
  pixelsTwo.show();
}


// ---------------------------------------------------------//
// ---------  Initialize suit colour at game start  --------//
// ---------------------------------------------------------//
void initializeSuitColour(int colour) {
  if (colour == 89) {
    rVal = 255;
    gVal = 0;
    bVal = 0;
    activateSuit(rVal, gVal, bVal);
    currentColour = 89;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    debugSerial.println("Starting suit as red.");
  }
  else if (colour == 88) {
    rVal = 0;
    gVal = 0;
    bVal = 255;
    activateSuit(rVal, gVal, bVal);
    currentColour = 88;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    debugSerial.println("Starting suit as blue.");
  }
}


// ---------------------------------------------------------//
// -----  Activate the suit for the start of the game  -----//
// ---------------------------------------------------------//
void activateSuit(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELS; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(r, g, b));
    pixelsOne.show();
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(r, g, b));
    pixelsTwo.show();
    tone(rfiduino.buzzer, notes[i], 50);
    delay(50);
    noTone(rfiduino.buzzer);
    delay(50);
  }
  tone(rfiduino.buzzer, notes[15], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);

  tone(rfiduino.buzzer, notes[15], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);

  tone(rfiduino.buzzer, notes[15], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);
}


// ---------------------------------------------------------//
// ------------  Change the colour of this suit  -----------//
// ---------------------------------------------------------//
void setColour(uint8_t colour) {
  if (colour == 89) {
    rVal = 255;
    gVal = 0;
    bVal = 0;
    changeColour(rVal, gVal, bVal);
    currentColour = 89;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    debugSerial.println("Colour changed to red.");
  }
  else if (colour == 88) {
    rVal = 0;
    gVal = 0;
    bVal = 255;
    changeColour(rVal, gVal, bVal);
    currentColour = 88;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    debugSerial.println("Colour changed to blue.");
  }
}


// ---------------------------------------------------------//
// -----------  Light up the LEDs on the suit  -------------//
// ---------------------------------------------------------//
void changeColour(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELS; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(r, g, b));
    pixelsOne.show();
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(r, g, b));
    pixelsTwo.show();
  }
  rfiduino.successSound();
}


// ---------------------------------------------------------//
// ------------------ Game over lights ---------------------//
// ---------------------------------------------------------//
void gameOverLights() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(rVal, gVal, bVal));
    pixelsOne.show();
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(rVal, gVal, bVal));
    pixelsTwo.show();
  }
  
  tone(rfiduino.buzzer, notes[0], 250);
  delay(250);
  noTone(rfiduino.buzzer);
  delay(250);
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(0, 0, 0));
    pixelsOne.show();
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(0, 0, 0));
    pixelsTwo.show();
  }
}

