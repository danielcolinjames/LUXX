// ---------------------------------------------------------//
// ------  Looks for a tag, calls appropriate methods ------//
// ---------------------------------------------------------//
void lookForTags() {

  tagCheck = rfiduino.decodeTag(tagData);
  
  if (tagCheck) {
    readCount++;
  }
  else {
    readCount = 0;
  }
  
  if (readCount == 1) {
    whoTaggedMe();
  }
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
      
      taggerID = 0;
      
      // debugSerial.print("Tag recognized. Suit: ");
      // debugSerial.println(taggerID);
      
      sendIWasTagged();
    }
    i++;
  }
  
  if (verifyKey == false) {
    // debugSerial.println("No match found.");
  }
}


// ---------------------------------------------------------//
// --------  Print out the current tag's ID number  --------//
// ---------------------------------------------------------//
void printCurrentTag() {
  
  // debugSerial.print("RFID Tag ID: ");
  
  for (int n = 0; n < 5; n++) {
    // debugSerial.print(tagData[n], DEC);
    if (n < 4) {
      // debugSerial.print(", ");
    }
  }
}
