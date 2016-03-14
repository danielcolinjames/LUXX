#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();

unsigned long start = millis();

// allocate two bytes for to hold a 10-bit analog reading
uint8_t payload[] = { 0, 0 };

// with Series 1 you can use either 16-bit or 64-bit addressing

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0x2, payload, sizeof(payload));

// 64-bit addressing: This is the SH + SL address of remote XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4008b490);
// unless you have MY on the receiving radio set to FFFF, this will be received as a RX16 packet
//Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

SoftwareSerial xbeeSerial(2, 3); //rx, tx


int pin5 = 0;

int statusLed = 11;
int errorLed = 12;

void flashLed(int pin, int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  xbeeSerial.begin(9600);
  
  Serial.begin(9600);
  xbee.setSerial(xbeeSerial);
}

void loop() {
   
    if (millis() - start > 3000) {
      
      payload[0] = 99;
      payload[1] = 50;
      
      xbee.send(tx);
    }
  
    if (xbee.readPacket(100)) {

    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getTxStatusResponse(txStatus);
    		
           if (txStatus.getStatus() == SUCCESS) {
            	// success.  time to celebrate
             	flashLed(statusLed, 5, 50);

              Serial.println("SUCCESS");
              
           } else {
             	Serial.println("FAILURE");
           }
        }
    } else if (xbee.getResponse().isError()) {
      Serial.println("ERROR");
    } else {
      Serial.println("TIMEOUT");
    }
    
    delay(1000);
}
