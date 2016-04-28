// ---------------------------------------------------------//
// - Handle two strands having different numbers of pixels -//
// ---------------------------------------------------------//
void stepThroughLights() {
  if (NUMPIXELSONE != NUMPIXELSTWO) {
    stepThroughLightsOne();
    stepThroughLightsTwo();
  }
  else {
    // assume both strands have 9 pixels
    
    // compare current millis() reading to the last reading.
    // if the difference is larger than the interval, we
    // need to move ahead one light.
    
    if ((millis() - prevMillis) > (450/NUMPIXELSONE)) {
      
      // assign the current value to a variable so that we
      // can compare it the next time we enter this method
      prevMillis = millis();
      
      // as long as it's not light 0, turn off the one before it
      if (loopCounter > 0) {
        turnOffPreviousLight((loopCounter - 1), 0);
      }
  
      // if we reach the last light, start back at 0 and turn
      // off the last light.
      if (loopCounter > NUMPIXELSONE - 1) {
        loopCounter = 0;
        turnOffPreviousLight((NUMPIXELSONE - 1), 0);
      }
  
      // turn on the next light in the sequence
      turnOnNextLight(loopCounter, 0);
  
      // increment loopCounter to go to the next light
      // the next time (millis() - prevMillis) > 100
      loopCounter++;
    }
  }
}


// ---------------------------------------------------------//
// -- Turn on next light, turn off previous on strand one --//
// ---------------------------------------------------------//
void stepThroughLightsOne() {

  // compare current millis() reading to the last reading.
  // if the difference is larger than the interval, we
  // need to move ahead one light.
  
  if ((millis() - prevMillisOne) > (450/NUMPIXELSONE)) {
    
    // assign the current value to a variable so that we
    // can compare it the next time we enter this method
    prevMillisOne = millis();

    // as long as it's not light 0, turn off the one before it
    if (loopCounterOne > 0) {
      turnOffPreviousLight((loopCounterOne - 1), 1);
    }

    // if we reach the last light, start back at 0 and turn
    // off the last light.
    if (loopCounterOne > NUMPIXELSONE - 1) {
      loopCounterOne = 0;
      turnOffPreviousLight((NUMPIXELSONE - 1), 1);
    }

    // turn on the next light in the sequence
    turnOnNextLight(loopCounterOne, 1);

    // increment loopCounter to go to the next light
    // the next time (millis() - prevMillis) > 100
    loopCounterOne++;
  }
}


// ---------------------------------------------------------//
// -- Turn on next light, turn off previous on strand two --//
// ---------------------------------------------------------//
void stepThroughLightsTwo() {

  // for the second strand of lights, do the same thing,
  // check to see if the current time minus the last check
  // is more than the interval between lights
  
  if ((millis() - prevMillisTwo) > (450/NUMPIXELSTWO)) {
    
    // assign the current value to a variable so that we
    // can compare it the next time we enter this method
    prevMillisTwo = millis();

    // as long as it's not light 0, turn off the one before it
    if (loopCounterTwo > 0) {
      turnOffPreviousLight((loopCounterTwo - 1), 2);
    }

    // if we reach the last light, start back at 0 and turn
    // off the last light.
    if (loopCounterTwo > NUMPIXELSTWO - 1) {
      loopCounterTwo = 0;
      turnOffPreviousLight((NUMPIXELSTWO - 1), 2);
    }

    // turn on the next light in the sequence
    turnOnNextLight(loopCounterTwo, 2);

    // increment loopCounter to go to the next light
    // the next time (millis() - prevMillis) > 100
    loopCounterTwo++;
  }
}


// ---------------------------------------------------------//
// -----------   Turn off the previous light   -------------//
// ---------------------------------------------------------//
void turnOffPreviousLight(int prevLight, int strand) {
  if (strand == 0) {
    pixelsOne.setPixelColor(prevLight, pixelsOne.Color(0, 0, 0));
    pixelsOne.show();
    pixelsTwo.setPixelColor(prevLight, pixelsTwo.Color(0, 0, 0));
    pixelsTwo.show();
  }
  else if (strand == 1) {
    pixelsOne.setPixelColor(prevLight, pixelsOne.Color(0, 0, 0));
    pixelsOne.show();
  }
  else if (strand == 2) {
    pixelsTwo.setPixelColor(prevLight, pixelsTwo.Color(0, 0, 0));
    pixelsTwo.show();
  }
}


// ---------------------------------------------------------//
// --------- Turn on the next light in the sequence --------//
// ---------------------------------------------------------//
void turnOnNextLight(int currLight, int strand) {
  if (strand == 0) {
    pixelsOne.setPixelColor(currLight, pixelsOne.Color(rVal, gVal, bVal));
    pixelsOne.show();
    pixelsTwo.setPixelColor(currLight, pixelsTwo.Color(rVal, gVal, bVal));
    pixelsTwo.show();
  }
  else if (strand == 1) {
    pixelsOne.setPixelColor(currLight, pixelsOne.Color(rVal, gVal, bVal));
    pixelsOne.show();
  }
  else if (strand == 2) {
    pixelsTwo.setPixelColor(currLight, pixelsTwo.Color(rVal, gVal, bVal));
    pixelsTwo.show();
  }
}


