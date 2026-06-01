#include "../helpers/logging.h"
// #include "../sensors/AM-2320.h"
#include "../sensors/dummy_sensor.h"

void setup() {
  setupLogging();
  setupSensor();
}

void loop() {
  float temperature = getTemperature();
  float humidity = getHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  delay(2000);
}