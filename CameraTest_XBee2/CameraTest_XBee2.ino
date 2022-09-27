
#include <Wire.h>
#include "ArduCAM.h"
#include <SPI.h>
#include "memorysaver.h"
#include <SoftwareSerial.h>


#define   FRAMES_NUM    0x00

const int CS = 7;
bool CAM1_EXIST = false;
bool stopMotion = false;
SoftwareSerial mySerial(2, 3); // RX, TX for Xbee
ArduCAM myCAM(OV2640, CS);
long int streamStartTime;
bool start_capture = false;

void setup() {

    uint8_t vid, pid;
    uint8_t temp;
    Wire.begin();
    Serial.begin(9600); //921600
    Serial.println(F("ArduCAM Start!"));
    mySerial.begin(9600);
    mySerial.println(F("ArduCAM Start!"));
    // set the CS output:
    pinMode(CS, OUTPUT);

    // initialize SPI:
    SPI.begin();
    //Check if the 4 ArduCAM Mini 5MP PLus Cameras' SPI bus is OK
    while (1) {
        myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
        temp = myCAM.read_reg(ARDUCHIP_TEST1);
        if (temp != 0x55) {
            Serial.println(F("SPI1 interface Error!"));
            mySerial.println(F("ACK CMD SPI interface Error! END"));

        } else {
            CAM1_EXIST = true;
            Serial.println(F("SPI1 interface OK."));
            mySerial.println(F("SPI1 interface OK."));

        }

        if (!(CAM1_EXIST)) {
            delay(1000);
            continue;
        } else
            break;
    }

    while (1) {
        //Check if the camera module type is OV2640
        myCAM.wrSensorReg8_8(0xff, 0x01);
        myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
        myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
        if ((vid != 0x26) && ((pid != 0x41) || (pid != 0x42))) {
            mySerial.println(F("ACK CMD Can't find OV2640 module! END"));
            Serial.println(F("ACK CMD Can't find OV2640 module! END"));
            delay(1000);
            continue;
        } else {
            mySerial.println(F("ACK CMD OV2640 detected. END"));
            Serial.println(F("ACK CMD OV2640 detected. END"));
            break;
        }
    }
    //Change to JPEG capture mode and initialize the OV5640 module
    myCAM.set_format(JPEG);
    myCAM.InitCAM();
    myCAM.clear_fifo_flag();
    myCAM.write_reg(ARDUCHIP_FRAMES, FRAMES_NUM);
    myCAM.OV2640_set_JPEG_size(OV2640_320x240);
    delay(1000);
    myCAM.clear_fifo_flag();
}

void loop() {
    uint8_t temp = 0xff, temp_last = 0;

    if (mySerial.available()) {
        temp = mySerial.read();
        Serial.print("Read from Xbee: ");
        Serial.println(temp);
        switch (temp)
        {
            case 1:
                start_capture = true;
                break;

            case 0:
                start_capture = false;
                break;
        }
    }
    if (CAM1_EXIST && start_capture) {
        streamStartTime = millis();
        myCAMSendToSerial(myCAM);
        double fps = ((millis() - streamStartTime) / 1000);
        Serial.println("fps: " + String(1 / fps));
        //start_capture = false;
    }
    delay(500);

}

void myCAMSendToSerial(ArduCAM myCAM) {
    char str[8];
    byte buf[5];

    static int i = 0;
    static int k = 0;
    uint8_t temp = 0, temp_last = 0;
    uint32_t length = 0;
    bool is_header = false;

    myCAM.flush_fifo(); //Flush the FIFO
    myCAM.clear_fifo_flag(); //Clear the capture done flag
    myCAM.start_capture();//Start capture

    while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

    length = myCAM.read_fifo_length();
    mySerial.print(F("FifoLength:,"));
    mySerial.print(length, DEC);
    mySerial.println(",");

    Serial.print(F("FifoLength:,"));
    Serial.print(length, DEC);
    Serial.println(",");
    
    if (length >= MAX_FIFO_SIZE) //8M
    {
        mySerial.println(F("Over size."));
        return;
    }
    if (length == 0) //0 kb
    {
        mySerial.println(F("Size is 0."));
        return;
    }

    myCAM.CS_LOW();
    myCAM.set_fifo_burst();

    mySerial.print("Image:,");
    Serial.print("Image:,");
    
        

    while (length--) {
        temp_last = temp;
        temp = SPI.transfer(0x00);
        //Read JPEG data from FIFO
        if ((temp == 0xD9) && (temp_last == 0xFF)) //If find the end ,break while,
        {
            buf[i++] = temp;  //save the last  0XD9
            //Write the remain bytes in the buffer
            myCAM.CS_HIGH();

            for (int i = 0; i < sizeof(buf); i++) {
                mySerial.print(buf[i]);
                mySerial.print(",");
                
                Serial.print(buf[i]);
                Serial.print(",");
                
                
            }

            Serial.println();
            Serial.println(F("Image transfer OK."));

            mySerial.println();
            mySerial.println(F("Image transfer OK."));

            is_header = false;
            i = 0;
        }
        if (is_header == true) {
            //Write image data to buffer if not full
            if (i < 5) {
                buf[i++] = temp;
            } else {
                //Stream 5 bytes of raw image data to serial
                myCAM.CS_HIGH();
                for (int i = 0; i < sizeof(buf); i++) {
                    mySerial.print(buf[i]);
                    mySerial.print(",");
                    
                    Serial.print(buf[i]);
                    Serial.print(",");
                    
                }
                i = 0;
                buf[i++] = temp;
                myCAM.CS_LOW();
                myCAM.set_fifo_burst();

            }
        } else if ((temp == 0xD8) & (temp_last == 0xFF)) {
            is_header = true;
            buf[i++] = temp_last;
            buf[i++] = temp;
        }

    }
}

