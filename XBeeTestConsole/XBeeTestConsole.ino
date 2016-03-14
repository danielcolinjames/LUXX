#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();

unsigned long start = millis();

// allocate two bytes for to hold a 10-bit analog reading
uint8_t payload[] = { 0, 0 };

// with Series 1 you can use either 16-bit or 64-bit addressing

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0x2, payload, sizeof(payload));

Rx16Response rx16 = Rx16Response();

uint8_t data = 0;
uint8_t data1 = 0;
uint8_t data2 = 0;

int suitAdminID = 85;
int colourChangeInstruction = 50;

// 64-bit addressing: This is the SH + SL address of remote XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4008b490);
// unless you have MY on the receiving radio set to FFFF, 
// this will be received as a RX16 packet
//Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

//SoftwareSerial xbeeSerial(2, 3); //rx, tx
SoftwareSerial debugSerial (10, 9);

void setup() {

  debugSerial.begin(9600);
  debugSerial.println("Starting debugger from console, waiting 5 seconds");

  //xbeeSerial.begin(9600);
  
  Serial.begin(9600);
  xbee.setSerial(Serial);

  delay(5000);
  debugSerial.println("sendAdminMessage()");
  
  sendAdminMessage();
}

void loop() {
  //sendPacket();
  //lookForPacket();
}

void lookForPacket() {
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    debugSerial.println("Packet found");
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      
      data = rx16.getData(0);
      data1 = rx16.getData(1);
      data2 = rx16.getData(2);
      
      debugSerial.print("data(0) = ");
      debugSerial.println(data);
      
      debugSerial.print("data(1) = ");
      debugSerial.println(data1);
      
      debugSerial.print("data(2) = ");
      debugSerial.println(data2);
    }
  }
  else {
    debugSerial.print("Looking for packet... millis = ");
    debugSerial.println(millis());
  }
}


void sendPacket() {
  if (millis() - start > 3000) {
    payload[0] = 99;
    payload[1] = 50;
    
    tx = Tx16Request(0x2, payload, 2);

    xbee.send(tx);
  }
  
  if (xbee.readPacket(100)) {
    
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      xbee.getResponse().getTxStatusResponse(txStatus);
      
       if (txStatus.getStatus() == SUCCESS) {

          debugSerial.println("SUCCESS");
          
       } else {
          debugSerial.println("FAILURE");
       }
    }
  } else if (xbee.getResponse().isError()) {
    debugSerial.println("ERROR");
  } else {
    debugSerial.println("TIMEOUT");
  }
  
  delay(1000);
}

void sendAdminMessage() {

  payload[0] = 85;
  payload[1] = 50;
  
  debugSerial.println("Sending admin message...");
  tx = Tx16Request(0x2, payload, 2);
  
  xbee.send(tx);

  debugSerial.println("SENDING ADMIN MESSAGE...");
    
  if (xbee.readPacket(100)) {

    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      xbee.getResponse().getTxStatusResponse(txStatus);
      
      if (txStatus.getStatus() == SUCCESS) {
        debugSerial.println("SUCCESSFUL ADMIN COMMAND");
       
      } else {
          Serial.println("FAILURE ADMIN COMMAND");
      }
    }
  } else if (xbee.getResponse().isError()) {
    Serial.println("ERROR ADMIN COMMAND");
  } else {
    Serial.println("TIMEOUT ADMIN COMMAND");
  }
}


