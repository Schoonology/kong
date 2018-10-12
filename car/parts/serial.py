import serial

class SerialController():
    def __init__(self):
        self.conn = serial.Serial(
                '/dev/ttyS0',
                baudrate=9600
        )
        self.steering = 0.0
        self.throttle = 0.0

    # Called once, in a background thread, when the car has started.
    def update(self):
        while True:
            line = self.conn.readline().strip()

            if len(line) == 0:
                continue

            try:
                [key, val] = line.split(b':')
                if key == b'0':
                    self.throttle = float(val)
                if key == b'1':
                    self.steering = float(val)
            except ValueError:
                pass

    # Called repeated in the main thread to read outputs.
    def run_threaded(self):
        return self.steering, self.throttle
