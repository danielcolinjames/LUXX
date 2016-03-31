/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>
#include <RFIDuino.h>
#include <SoftwareSerial.h>

/*
  This example is for Series 1 XBee (802.15.4)
  Receives either a RX16 or RX64 packet and sets a PWM value based on packet data.
  Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
RFIDuino rfiduino(1.1);

// create reusable response objects for responses we expect to handle 
Rx16Response rx16 = Rx16Response();

SoftwareSerial debugSerial (10, 9); // rx, tx

uint8_t option = 0;
uint8_t data = 0;
uint8_t data1 = 0;

void setup() {
  
  // start serial
  Serial.begin(9600);
  
  debugSerial.begin(9600);
  debugSerial.println("Starting debugger from suit...");
  
  xbee.setSerial(Serial);
}

// continuously reads packets, looking for RX16
void loop() {
  
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
    
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
    	option = rx16.getOption();
    	data = rx16.getData(0);
      data1 = rx16.getData(1);
      
      digitalWrite(rfiduino.led2,HIGH);
      
      debugSerial.print("data(0) = ");
      debugSerial.println(data);
      debugSerial.print("data(1) = ");
      debugSerial.println(data1);
      
      rfiduino.successSound();
    }
  }
  else if (xbee.getResponse().isError()) {
      digitalWrite(rfiduino.led1,HIGH);
      rfiduino.errorSound();
  }
  else {
    digitalWrite(rfiduino.led2, LOW);
  }
}
