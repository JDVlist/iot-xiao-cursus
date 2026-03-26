/*
 * IoT Sensor Project - Step 7: Sensor Data Collection
 * 
 * This Arduino sketch implements a comprehensive IoT sensor system that:
 * - Connects to WiFi for data transmission
 * - Collects GPS location data
 * - Reads sensor data
 * - Transmits sensor values to a remote server/cloud (Sensor Things API)
 * 
 * Hardware: XIAO ESP32 C3 with various sensors
 * Author: IoT Project
 * Date: aug 2025
 */

#include <Arduino_JSON.h>  // from https://github.com/arduino-libraries/Arduino_JSON

// Include helper libraries for different system components
#include "../arduino_secrets.h"  // WiFi credentials and secrets

#include "../helpers/logging.h"   // Serial communication and logging setup
#include "../helpers/wifi.h"      // WiFi connection management
#include "../helpers/gnss.h"          // GPS location tracking
#include "../helpers/stringFormat.h" 

#include <HTTPClient.h> // ships with ESP32 library, no extra lib needed

const char* serviceHost = "http://iot.aardvark.myds.me/FROST-Server";
const char* serviceVersion = "v1.1";

HTTPClient http;

// Function declaration for data transmission
void transmitValue(float value, char* UoM, uint datastreamId);

/*
 * SENSOR CONFIGURATION
 * 
 * Uncomment the sensor you want to use. Only one sensor can be active at a time
 * due to pin conflicts and I2C address conflicts.
 */
//#include "ADXL-345.h"   // Accelerometer ✅
//#include "AHT-21B.h"    // Measure temperature and humudity ✅
//#include "BME-280.h"    // Measure Thermometer, Barometer en Vochtigheid ✅
//#include "HMC-5883L.h"  // Digital Compass ✅
//#include "HW-390.h"     // Measure Soil Moisture ✅
//#include "HX-711.h"     // Weegschaal 🚜
//#include "MAT-060.h"    // Sliding potentiometer ✅
//#include "MAX-4466.h"   // Microphone applifier, clap detector ✅
//#include "MAX-30102.h"  // PulseOximeter ✅
//#include "MB-090.h"     // Bending Detection (90mm) ✅
//#include "RCWL-1604.h"  // Distance measurement ✅
//#include "SEN-0564.h"   // CO measurement ✅
//#include "TTP-223B.h"   // Capacitive touch ✅

/**
 * Setup function - runs once at startup
 * Initializes all system components in the correct order
 */
void setup() {
  // Initialize system components
  setupLogging();  // Start serial communication at 115200 baud
  setupGNSS();     // Initialize GPS module on pins D4/D5
  setupSensor();   // Initialize the selected sensor
  setupWiFi();     // Start WiFi connection process

  Serial.println("[SYS ] Booted");  // Confirm successful boot
}

/**
 * Transmit sensor value to remote server/cloud
 * 
 * @param value The sensor reading to transmit
 * 
 * This function handles the data transmission logic. Currently it only
 * logs the value, but can be extended to send data via HTTP POST,
 * MQTT, or other protocols.
 */
void transmitValue(float value, char* UoM, uint datastreamId) {
  if (WiFi.status() == WL_CONNECTED) {
    // WiFi is connected - ready to transmit data
    Serial.printf("Value: %.2f%s (Datastream id: %d)\n", value, UoM, datastreamId);

    // Example implementations:
    // - HTTP POST
    // - MQTT publish

/*
    JSONVar point;
    point["type"] = "Point";
    JSONVar coordinates;
    coordinates[0] = lat;
    coordinates[1] = lng;
    point["coordinates"] = coordinates;

    JSONVar featureOfInterest;
    featureOfInterest["name"] = "hier"; // TODO
    featureOfInterest["description"] = "iets meer naar ginder"; // TODO
    featureOfInterest["encodingType"] = "application/vnd.geo+json";
    featureOfInterest["feature"] = point;
*/

    JSONVar observation;
    //  observation["FeatureOfInterest"] = featureOfInterest;
    //  observation["phenomenonTime"] = getISO8601dateTime();
    observation["result"] = value;

    const String url = formatString("%s/%s/Datastreams(%d)/Observations", serviceHost, serviceVersion, datastreamId);
    auto body = JSON.stringify(observation);

    Serial.print("HTTP POST to ");
    Serial.print(url);
    Serial.print(" with body ");
    Serial.println(body);

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    auto httpCode = http.POST(body);
    Serial.printf("[HTTP] Result of HTTP POST %d\n", httpCode);
    http.end();

  } else {
    // WiFi not connected - log the value for debugging
    Serial.printf("[WiFi] Not connected to Transmit. Value: %.2f%s (Datastream Id: %d)\n", value, UoM, datastreamId);
  }
}

/**
 * Main loop function - runs forever
 * 
 * This function implements a non-blocking architecture where each
 * component (WiFi, GPS, sensor) is checked and updated independently.
 * This prevents one component from blocking the others.
 */
void loop() {
  // Check and update WiFi connection status
  loopWifi();

  // Read and process GPS data
  loopGNSS();

  // Read sensor data and transmit if valid
  loopSensor();
}