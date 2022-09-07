import random
import sys
import serial
import serial.tools.list_ports as list_serial_ports
import threading

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow

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

        self.setup_connections()
        self.refresh_serial_baud_rate()
        self.serialConnection = None
        self.serialMonitor = None
        self.dummyPort = False

    def setup_connections(self):
        self.connectButton.clicked.connect(self.init_serial)
        self.refreshButton.clicked.connect(self.refresh_serial_baud_rate)

    def init_serial(self):
        if not self.dummyPort:
            print("Starting serial connection")
            port = self.comPortDropdown.currentText()
            port = (port.split(" ")[0])
            try:
                self.serialConnection = serial.Serial(
                    port=port,
                    baudrate=9600,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=0)
            except Exception as e:
                print("Error trying to connect to the specified COM port:")
                print(e)
                return
            print(self.serialConnection)
            self.check_serial_event()

            self.connectButton.setText("Disconnect")
            self.connectButton.clicked.disconnect(self.init_serial)
            self.connectButton.clicked.connect(self.discon_serial)
        else:
            self.check_serial_dummy_event()

    def check_serial_dummy_event(self):
        serial_thread = threading.Timer(1, self.check_serial_dummy_event)
        serial_thread.start()
        print("dummy event call")
        self.update_plots_data(*random.sample(range(0, 20), 4))

    def discon_serial(self):
        self.serialConnection.close()
        self.connectButton.setText("Connect")
        self.connectButton.clicked.disconnect(self.discon_serial)
        self.connectButton.clicked.connect(self.init_serial)

    def check_serial_event(self):
        serial_thread = threading.Timer(1, self.check_serial_event)

        if self.serialConnection.is_open:
            serial_thread.start()
            if self.serialConnection.in_waiting:
                end_of_line = b'\n'
                line = bytearray()
                line = self.serialConnection.readline()
                line = line.rstrip()
                data = line.decode("utf-8")
                data = data.split(",")
                # print(data)
                self.update_plots_data(*data)

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

        baudrates = ["9600", "19200", "38400"]
        self.baudRateDropdown.addItems(baudrates)


app = QApplication(sys.argv)
w = MainWindow()
app.exec_()
