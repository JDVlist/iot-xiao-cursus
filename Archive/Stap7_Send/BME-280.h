//  BME280 PIN layout      
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       VCC          RED
//       |o  |       GND          BLACK
//       |o  |       SCL
//       |o  |       SDA
//       |o  |       CSB
//       |o  |       SDO
//       +---+

#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define I2C_ADDRESS 0x76

Adafruit_BME280 bme;  // I2C

void setupSensor() {
  if (!bme.begin(I2C_ADDRESS)) {
    Serial.println("[BMP180] Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("[BMP180] SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("[BMP180] ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("         ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("         ID of 0x60 represents a BME 280.\n");
    Serial.print("         ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
    Serial.print("[BMP180] Device OK");
}

void loopSensor() {
  delay(1000);

  auto temperature = bme.readTemperature();
  auto pressure    = bme.readPressure();
  auto altitude    = bme.readAltitude(SEALEVELPRESSURE_HPA);
  auto relHumidity = bme.readHumidity();

  const static uint datastreamIdTemp   = x;
  const static uint datastreamIdPres   = x;
  const static uint datastreamIdAlt    = x;
  const static uint datastreamIdRelHum = x;

  transmitValue(temperature, "°C", datastreamIdTemp);
  transmitValue(pressure, " hPa", datastreamIdPres);
  transmitValue(altitude, " m", datastreamIdAlt);
  transmitValue(relHumidity, " % rH", datastreamIdRelHum);
}
