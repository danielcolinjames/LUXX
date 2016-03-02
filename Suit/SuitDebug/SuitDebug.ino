#include <SoftwareSerial.h>
#include <XBee.h>
#include <RFIDuino.h> //include the RFIDuino Library

SoftwareSerial debugSerial(10, 11); // RX, TX
XBee xbee = XBee();
RFIDuino myRFIDuino(1.1);     //initialize an RFIDuino object for hardware version 1.1

byte tagData[5]; //Holds the ID numbers from the tag

unsigned long start = millis();

// allocate one bytes for to hold an 8-bit number
uint8_t payload[] =  {0, 1, 2};

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(1, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

void setup() {

  Serial.begin(9600);
  xbee.setSerial(Serial);

  // debug serial begin
  debugSerial.begin(9600);

  debugSerial.print("Starting debugger...");  
}

void loop() {

  if (myRFIDuino.scanForTag(tagData) == true) {
    
    debugSerial.print("Tag detected");
    
//    payload[0] = tagData[0];
    xbee.send(tx);
    
    debugSerial.println("Data sent over XBee");

    // after sending a tx request, we expect a status response
    // wait up to 2 seconds for the status response
    
    if (xbee.readPacket(2000)) {
      // got a response!
      debugSerial.println("Received response from other XBee!");
      
      // should be a znet tx status
      if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
        
        xbee.getResponse().getTxStatusResponse(txStatus);

        // get the delivery status, the fifth byte
        if (txStatus.getStatus() == SUCCESS) {
          // success.  time to celebrate
          debugSerial.println("SUCCESS");
          
          myRFIDuino.successSound();
          
        } else {
          // the remote XBee did not receive our packet. is it powered on?
          myRFIDuino.errorSound();
          debugSerial.println("XBee didn't receive packet");
        }
      }
    } else if (xbee.getResponse().isError()) {
      debugSerial.print("Error reading packet.  Error code: ");
      debugSerial.println(xbee.getResponse().getErrorCode());
      // or flash error led
    } else {
      // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
      debugSerial.print("Local XBee did not provide a timely TX status response");
      myRFIDuino.errorSound();
      delay(250);
      myRFIDuino.errorSound();
    }
    delay(1000);
  }
}
