// ---------------------------------------------------------//
// ---------- Initial setup to assign suits colours  -------//
// ---------------------------------------------------------//
void startGame() {
  
  // START GAME OF VIRAL TAG
  if (gameMode == 1) {
    
    for (int i = 0; i < 10; i++) {

      // assign half of the suits a 90 (blue)
      if (i % 2 == 0) {
        stateArray[i] = 90;
      }

      // assign the other half a 91 (red)
      else {
        stateArray[i] = 91;
      }
    }
    initializeColours();
  }

  // START GAME OF REGULAR TAG
  else if (gameMode == 2) {
    
    for (int i = 0; i < 10; i++) {
      
      // assign all of the suits a 90 (blue) except the last one
      if (i != 9) {
        stateArray[i] = 90;
      }

      // the last one is "it," and assigned a 91 (red)
      else {
        stateArray[i] = 91;
      }
    }
    initializeColours();
  }

  // START GAME OF OTHER TAG
  else if (gameMode == 3) {
  //TODO
  }
}


// ---------------------------------------------------------//
// -------- Tell each suit which colour it starts as  ------//
// ---------------------------------------------------------//
void initializeColours() {
  for (int i = 0; i < (sizeof(stateArray)/sizeof(int)); i++) {
    // if (i == 4){} is for testing only, the suit I'm testing 
    // with is suit 5 (i + 1)
   
   Serial.print("----------------------- INITIALIZING SUIT  >  ");
   Serial.print(i + 1);
   Serial.println("  < ---------------------");
   
    suit_ID = i + 1;
    suitAdminID = suit_ID + 80;
    
    if (i == 4 || i == 5) {
      
      colourChangeInstruction = (unsigned char)stateArray[i];
      sendAdminMessage();
    }
    else {
      Serial.print("Skipping suit ");
      Serial.print(i + 1);
      Serial.println(" during testing phase.");
    }
  }
  printOutStates();
}


// ---------------------------------------------------------//
// ----------   Prints out the state of each suit  ---------//
// ---------------------------------------------------------//
void printOutStates() {;

  // sizeof is weird and needs to be converted to int first
  Serial.println();
  Serial.println("GAME STATE: ");

  for (int i = 0; i < (sizeof(stateArray)/sizeof(int)); i++) {
    Serial.print("Suit ");
    Serial.print(i + 1);
    Serial.print(" = ");
    Serial.println(stateArray[i]);
  }
  
  Serial.println();
}

