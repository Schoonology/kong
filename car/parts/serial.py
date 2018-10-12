import serial

class SerialController():
    def __init__(self):
        self.conn = serial.Serial(
                '/dev/ttyS0',
                baudrate=9600
        )
        self.recording = False
        self.mode = 'user'
        self.steering = 0.0
        self.throttle = 0.0

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

    def run_threaded(self, img_arr=None):
        self.img_arr = img_arr
        return self.steering, self.throttle, self.mode, self.recording
