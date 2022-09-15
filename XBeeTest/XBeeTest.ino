#include "./XBee.h"

void setup()
{
	XBEE_Init();
  
  Serial.begin(115200);
  
	delay(300);
}

#define XBEE_Print(str)   XbeeSerial.print(str)
#define XBEE_Println(str) XbeeSerial.println(str)
int i = 0;
double x[4];
void loop()
{
    //x[0] = 5*sin(i);
    x[0] = 10;
    
    //x[1] = 5*-sin(i);
    x[1] = -10;
    
    x[2] = 5*sin(i);
    x[3] = 5*cos(i);
    
//    XbeeSerial.print(x[0]);
//    XbeeSerial.print(",");
//    XbeeSerial.print(x[1]);
//    XbeeSerial.print(",");
//    XbeeSerial.print(x[2]);
//    XbeeSerial.print(",");
//    XbeeSerial.println(x[3]);
//    
    Serial.print(x[0]);
    Serial.print(",");
    Serial.print(x[1]);
    Serial.print(",");
    Serial.print(x[2]);
    Serial.print(",");
    Serial.println(x[3]);

    
    i++;
    delay(1000);
}
