// ASAIR AHT21B Temperatuur- en Luchtvochtigheidssensor Module - I2C

//  AHT21B PIN layout      
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       VCC          RED
//       |o  |       SDA          BLACK
//       |o  |       GND
//       |o  |       SCL
//       +---+

// Library: Adafruit AHTX0 
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

void setupSensor() {
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
}

void loopSensor() {
  delay(100);

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
 // Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
 // Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  const static uint datastreamIdTemp = x;
  const static uint datastreamIdRelH = x;

  transmitValue(temp.temperature, " degrees C", datastreamIdTemp);
  transmitValue(humidity.relative_humidity, "% rH", datastreamIdRelH);
}
