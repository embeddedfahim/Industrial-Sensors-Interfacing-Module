/*
  AUTHOR:   Fahim Faisal (embeddedfahim)
  DATE:     January 25, 2021
  LICENSE:  Apache 2.0 (Public Domain)
  CONTACT:  embeddedfahim@gmail.com
*/

#include<SPI.h> // library for Serial Peripheral Interface (SPI) communication..
#include<mcp2515.h> // library for the MCP2515 CAN controller chip..

// global variables
float temp, pressure, alt, hum;
struct can_frame canMsg; 

// objects
MCP2515 mcp2515(10); // 10 is the default CS pin..
 
void setup() {
  // communication protocols initialization
  Serial.begin(9600); // begin UART communication with PC at a rate of 9600 baud per second..
  SPI.begin(); // begin SPI communication..

  // modules initialization
  mcp2515.reset(); // reset mcp2515 chip for starting a new CAN session..
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ); // set CAN bus speed to 500KBPS and CAN clock to 16 MHz..
  mcp2515.setNormalMode(); // mode for both listening and talking to the CAN bus..
}

void loop() {
  if(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) { // commence poll reading if there's no error..
    float temp = canMsg.data[0];
    float pressure = canMsg.data[1];
    float alt = canMsg.data[2];
    float hum = canMsg.data[3];
    int hall = canMsg.data[4];
    int proximity = canMsg.data[5];

    // write to serial monitor..
    Serial.print("Temparature: ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude: ");
    Serial.print(alt);
    Serial.println(" Meters");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println("%");
    Serial.print("Magnet: ");
    
    if(hall == 1) {
      Serial.println("DETECTED..");
    }
    else {
      Serial.println("NOT DETECTED..");
    }
    
    Serial.print("Metal: ");
    
    if(proximity == 0) {
      Serial.println("DETECTED..");
    }
    else {
      Serial.println("NOT DETECTED..");
    }
  }
}
