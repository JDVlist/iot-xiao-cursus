//  ADXL345 PIN layout           ADXL345 COLOR   PIN
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       GND          BLACK
//       |o  |       VDD          RED
//       |o  |       CS
//       |o  |       INT1
//       |o  |       INT2
//       |o  |       SD0
//       |o  |       SDA          YELLOW          D4
//       |o  |       SCL          GREEN           D5
//       +---+

#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setupSensor() {
  /* Initialise the sensor */
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("[ADXL345] Ooops, no ADXL345 detected ... Check your wiring!");
    while (1)
      ;
  }
  Serial.println("[ADXL345] Device connected over I2C");

  sensor_t sensor;
  accel.getSensor(&sensor);

  Serial.print("[ADXL345] Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("[ADXL345] Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("[ADXL345] Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("[ADXL345] Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("[ADXL345] Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("[ADXL345] Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  //accel.setRange(ADXL345_RANGE_2_G);
}

void loopSensor() {
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("[ADXL345] X: ");
  Serial.print(event.acceleration.x);
  Serial.print("  ");
  Serial.print("[ADXL345] Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("  ");
  Serial.print("[ADXL345] Z: ");
  Serial.print(event.acceleration.z);
  Serial.print("  ");
  Serial.println("m/s^2 ");

  const static uint datastreamId = x;

  transmitValue(event.acceleration.x, "m/s^2", datastreamId);
}
