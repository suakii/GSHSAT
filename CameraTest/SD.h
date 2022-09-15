#ifndef SD_H
#define SD_H


#include <SPI.h>
#include <SD.h>
#include<Wire.h>




// #################### SD ####################
#include "./PIN_ASSIGN.h"


void SD_Init();
void SD_Write(String str);
String SD_GetDirName();

typedef struct {
	String  logFileName;
	// char    logFileName[8];
	char    DirName[6];
	// File    logFile;
} Sd_t;


#endif