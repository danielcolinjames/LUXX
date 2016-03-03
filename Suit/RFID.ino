// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTag() {
  isThereATag();

  if (readCount == 1) {
    whoTaggedMe();
    debugSerial.print("Tag detected");
  }
}


// ---------------------------------------------------------//
// -----  Figure out if there is a tag on the reader  ------//
// ---------------------------------------------------------//
void isThereATag() {

  tagCheck = rfiduino.decodeTag(tagData);

  if (tagCheck) {
    readCount++;
  }
  else {
    readCount = 0;
  }
}


// ---------------------------------------------------------//
// --------  Print out the current tag's ID number  --------//
// ---------------------------------------------------------//
void printCurrentTag() {
  
//  if (debugging) Serial.print("RFID Tag ID:");
  
  for (int n = 0; n < 5; n++) {
//    if (debugging) Serial.print(tagData[n], DEC);
    if (n < 4) {
//      if (debugging) Serial.print(", ");
    }
  }
//  if (debugging) Serial.print("\n\r");
}



// ---------------------------------------------------------//
// --------  Figure out which suit tagged this one  --------//
// ---------------------------------------------------------//
void whoTaggedMe() {
  
  verifyKey = false;
  
  i = 0;
  
  while (i < NUMBER_OF_CARDS && verifyKey == false) {
    
    verifyKey = rfiduino.compareTagData(tagData, keyTag[i]);
    
    if (verifyKey == true) {
      
      tagger_ID = i + 1;
      
      uint8_t message[] = {
        (uint8_t)startBit, 
        (uint8_t)suit_ID, 
        (uint8_t)tagger_ID
        };
        
      sendToXBee(message);
      
      if(confirmDelivery() == true) {
        lookForInstruction();
      }
    }
    i++;
  }
}



