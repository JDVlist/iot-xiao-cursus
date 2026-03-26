// Updated pin definitions for XIAO ESP32 C3

const uint8_t trigPin = D2;  // Changed from D8
const uint8_t echoPin = D3;  // Changed from D7

float distance = -1.0f;
unsigned long lastDistanceUpdate = 0;
bool distanceSensorError = false;

void setupSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("[RCWL1604] Distance Sensor initialized");
}

void loopSensor() {
  // Add some delay between readings to avoid interference
  if (millis() - lastDistanceUpdate < 100) {
    return;  // Don't read too frequently
  }

  lastDistanceUpdate = millis();

  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10 microsecond pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  unsigned long duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout

  // Check for timeout or invalid reading
  if (duration == 0) {
    if (!distanceSensorError) {
      Serial.println("[RCWL1604]Distance sensor timeout - check wiring");
      distanceSensorError = true;
    }
    distance = -1.0f;
  } else {
    // Calculate distance in cm
    distance = duration * 0.0343 / 2;  // Speed of sound (343 m/s) divided by 2 (to go and back)

    // Validate distance reading (reasonable range: 2cm to 400cm)
    if (distance >= 2.0 && distance <= 400.0) {
      distanceSensorError = false;
      Serial.printf("[RCWL1604] Distance: %.1f cm\n", distance);

      const static uint datastreamId = x;

      transmitValue(distance, "cm", datastreamId);
    } else {
      if (!distanceSensorError) {
        Serial.printf("[RCWL1604] Invalid distance reading: %.1f cm\n", distance);
        distanceSensorError = true;
      }
      distance = -1.0f;
    }
  }
}
