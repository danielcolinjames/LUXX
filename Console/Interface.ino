// ---------------------------------------------------------//
// ------ Listen for biosensor data sent through Max -------//
// ---------------------------------------------------------//
void listenForSensorData() {
  
  if (Serial.available()) {
    outgoingBrightness = Serial.read();
  }


//  For receiving multiple streams of data from Max
//  
//  int v = 0;
//  char ch = 0;
//  
//  if (Serial.available()) {
//    if (ch = Serial.read()) {
//      switch(ch) {
//        case '0' ... '9':
//          v = ch - '0' + v * 10;
//          break;
//        case 'b':
//          outgoingBrightness = v;
//          v = 0;
//          break;
//        case 'n':
//          outgoingNote = v;
//          v = 0;
//          break;
//      }
//    }
//  }


//  below is how to do it if using SoftwareSerial instead of hardware
//  and using a second Arduino to send values into this one

//  interfaceSerial.listen();
//  boolean listeningBoolean = true;
//  
//  while (interfaceSerial.available() && listeningBoolean == true) {
//    
//    incomingValue = interfaceSerial.read();
//    
//    listeningBoolean = false;
//    
//    // debugSerial.print("Reading: ");
//    // debugSerial.println(reading);
//  }
}


// ---------------------------------------------------------//
// ---- Scale the incoming values to RGB and brightness ----//
// ---------------------------------------------------------//
void mapValues() {
  
}
