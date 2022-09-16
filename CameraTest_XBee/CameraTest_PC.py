


import time
import serial
import cv2
import datetime
# PORT = '/dev/ttyACM0' # Ubuntu
#PORT = 'COM4'         # Windows
PORT = '/dev/cu.usbmodem14601' #mac

from serial.tools import list_ports
port = list(list_ports.comports())
for p in port:
    print(p.device)


BAUD = 115200       # Change to 115200 for Due

if __name__ == '__main__':
    # Open connection to Arduino with a timeout of two seconds
    port = serial.Serial(PORT, BAUD)
    # Wait a spell
    time.sleep(1)


    done = False
    image_num = 1

    while not done:
        end_of_line = b'\n'
        line = port.readline()
        line = line.rstrip()
        data = line.decode("utf-8")
        data = data.split(",")

        today = datetime.date.today()
        fileName = str(image_num)+"_" + str(today.year) + str(today.month) + str(today.day)+".jpg"

        tmpfile = open(fileName, "wb")

        if data[0] == 'FifoLength:':
            imgsize =  int(data[1])

        elif data[0] == 'Image:':
            for i in range(1, len(data)-1):
                current_byte = int(data[i]).to_bytes(1, 'big')
                tmpfile.write(current_byte)

            image_num += 1
            tmpfile.close()
            print(fileName + " saved")

        elif data[0] == 'Image transfer OK.':
            fileName_cv = str(image_num-1) + "_" + str(today.year) + str(today.month) + str(today.day) + ".jpg"
            try:
                img = cv2.imread(fileName_cv)
                cv2.imshow("ArduCAM [ESC to quit]", img)
            except Exception as e:
                print(e)
                pass
            if cv2.waitKey(1) == 27:
                done = True
                break






        # line = line.rstrip()
        # data = line.decode("utf-8")
        # self.log_record.save(data)
        # data = data.split(",")
        #         # print(data)
        # self.update_plots_data(*data)


