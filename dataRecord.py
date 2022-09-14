import time
import csv


class DataRecord():
    def __init__(self):
        self.state = False

    def save(self, data):
        if self.state:
            data.append(time.asctime())
            with open("./data_log.csv", "a") as f:
                writer = csv.writer(f, delimiter=",")
                writer.writerow(data)

    def start(self):
        self.state = True
        print("starting record in csv...")

    def stop(self):
        self.state = False
        print("stopping record in csv...")