// ---------------------------------------------------------//
// ------------------  Set the suit's colour  --------------//
// ---------------------------------------------------------//
void setColour(int colour) {
  
  // Orange
  if (colour == 89) {
    rVal = 255;
    gVal = 150;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    // debugSerial.println("Changing suit colour to orange.");
  }
  
  // Blue
  else if (colour == 88) {
    rVal = 0;
    gVal = 53;
    bVal = 255;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    // debugSerial.println("Changing suit colour to blue.");
  }
  
  // Light orange
  else if (colour == 87) {
    rVal = 221;
    gVal = 168;
    bVal = 84;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    // debugSerial.println("Changing suit colour to light orange.");
  }
  
  // Turquoise
  else if (colour == 86) {
    rVal = 64;
    gVal = 224;
    bVal = 208;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    // debugSerial.println("Changing suit colour to turqoise.");
  }
  
  // Red
  else if (colour == 85) {
    rVal = 255;
    gVal = 0;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    // debugSerial.println("Changing suit colour to red.");
  }
  
  // Green
  else if (colour == 84) {
    rVal = 0;
    gVal = 255;
    bVal = 0;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    // debugSerial.println("Changing suit colour to green.");
  }
  
  // Pink
  else if (colour == 83) {
    rVal = 215;
    gVal = 0;
    bVal = 255;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    // debugSerial.println("Changing suit colour to pink.");
  }
  
  // Light green
  else if (colour == 82) {
    rVal = 141;
    gVal = 255;
    bVal = 0;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    // debugSerial.println("Changing suit colour to light green.");
  }
  
  // Yellow
  else if (colour == 81) {
    rVal = 255;
    gVal = 255;
    bVal = 150;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
    // debugSerial.println("Changing suit colour to yellow.");
  }
  
  // Purple
  else if (colour == 80) {
    rVal = 100;
    gVal = 0;
    bVal = 255;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
    // debugSerial.println("Changing suit colour to purple.");
  }
}


// ---------------------------------------------------------//
// -----  Activate the suit for the start of the game  -----//
// ---------------------------------------------------------//
void activateSuit(int r, int g, int b) {
  
  for (int i = 0; i < NUMPIXELSONE; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(r, g, b));
    pixelsOne.show();
    
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(r, g, b));
    pixelsTwo.show();
    
    // if one strand has 1 pixel more than the other, it has to be
    // plugged into the second NeoPixel pin
    if (i == NUMPIXELSONE && NUMPIXELSTWO != NUMPIXELSONE) {
      pixelsTwo.setPixelColor(NUMPIXELSTWO - 1, pixelsTwo.Color(r, g, b));
      pixelsTwo.show();
    }
    
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
// ------  Deactivate the suit at the end of the game  -----//
// ---------------------------------------------------------//
void deactivateSuit() {
  
  for (int i = 0; i <= NUMPIXELSONE; i++) {
    pixelsOne.setPixelColor(NUMPIXELSONE - i, pixelsOne.Color(255, 255, 255));
    pixelsOne.show();
    
    pixelsTwo.setPixelColor(NUMPIXELSTWO - i, pixelsTwo.Color(255, 255, 255));
    pixelsTwo.show();

    tone(rfiduino.buzzer, notes[15 - i], 50);
    delay(50);
    noTone(rfiduino.buzzer);
    delay(50);
  }
  tone(rfiduino.buzzer, notes[0], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);

  tone(rfiduino.buzzer, notes[0], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);

  tone(rfiduino.buzzer, notes[0], 100);
  delay(300);
  noTone(rfiduino.buzzer);
  delay(300);
}


// ---------------------------------------------------------//
// -----------  Light up the LEDs on the suit  -------------//
// ---------------------------------------------------------//
void changeColour(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELSONE; i++) {
    pixelsOne.setPixelColor(i, pixelsOne.Color(r, g, b));
    pixelsOne.show();
  }
  
  for (int i = 0; i < NUMPIXELSTWO; i++) {
    pixelsTwo.setPixelColor(i, pixelsTwo.Color(r, g, b));
    pixelsTwo.show();
  }
}


// ---------------------------------------------------------//
// ----------------------- Game over -----------------------//
// ---------------------------------------------------------//
void gameOver() {
  boolean gameRestartDetected = false;
  rVal = 255;
  gVal = 255;
  bVal = 255;
  
  while (gameRestartDetected == false) {
    stepThroughLights();
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // debugSerial.print("Packet found.");
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        xbee.getResponse().getRx16Response(rx16);
        
        uint8_t packetType = rx16.getData(0);
        
        if (packetType == 98) {
          gameRestartDetected = true;
          uint8_t colour = rx16.getData(1);
          
          setColour(colour);
          activateSuit(rVal, gVal, bVal);
        }
      }
    }
  }
}

