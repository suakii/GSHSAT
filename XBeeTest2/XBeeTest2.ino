//#include "./XBee.h"

void setup()
{
  Serial.begin(9600);
  
  Serial2.begin(9600);

	//XBEE_Init();
  
  
	delay(300);
}

//#define XBEE_Print(str)   XbeeSerial.print(str)
//#define XBEE_Println(str) XbeeSerial.println(str)

int i = 0;
double x[4];
void loop()
{

  //x[0] = 5*sin(i);
    /*
    x[0] = 10;
    
    //x[1] = 5*-sin(i);
    x[1] = -10;
    
    x[2] = 5*sin(i);
    x[3] = 5*cos(i);
    
    
    XbeeSerial.print(x[0]);
    XbeeSerial.print(",");
    XbeeSerial.print(x[1]);
    XbeeSerial.print(",");
    XbeeSerial.print(x[2]);
    XbeeSerial.print(",");
    XbeeSerial.println(x[3]);
    Serial.println("send lgo");
    if(XbeeSerial.available()) {
      Serial.println("Receive");
      Serial.println(XbeeSerial.read());

    }
    */

  x[0] = 5*sin(i);
  x[0] = 10;
    
  x[1] = -10;
    
  x[2] = 5*sin(i);
  x[3] = 5*cos(i);
    
    
  Serial2.print(x[0]);
  Serial2.print(",");
  Serial2.print(x[1]);
  Serial2.print(",");
  Serial2.print(x[2]);
  Serial2.print(",");
  Serial2.println(x[3]);
  
  Serial.println("send lgo");
  Serial.println(i);
  

//    
//    Serial.print(x[0]);
//    Serial.print(",");
//    Serial.print(x[1]);
//    Serial.print(",");
//    Serial.print(x[2]);
//    Serial.print(",");
//    Serial.println(x[3]);

    //XBEE_Test();
    i++;
    delay(1000);
}
