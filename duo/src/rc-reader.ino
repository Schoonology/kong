#define DURATION_REST 1501.0f
#define DURATION_AMPLITUDE 600.0f
#define STEERING_PIN A1
#define THROTTLE_PIN A0

// We do not need the Particle Cloud, so disable it.
SYSTEM_MODE(MANUAL);

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
 * Initialize internal state.
 */
void setup() {
  WiFi.off();

  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

  Serial1.begin(9600);
}

/**
 * Run continuously.
 */
void loop() {
  float throttle = readChannel(THROTTLE_PIN);
  float steering = readChannel(STEERING_PIN) * -1; // Inverted: left is -1

  Serial.printf("0:%.4f\n", throttle);
  Serial.printf("1:%.4f\n", steering);

  Serial1.printf("0:%.4f\n", throttle);
  Serial1.printf("1:%.4f\n", steering);
}
