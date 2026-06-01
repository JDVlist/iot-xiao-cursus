/*
 * IoT Sensor Project - Step 5: Sensor Data Collection
 * 
 * This Arduino sketch implements a comprehensive IoT sensor system that:
 * - Connects to WiFi for data transmission
 * - Collects GPS location data
 * - Reads sensor data 
 * - Prints sensor values 
 * 
 * Hardware: XIAO ESP32 C3 with various sensors
 * Author: IoT Project
 * Date: aug 2025
 */

#include "../arduino_secrets.h"  // WiFi credentials and secrets
// Include helper libraries for different system components
#include "../helpers/logging.h"          // Serial communication and logging setup
#include "../helpers/wifi.h"             // WiFi connection management
//#include "helpers/dateTime.h"   // Time synchronization (currently disabled)
#include "../helpers/gnss.h"  // GPS location tracking

#include <ArduinoJson.h>

// Function declaration for data transmission
void transmitValue(float value, char* UoM);

/*
 * SENSOR CONFIGURATION
 * 
 * Uncomment the sensor you want to use. Only one sensor can be active at a time
 * due to pin conflicts and I2C address conflicts.
 */

#include "AM-2320.h"

/**
 * Setup function - runs once at startup
 * Initializes all system components in the correct order
 */
void setup() {
  // Initialize system components
  setupLogging();  // Start serial communication at 115200 baud
  setupGNSS();     // Initialize GPS module on pins D4/D5
  setupSensor();   // Initialize the selected sensor
  // setupWiFi();     // Start WiFi connection process

  Serial.println("[SYS ] Booted");  // Confirm successful boot
}

// /**
//  * Transmit sensor value to remote server/cloud
//  * @param value The sensor reading to transmit
//  */
// void transmitValue(float value, char* UoM) {

//   if (WiFi.status() == WL_CONNECTED) {
//     // WiFi is connected - ready to transmit data
//     Serial.printf("%.2f%s\n", value, UoM);

//     // TODO: Add POST request

//   } else {
//     // WiFi not connected - log the value for debugging
//     Serial.printf("[WiFi] Not connected to Transmit. Value: %.2f%s\n", value, UoM);
//   }
// }

/**
 * Main loop function - runs forever
 */
void loop() {
  // Update GNSS state
  updateGNSS();

  // Read GNSS values
  GNSSData gps = getGNSSData();

  // Read sensor values
  float temp = getTemperature();
  float hum = getHumidity();

  // Build JSON
  StaticJsonDocument<256> doc;
  doc["type"] = "Feature";

  //TODO: fix fixen
  if (gps.hasFix) {
    JsonObject geometry = doc.createNestedObject("geometry");
    geometry["type"] = "Point";

    JsonArray coordinates = geometry.createNestedArray("coordinates");
    coordinates.add(gps.lng);   // GeoJSON: longitude first
    coordinates.add(gps.lat);   // then latitude
  } else {
    doc["geometry"] = nullptr;
    Serial.println("[GNSS] No valid fix available");
  }

  JsonObject properties = doc.createNestedObject("properties");
  properties["degrees C"] = temp;
  properties["% rH"] = hum;
  properties["hasFix"] = gps.hasFix;
  properties["gnssAgeMs"] = gps.age;

  String json;
  serializeJson(doc, json);

  Serial.println(json);

  delay(2000);
}