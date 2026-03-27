#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setupSensor() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  }

  am2320.begin();
}

float getTemperature() {
  return am2320.readTemperature();
}

float getHumidity() {
  return am2320.readHumidity();
}