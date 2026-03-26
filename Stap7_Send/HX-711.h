//  HX711 PIN layout
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       GND         BLACK
//       |o  |       DT          WHITE.
//       |o  |       SCK         GRAY
//       |o  |       VCC.        RED
//       +---+

#include <HX711.h>

HX711 scale;

uint8_t dataPin = D1;
uint8_t clockPin = D2;

void setupSensor() {
  scale.begin(dataPin, clockPin);

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  Serial.println("\nEmpty the scale, press a key to continue");
  while (!Serial.available())
    ;
  while (Serial.available()) Serial.read();

  scale.tare();
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));


  Serial.println("\nPut 1000 gram in the scale, press a key to continue");
  while (!Serial.available())
    ;
  while (Serial.available()) Serial.read();

  scale.calibrate_scale(1000, 5);
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));
}

void loopSensor() {
  auto weight = scale.get_units(10) / 1000.f;

  const static uint datastreamId = x;

  transmitValue(weight, "kg", datastreamId);
}
