#define DURATION_REST 1501.0f
#define DURATION_AMPLITUDE 600.0f
#define STEERING_PIN A1
#define THROTTLE_PIN A0
#define LED_PIN D0
#define SWITCH_PIN D1
#define BUZZER_PIN D2

// We do not need the Particle Cloud, so disable it.
SYSTEM_MODE(MANUAL);

// Type for device modes.
enum Mode {
  DRIVING = 0,
  SCAN_START,
  SCANNING,
  SCAN_STOP
};

bool modeSwitchDown = false;
Mode deviceMode = DRIVING;
uint8_t targetAddress[6] = { 0x48, 0x78, 0x63, 0xF6, 0xD2, 0xE3 };

float throttle;
float steering;

/**
 * Returns a [-1.0, 1.0] control value based on the PWM signal input on `pin`.
 */
float readChannel(int pin) {
  unsigned long duration = pulseIn(pin, HIGH);

  if (abs(duration) < 0.01f) {
    return 0.0f;
  }

  return constrain((duration - DURATION_REST) / DURATION_AMPLITUDE, -1.0f, 1.0);
}

/**
 * Switches the active mode from SCANNING to DRIVING and vice-versa. [Uses
 * transitional states, SCAN_START and SCAN_STOP.]
 */
void switch_modes() {
  if (deviceMode == DRIVING) {
    Serial.println("SCANNING");
    deviceMode = SCAN_START;
  } else {
    Serial.println("DRIVING");
    deviceMode = SCAN_STOP;
  }
}

/**
 * Handler for Bluetooth scan results.
 */
void reportCallback(advertisementReport_t *report) {
  if (memcmp(report->peerAddr, targetAddress, 6) == 0) {
    digitalWrite(BUZZER_PIN, HIGH);
  }
}

/**
 * Starts scanning for nearby Bluetooth devices in a background thread.
 */
void startBluetoothScanning() {
  ble.init();

  ble.onScanReportCallback(reportCallback);
  ble.setScanParams(0x00, 0x0060, 0x0030);
  ble.startScanning();
}

/**
 * Stops the Bluetooth scan and destroys the background thread.
 */
void stopBluetoothScanning() {
  ble.stopScanning();
  ble.deInit();
}

/**
 * Initialize internal state.
 */
void setup() {
  WiFi.off();

  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial1.begin(9600);

  modeSwitchDown = false;
}

/**
 * Run continuously.
 */
void loop() {
  if (digitalRead(SWITCH_PIN) == HIGH) {
    modeSwitchDown = true;
  } else if (modeSwitchDown) {
    modeSwitchDown = false;
    switch_modes();
  }

  switch (deviceMode) {
    case DRIVING:
      throttle = readChannel(THROTTLE_PIN);
      steering = readChannel(STEERING_PIN) * -1; // Inverted: left is -1

      Serial.printf("0:%.4f\n", throttle);
      Serial.printf("1:%.4f\n", steering);

      Serial1.printf("0:%.4f\n", throttle);
      Serial1.printf("1:%.4f\n", steering);
      break;
    case SCAN_START:
      digitalWrite(LED_PIN, HIGH);
      startBluetoothScanning();
      deviceMode = SCANNING;

      Serial1.printf("2:1\n");
      break;
    case SCAN_STOP:
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      stopBluetoothScanning();
      deviceMode = DRIVING;

      Serial1.printf("2:0\n");
      break;
  }
}
