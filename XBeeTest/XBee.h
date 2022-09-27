#ifndef XBEE_H
#define XBEE_H

#include <SoftwareSerial.h>
#include "./PIN_ASSIGN.h"

SoftwareSerial XbeeSerial(PIN_XBEE_TX, PIN_XBEE_RX);

void XBEE_Init();
void XBEE_Test();

#define XBEE_Print(str)   XbeeSerial.print(str)
#define XBEE_Println(str) XbeeSerial.println(str)

#endif
