#include "./SD.h"
#include "./Camera.h"
#include "./XBee.h"

void setup()
{
    // Wire(Arduino-I2C)
	
  Wire.begin();
  Serial.begin(9600);

	XBEE_Init();

  //SD_Init();
	
  CAM_Init();
  
  while (!Serial);
  while (!XbeeSerial);
 	Serial.println("Xbee init done");

	delay(500);
}

void loop()
{
    //test

  Serial.println("Call Cam Send Pic");

  CAM_SendPic();
  
  //delayMicroseconds(15);
  delay(1000);
}
//ToDO
//using serial start camera send request arudion 
