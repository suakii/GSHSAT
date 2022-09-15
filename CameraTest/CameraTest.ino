#include "./SD.h"
#include "./Camera.h"


void setup()
{
	// Wire(Arduino-I2C)
	Wire.begin();
	Serial.begin(115200);

//	SD_Init();	
	CAM_Init();		
	Serial.println(F("Init done"));
	delay(300);
}

void loop()
{
//	CAM_TakePic();		
//	delay(3000);			
    CAM_SendPic();
    //delay(1000);
    

}


//ToDO
//using serial start camera send request arudion 
