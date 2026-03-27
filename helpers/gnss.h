#pragma once

#include <TinyGPSPlus.h>

// Install TinyGPSPlus in Arduino IDE libraries:
// https://github.com/mikalhart/TinyGPSPlus

// Pin definitions for XIAO ESP32 C3
#define RXPin D7
#define TXPin D6
#define SERIALGNSS Serial1

const uint32_t GNSSBaud = 9600;

// GNSS data container
struct GNSSData {
  double lat;              // WGS84 latitude in decimal degrees
  double lng;              // WGS84 longitude in decimal degrees
  bool hasFix;             // true when a valid location fix is available
  unsigned long age;       // age of the fix in milliseconds
};

// Internal GNSS parser object
TinyGPSPlus gnss;

// Internal state
double currentLat = 0.0;
double currentLng = 0.0;
bool gnssHasFix = false;
unsigned long lastGNSSUpdate = 0;
bool gnssTimeoutReported = false;

// Initialize GNSS serial connection
void setupGNSS()
{
  Serial.println("[GNSS] Initializing...");
  SERIALGNSS.begin(GNSSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println("[GNSS] Initialized");
}

// Read and process incoming GNSS data
void updateGNSS()
{
  while (SERIALGNSS.available() > 0) {
    if (gnss.encode(SERIALGNSS.read())) {
      if (gnss.location.isUpdated() && gnss.location.isValid()) {
        currentLat = gnss.location.lat();
        currentLng = gnss.location.lng();
        gnssHasFix = true;
        lastGNSSUpdate = millis();
        gnssTimeoutReported = false;
      }
    }
  }

  // Timeout after 30 seconds without a valid update
  if (gnssHasFix && (millis() - lastGNSSUpdate > 30000)) {
    gnssHasFix = false;

    if (!gnssTimeoutReported) {
      Serial.println("[GNSS] Timeout - no recent fix");
      gnssTimeoutReported = true;
    }
  }
}

// Get only latitude
double getLatitude()
{
  return currentLat;
}

// Get only longitude
double getLongitude()
{
  return currentLng;
}

// Check whether current GNSS fix is valid
bool hasGNSSFix()
{
  return gnssHasFix;
}

// Get all GNSS data in one call
GNSSData getGNSSData()
{
  GNSSData data;
  data.lat = currentLat;
  data.lng = currentLng;
  data.hasFix = gnssHasFix;
  data.age = gnss.location.age();
  return data;
}