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
  // setupGNSS();     // Initialize GPS module on pins D4/D5
  setupSensor();   // Initialize the selected sensor
  // setupWiFi();     // Start WiFi connection process

  Serial.println("[SYS ] Booted");  // Confirm successful boot
}

/**
 * Transmit sensor value to remote server/cloud
 * @param value The sensor reading to transmit
 */
void transmitValue(float value, char* UoM) {

  if (WiFi.status() == WL_CONNECTED) {
    // WiFi is connected - ready to transmit data
    Serial.printf("%.2f%s\n", value, UoM);

    // TODO: Add POST request

  } else {
    // WiFi not connected - log the value for debugging
    Serial.printf("[WiFi] Not connected to Transmit. Value: %.2f%s\n", value, UoM);
  }
}

/**
 * Main loop function - runs forever
 */
void loop() {
  // Check and update WiFi connection status
  // loopWifi();

  // Read and process GPS data
  // loopGNSS();
  
  // Sensor
  float temp = getTemperature();
  float hum = getHumidity();

  json = {
    "type": "Feature",
    "geometry": {
      "type": "Point", 
      "coordinates": [1.0, 1.0]
    },
    "properties": {
      "degrees C": temp,
      "% rH": hum
    }
  }

  transmitValue(json, "json");

  delay(2000);
}