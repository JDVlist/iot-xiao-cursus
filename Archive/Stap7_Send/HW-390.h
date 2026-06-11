const int MOISTURE_ANALOGIC_IN = A0;  // Arduino's analogic pin

const int DRY = 3688;  // TODO: This is value when soil is dry. Adjust according to your usecase
const int WET = 560;   // TODO: This is value when soil is wet. Adjust according to your usecase

void setupSensor() {
  pinMode(MOISTURE_ANALOGIC_IN, INPUT);
}

void loopSensor() {
  delay(3000);

  auto moistureAnalogicVal = analogRead(MOISTURE_ANALOGIC_IN);  // Read the analogic data and convert it to [0, 4095]
  auto percentage = map(moistureAnalogicVal, WET, DRY, 100, 0);

  const static uint datastreamId = x;

  transmitValue(percentage, "% rH", datastreamId);
}
