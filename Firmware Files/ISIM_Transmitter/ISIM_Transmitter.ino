/*
  AUTHOR:   Fahim Faisal (embeddedfahim)
  DATE:     January 25, 2021
  LICENSE:  Apache 2.0 (Public Domain)
  CONTACT:  embeddedfahim@gmail.com
*/

#include<SPI.h> // library for Serial Peripheral Interface (SPI) communication..
#include<Wire.h> // library for Inter-Integrated Circuit (I2C) communication..
#include<Adafruit_Sensor.h> // adafruit generic sensor interfacing library..
#include<Adafruit_BME280.h> // library for the BME280 breakout board..
#include<mcp2515.h> // library for the MCP2515 CAN controller chip..

#define SEALEVELPRESSURE_HPA (1013.25) // standard atmospheric pressure at sea level..

// pins
int hall_sensor = 3;
int proximity_sensor = 4;

// global variables
float temp, pressure, alt, hum;
int hall, proximity;
struct can_frame canMsg;

// objects
Adafruit_BME280 bme;
MCP2515 mcp2515(10); // 10 is the default CS pin..

void setup() {
  // pins initialization
  pinMode(hall_sensor, INPUT);
  pinMode(proximity_sensor, INPUT);
  
  // communication protocols initialization
  Serial.begin(9600); // begin UART communication with PC at a rate of 9600 baud per second..
  SPI.begin(); // begin SPI communication..

  // modules initialization
  mcp2515.reset(); // reset mcp2515 chip for starting a new CAN session..
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ); // set CAN bus speed to 500KBPS and CAN clock to 16 MHz..
  mcp2515.setNormalMode(); // mode for both listening and talking to the CAN bus..

  if(!bme.begin(0x76)) { // default address of BME280 is 0x76..
    Serial.println("Could not find a valid BME280 sensor, check wiring!!");
    while(1); // loop forever so that the MCU resets itself at some point..
  }
}

void loop() {
  // read sensor values..
  temp = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  hum = bme.readHumidity();
  hall = digitalRead(hall_sensor);
  proximity = digitalRead(proximity_sensor);
  
  canMsg.can_id  = 0x036; // set CAN ID to 0x036..
  canMsg.can_dlc = 8; // set CAN data length to 8..
  canMsg.data[0] = temp; // store temparature reading at index 0..
  canMsg.data[1] = pressure; // store atmospheric pressure reading at index 1..
  canMsg.data[2] = alt; // store approximate altitude reading at index 2..
  canMsg.data[3] = hum; // store humidity reading at index 3..
  canMsg.data[4] = hall; // store hall sensor reading at index 4..
  canMsg.data[5] = proximity; // store proximity sensor reading at index 5..
  canMsg.data[6] = 0x00; // null value..
  canMsg.data[7] = 0x00; // null value..
  mcp2515.sendMessage(&canMsg); // send the message..
  
  delay(500); // wait for 1 second before starting over..
}
