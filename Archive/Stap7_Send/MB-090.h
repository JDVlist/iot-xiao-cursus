//  MB-090 PIN layout  (left to right)
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       GND        Black
//       +---+
////
// Observed Properties:
// - bending resistance
//
// Unit of Measure:
// - ohm

// Resistance (Kohm) will increase when bending angle increases

// Note: do not forget the 10Kohm resistor
// 3.3V --- Alpha MB090 bending sensor --- Analog pin (e.g. GPIO3) --- 10kΩ resistor --- GND

const uint8_t measurePin = A0;  // analoge uitgang van versterkt microfoon signaal

const float fixedResistor = 10000;  // 10Kohm
const float vcc = 3.3f;             // VCC

void setupSensor() {
  pinMode(measurePin, INPUT);

  Serial.println("[MB090] Alpha MB090 bending sensor ready");
}

void loopSensor() {
  delay(200);

  auto flexADC = analogRead(measurePin);

  float flexVoltage = flexADC * vcc / 4095.0;
  float flexResistance = (fixedResistor * (vcc / flexVoltage - 1.0));

  // float angle = map(flexResistance, flatResistance, bendResistance, 0, 90.0);

    const static uint datastreamId = x;

  transmitValue(flexResistance, " ohm", datastreamId);
}
