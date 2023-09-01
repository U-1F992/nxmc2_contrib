import configparser
import threading
import time

import serial

config = configparser.ConfigParser()
config.read("./config.ini")
target_port = config["Config"]["Target"]
passthrough_port = config["Config"]["PassThrough"]
baudrate = int(config["Config"]["BaudRate"])

target = serial.Serial(target_port, baudrate)
passthrough = serial.Serial(passthrough_port, baudrate)

event = threading.Event()


def from_target():
    while not event.is_set():
        data = target.read_all()
        if data is None or data == b"":
            continue
        passthrough.write(data)

        lines = [s for s in data.decode().split("\r\n") if s != ""]
        if len(lines) == 0:
            continue
        print("\n".join(["<<< " + lines[0]] + ["... " + line for line in lines[1:]]))


def to_target():
    while not event.is_set():
        data = passthrough.read_all()
        if data is None or data == b"":
            continue
        target.write(data)

        print(">>> " + " ".join(f"{b:02X}" for b in data))


threading.Thread(target=from_target).start()
threading.Thread(target=to_target).start()

try:
    while True:
        time.sleep(0.001)
except KeyboardInterrupt:
    event.set()
