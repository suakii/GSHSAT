#include "./XBee.h"


void XBEE_Init() {
	XbeeSerial.begin(9600);
	Serial.println("Xbee init done");

}
int j = 0;
void XBEE_Test() {
  double x[4];
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
  Serial.println(j++);
  if(XbeeSerial.available()) {
    Serial.println("Receive");
    Serial.println(XbeeSerial.read());
  }


}

