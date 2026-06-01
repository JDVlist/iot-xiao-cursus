//  HX711 PIN layout
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       GND         BLACK
//       |o  |       DT          WHITE.
//       |o  |       SCK         GRAY
//       |o  |       VCC.        RED
//       +---+
//
// Observed Properties:
// - weight
//
// Unit of Measure:
// - g

#include <HX711.h> // Rob Tillaart

HX711 scale;

uint8_t dataPin = D1;
uint8_t clockPin = D2;

void setupSensor() {
  scale.begin(dataPin, clockPin);

  Serial.print("HX711_LIB_VERSION: ");
  Serial.println(HX711_LIB_VERSION);

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nEmpty the scale, press a key to continue");
  while (!Serial.available())
    ;
  while (Serial.available()) Serial.read();

  scale.tare();
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nPut 1000 gram on the scale, press a key to continue");
  while (!Serial.available());
  while (Serial.available()) Serial.read();

  scale.calibrate_scale(1000, 5);
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));
}

void loopSensor() {
  auto weight = scale.get_units(10) / 1000.f;

   transmitValue(weight, "kg");
}
