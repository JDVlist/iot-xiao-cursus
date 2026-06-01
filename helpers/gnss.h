#pragma once

#include <math.h>
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
  double lat;
  double lng;
  bool hasFix;
  unsigned long age;
  unsigned long charsProcessed;
  unsigned long sentencesWithFix;
  unsigned long failedChecksum;
  const char* status;
};

// Internal GNSS parser object
TinyGPSPlus gnss;

// Internal state
double currentLat = NAN;
double currentLng = NAN;
bool gnssHasFix = false;
unsigned long lastGNSSUpdate = 0;
bool gnssTimeoutReported = false;
const char* gnssStatus = "waiting for GNSS data";

void setupGNSS() {
  Serial.println("[GNSS] Initializing...");
  SERIALGNSS.begin(GNSSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println("[GNSS] Initialized");
}

void updateGNSS() {
  while (SERIALGNSS.available() > 0) {
    if (gnss.encode(SERIALGNSS.read())) {
      if (gnss.location.isUpdated() && gnss.location.isValid()) {
        currentLat = gnss.location.lat();
        currentLng = gnss.location.lng();
        gnssHasFix = true;
        lastGNSSUpdate = millis();
        gnssTimeoutReported = false;
        gnssStatus = "valid GNSS fix";
      } else if (!gnss.location.isValid()) {
        gnssHasFix = false;
        currentLat = NAN;
        currentLng = NAN;
        gnssStatus = "GNSS data received, waiting for valid location fix";
      }
    }
  }

  if (millis() > 5000 && gnss.charsProcessed() < 10) {
    gnssHasFix = false;
    currentLat = NAN;
    currentLng = NAN;
    gnssStatus = "no GNSS data received; check wiring, power, and RX/TX pins";
  }

  if (gnssHasFix && (millis() - lastGNSSUpdate > 30000)) {
    gnssHasFix = false;
    currentLat = NAN;
    currentLng = NAN;
    gnssStatus = "GNSS timeout; no recent valid location fix";

    if (!gnssTimeoutReported) {
      Serial.println("[GNSS] Timeout - no recent fix");
      gnssTimeoutReported = true;
    }
  }
}

double getLatitude() {
  return currentLat;
}

double getLongitude() {
  return currentLng;
}

bool hasGNSSFix() {
  return gnssHasFix;
}

GNSSData getGNSSData() {
  GNSSData data;
  data.lat = currentLat;
  data.lng = currentLng;
  data.hasFix = gnssHasFix;
  data.age = gnss.location.age();
  data.charsProcessed = gnss.charsProcessed();
  data.sentencesWithFix = gnss.sentencesWithFix();
  data.failedChecksum = gnss.failedChecksum();
  data.status = gnssStatus;
  return data;
}