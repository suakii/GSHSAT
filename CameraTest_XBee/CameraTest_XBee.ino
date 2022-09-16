#include "./SD.h"
#include "./Camera.h"
#include "./XBee.h"

void setup()
{
	// Wire(Arduino-I2C)
	Wire.begin();
	
  //Serial.begin(57600);
  XBEE_Init();
  //Serial.println("Xbee Init done");

    //SD_Init();
	CAM_Init();
	delay(300);
}

void loop()
{
    CAM_SendPic();
    delayMicroseconds(15);

}


//ToDO
//using serial start camera send request arudion 
