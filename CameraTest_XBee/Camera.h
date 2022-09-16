#ifndef CAMERA_H
#define CAMERA_H


#include "./PIN_ASSIGN.h"

#include <SPI.h>


void CAM_Init();
void CAM_TakePic();

void CAM_SendPic();

typedef struct {

}Camera_t;

#endif
