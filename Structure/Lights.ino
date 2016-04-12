// ---------------------------------------------------------//
// -------------------- Change the colour ------------------//
// ---------------------------------------------------------//
void setColour(uint8_t colour) {
  
  // Yellow
  if (colour == 89) {
    rVal = 255;
    gVal = 189;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
  }
  
  // Blue
  else if (colour == 88) {
    rVal = 0;
    gVal = 53;
    bVal = 255;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
  }
  
  // Orange
  else if (colour == 87) {
    rVal = 255;
    gVal = 93;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
  }
  
  // Light blue
  else if (colour == 86) {
    rVal = 44;
    gVal = 255;
    bVal = 253;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
  }
  
  // Red
  else if (colour == 85) {
    rVal = 255;
    gVal = 0;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
  }
  
  // Green
  else if (colour == 84) {
    rVal = 0;
    gVal = 255;
    bVal = 0;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
  }
  
  // Pink
  else if (colour == 83) {
    rVal = 215;
    gVal = 0;
    bVal = 255;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
  }
  
  // Light green
  else if (colour == 82) {
    rVal = 141;
    gVal = 255;
    bVal = 0;
    digitalWrite(rfiduino.led1, LOW); // red off
    digitalWrite(rfiduino.led2, HIGH); // green on
  }
  
  // White
  else if (colour == 81) {
    rVal = 255;
    gVal = 255;
    bVal = 255;
    digitalWrite(rfiduino.led1, HIGH); // red on
    digitalWrite(rfiduino.led2, HIGH); // green on
  }
  
  // Black
  else if (colour == 80) {
    rVal = 0;
    gVal = 0;
    bVal = 0;
    digitalWrite(rfiduino.led2, LOW); // green off
    digitalWrite(rfiduino.led1, HIGH); // red on
  }
}


// ---------------------------------------------------------//
// ----------- Light up all the lights, light-style --------//
// ---------------------------------------------------------//
void lightUp() {
  for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(colours[i][r], colours[i][g], colours[i][b]));
  }
}


// ---------------------------------------------------------//
// -------- Beep and show the most recent tag colour -------//
// ---------------------------------------------------------//
void beep() {
  for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(rVal, gVal, bVal));
  }
  rfiduino.successSound();
  delay(1000);
}


// ---------------------------------------------------------//
// ------- Step through the lights one pixel at a time -----//
// ---------------------------------------------------------//
void stepThroughLights() {
  
  if (millis() - lightMillis > 75) {
    
    lightMillis = millis();
    
    // shift each row in colours[row][column] down
    
    for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
      
      // for every light but the first, set its colour as the previous light's
      if (i != 0) {
        uint8_t tempR = colours[i][r];
        uint8_t tempG = colours[i][g];
        uint8_t tempB = colours[i][b];
        
        colours[i + 1][r] = tempR;
        colours[i + 1][g] = tempG;
        colours[i + 1][b] = tempB;
      }
      
      // for the first light, set its colour as the last light's
      else {
        uint8_t tempR = colours[NUMBER_OF_PIXELS - 1][r];
        uint8_t tempG = colours[NUMBER_OF_PIXELS - 1][g];
        uint8_t tempB = colours[NUMBER_OF_PIXELS - 1][b];
        
        colours[i][r] = tempR;
        colours[i][g] = tempG;
        colours[i][b] = tempB;
      }
    }
  }
}


// ---------------------------------------------------------//
// -- Colour segments of the lights to represent each suit -//
// ---------------------------------------------------------//
void divideColours() {
  for (int i = 0; i < 10; i++) {
    uint8_t suitID = i;
    uint8_t counter = 0;

    if (activeSuits[suitID] = true) {
      counter++;
      for (int j = 0; j < segment; j++) {
        setColour(states[suitID]);
        colours[((segment * counter) - counter) + j][r] = rVal;
        colours[((segment * counter) - counter) + j][g] = rVal;
        colours[((segment * counter) - counter) + j][b] = rVal;
      }
    }
  }
}

