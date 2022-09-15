import random
import sys
import serial
import serial.tools.list_ports as list_serial_ports
import threading
import datetime
from dataRecord import DataRecord
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtGui import QPixmap
from MainWindow import Ui_MainWindow
from graphs import graph_1, graph_2, graph_3, graph_4, graph_altitude


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.show()

        # graph class
        self.graph_1 = graph_1.graph_1()
        self.graph_2 = graph_2.graph_2()
        self.graph_3 = graph_3.graph_3()
        self.graph_4 = graph_4.graph_4()

        self.graphicsView.addItem(self.graph_1)
        self.graphicsView.addItem(self.graph_2)
        self.graphicsView.nextRow()
        self.graphicsView.addItem(self.graph_3)
        self.graphicsView.addItem(self.graph_4)
        ##data
        self.log_record = DataRecord()

        self.setup_connections()
        self.refresh_serial_baud_rate()
        self.serialConnection = None
        self.serialMonitor = None
        self.dummyPort = False

        ##image
        self.camera_image_pixmap = None
        self.image_num = 1



    def setup_connections(self):
        self.connectButton.clicked.connect(self.init_serial)
        self.refreshButton.clicked.connect(self.refresh_serial_baud_rate)

        self.saveButton.clicked.connect(self.log_record.start)
        self.stopButton.clicked.connect(self.log_record.stop)

    def init_serial(self):
        if not self.dummyPort:
            print("Starting serial connection")
            port = self.comPortDropdown.currentText()
            port = (port.split(" ")[0])


            try:
                self.serialConnection = serial.Serial(
                    port=port,
                    baudrate=int(self.baudRateDropdown.currentText()),
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=0)
            except Exception as e:
                print("Error trying to connect to the specified COM port:")
                print(e)
                return
            print(self.serialConnection)
            self.check_serial_event() #thread data ack
            # self.show_camera_image() #thread for image

            self.connectButton.setText("Disconnect")
            self.connectButton.clicked.disconnect(self.init_serial)
            self.connectButton.clicked.connect(self.discon_serial)
        else:
            self.check_serial_dummy_event()

    def check_serial_dummy_event(self):
        serial_dummy_thread = threading.Timer(0.5, self.check_serial_dummy_event)
        serial_dummy_thread.daemon = True
        serial_dummy_thread.start()
        dumy_data = random.sample(range(0, 20), 4)
        self.update_plots_data(*dumy_data)
        self.log_record.save(dumy_data)

    def discon_serial(self):
        self.serialConnection.close()
        self.connectButton.setText("Connect")
        self.connectButton.clicked.disconnect(self.discon_serial)
        self.connectButton.clicked.connect(self.init_serial)

    def show_camera_image(self):
        camera_image_thread = threading.Timer(0.5, self.show_camera_image)
        camera_image_thread.daemon = True

        camera_image_thread.start()
        if self.camera_image_pixmap:
            self.imageLabel.setPixmap(self.camera_image_pixmap)

    def check_serial_event(self):
        serial_thread = threading.Timer(0.5, self.check_serial_event)
        serial_thread.daemon = True

        if self.serialConnection.is_open:
            serial_thread.start()
            if self.serialConnection.in_waiting:
                # graph and file save
                # end_of_line = b'\n'
                line = self.serialConnection.readline()
                # # print(line)
                #
                line = line.rstrip()
                data = line.decode("utf-8")
                # self.log_record.save(data)
                data = data.split(",")
                # # print(data)
                # self.update_plots_data(*data)

                #image show
                today = datetime.date.today()
                fileName = str(self.image_num) + "_" + str(today.year) + str(today.month) + str(today.day) + ".jpg"
                tmpfile = open(fileName, "wb")

                if data[0] == 'FifoLength:':
                    imgsize = int(data[1])

                elif data[0] == 'Image:':
                    for i in range(1, len(data) - 1):
                        current_byte = int(data[i]).to_bytes(1, 'big')
                        tmpfile.write(current_byte)

                    self.image_num += 1
                    tmpfile.close()
                    print(fileName + " saved")

                elif data[0] == 'Image transfer OK.':
                    fileName_pixmap = str(self.image_num - 1) + "_" + str(today.year) + str(today.month) + str(
                        today.day) + ".jpg"

                    self.camera_image_pixmap = QPixmap(fileName_pixmap)
                    self.imageLabel.setPixmap(self.camera_image_pixmap)







    def update_plots_data(self, *args):
        self.graph_1.update(args[0])
        self.graph_2.update(args[1])
        self.graph_3.update(args[2])
        self.graph_4.update(args[3])

    def refresh_serial_baud_rate(self):
        print("refresh serial")
        # add
        ports = list_serial_ports.comports(include_links=False)
        portlist = list()

        for port in ports:
            portlist.append(str(port))

        # clear
        while self.comPortDropdown.count() > 0:
            self.comPortDropdown.removeItem(0)

        self.comPortDropdown.addItems(portlist)

        while self.baudRateDropdown.count() > 0:
            self.baudRateDropdown.removeItem(0)

        baudrates = ["9600", "19200", "38400", "115200", "250000"]
        self.baudRateDropdown.addItems(baudrates)


app = QApplication(sys.argv)
w = MainWindow()
app.exec_()
