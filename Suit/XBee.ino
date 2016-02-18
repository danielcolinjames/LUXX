// ---------------------------------------------------------//
// ------  Send suit_ID and tagger_ID to the console  ------//
// ---------------------------------------------------------//
void sendToXBee() {

  Serial.write(99);
  Serial.write((unsigned char)suit_ID);
  Serial.write((unsigned char)tagger_ID);
}


// ---------------------------------------------------------//
// ----  Look for admin messages addressed to this suit ----//
// ---------------------------------------------------------//
void lookForAdminMessage() {
  
  boolean confirmationReceived = false;
  
  waitingToBeAddressed = true;
  receivingInstruction = true;

  if (Serial.read() == (unsigned char)suitAdminID) {
    
    while (receivingInstruction == true) {
      if (checkForTimeout()) break;
      
      Serial.write((unsigned char)suitReadyID);
      
      unsigned char instruction = Serial.read();
      
      if (instruction == (unsigned char)90 || instruction == (unsigned char)91) {
        receivingInstruction = false;
        
        while (confirmationReceived == false) {
          if (checkForTimeohut()) break;
          
          Serial.write((unsigned char)suitConfirmationID);

          if (Serial.read() == (unsigned char)77) {
            confirmationReceived = true;
          }
        }
        
        // instruction received.        
        initializeSuitColour(instruction);
      }
    }
  }
}


// ---------------------------------------------------------//
// ---------  Wait for a colour change instruction  --------//
// ---------------------------------------------------------//
void awaitInstruction() {
  
  waitingToBeAddressed = true;
  receivingInstruction = true;
  
  while (waitingToBeAddressed == true) {
    if (checkForTimeout()) break;
    
    if (Serial.read() == suit_ID) {
      waitingToBeAddressed = false;

      while (receivingInstruction == true) {
        if (checkForTimeout()) break;
        
        Serial.write(suitReadyID);
        
        unsigned char instruction = Serial.read();
        if (instruction == 50 || instruction == 55) {
          receivingInstruction = false;

          // instruction received.
          Serial.write(suitConfirmationID);
          if (instruction == 50) {
            changeSuitColour();
            rfiduino.successSound();
          }
          
          else {
            delay(250);
          }
        }
      }
    }
  }
  delay(250);
}


// ---------------------------------------------------------//
// ------------------  Check for a timeout  ----------------//
// ---------------------------------------------------------//
boolean checkForTimeout() {
  if ((millis() - timeoutMillis) > 2500) {
    timeoutMillis = 0;
    return true;
  }
  else {
    return false;
  }
}
  
