// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTag() {
  isThereATag();

  if (readCount == 1) {
    whoTaggedMe();
    
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
  
  // debugSerial.println("Tag detected. Looking for match...");
  
  verifyKey = false;
  
  i = 0;
  
  while (i < NUMBER_OF_CARDS && verifyKey == false) {
    
    verifyKey = rfiduino.compareTagData(tagData, keyTag[i]);
    
    if (verifyKey == true) {
      
      taggerID = i + 1;
      
      // debugSerial.print("Tag recognized. Suit: ");
      // debugSerial.println(taggerID);
      
      payload[0] = (uint8_t)startBit;
      payload[1] = (uint8_t)suitID;
      payload[2] = (uint8_t)taggerID;
      
      sendToXBee(payload, 3);

      // debugSerial.println("Completed. Returning to loop.");
    }
    i++;
  }
}



