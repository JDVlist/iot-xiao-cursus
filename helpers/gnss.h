#pragma once

// math.h is used for NAN.
// NAN means "Not A Number". We use it when latitude/longitude are not valid yet.
// That is clearer than returning 0.0, because 0.0 is a real coordinate.
#include <math.h>

// TinyGPSPlus reads NMEA sentences from the GNSS module and turns them into
// usable values such as latitude, longitude, age, and debug counters.
#include <TinyGPSPlus.h>

// Install TinyGPSPlus in Arduino IDE libraries:
// https://github.com/mikalhart/TinyGPSPlus

// Pin definitions for the XIAO ESP32 C3.
//
// The GNSS module sends data from its TX pin.
// The XIAO receives that data on its RX pin.
//
// So the wire goes:
//   GNSS TX -> XIAO D7 / RX
//
// D6 / TX is defined here for completeness, but in this course step we only
// read data from the GNSS module.
#define RXPin D7
#define TXPin D6

// The XIAO has more than one serial connection.
// Serial is used for USB/Serial Monitor.
// Serial1 is used here for the GNSS module.
#define SERIALGNSS Serial1

// Most simple GNSS modules use 9600 baud by default.
// Baud rate is the speed of the serial connection.
const uint32_t GNSSBaud = 9600;

// GNSSData is the small data package that the rest of the code can use.
//
// The sketch does not need to know how TinyGPSPlus works internally.
// It only calls getGNSSData() and receives this struct back.
struct GNSSData {
  // WGS 84 latitude in decimal degrees.
  // Example: 52.110352
  double lat;

  // WGS 84 longitude in decimal degrees.
  // Example: 4.897106
  double lng;

  // true when the GNSS module has calculated a valid position.
  // false when there is no usable position yet.
  bool hasFix;

  // Age of the last known location in milliseconds.
  // A low value means the position is fresh.
  unsigned long age;

  // Number of characters received and processed from the GNSS module.
  // If this stays at 0, the XIAO is not receiving serial data.
  unsigned long charsProcessed;

  // Number of NMEA sentences that contained a valid location fix.
  // This should start increasing once the module has a fix.
  unsigned long sentencesWithFix;

  // Number of NMEA sentences that failed their checksum.
  // A few failures can happen. Many failures can point to noisy wiring,
  // unstable power, or a bad connection.
  unsigned long failedChecksum;

  // Human-readable status for debugging in the Serial Monitor.
  const char* status;
};

// Internal GNSS parser object.
// This object keeps track of all received NMEA data.
TinyGPSPlus gnss;

// Internal state.
//
// These variables remember the last useful GNSS values.
// They are kept private to this helper file. Other code should use
// getGNSSData(), getLatitude(), getLongitude(), or hasGNSSFix().
double currentLat = NAN;
double currentLng = NAN;
bool gnssHasFix = false;

// millis() timestamp of the last valid location update.
// Used to detect when an old fix has become stale.
unsigned long lastGNSSUpdate = 0;

// Prevents printing the timeout warning over and over again.
bool gnssTimeoutReported = false;

// Short text that explains the current GNSS state.
const char* gnssStatus = "waiting for GNSS data";

// Start the serial connection to the GNSS module.
//
// Call this once from setup().
void setupGNSS() {
  Serial.println("[GNSS] Initializing...");
  SERIALGNSS.begin(GNSSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println("[GNSS] Initialized");
}

// Read and process incoming GNSS bytes.
//
// Call this often from loop().
//
// The GNSS module sends a constant stream of characters. TinyGPSPlus needs to
// see those characters one by one. When enough characters have been received
// to complete an NMEA sentence, TinyGPSPlus updates its internal values.
void updateGNSS() {
  // Read all bytes that are currently waiting in the serial buffer.
  while (SERIALGNSS.available() > 0) {
    // gnss.encode(...) returns true when a complete sentence was processed.
    if (gnss.encode(SERIALGNSS.read())) {
      // A location can be "updated" but still not valid.
      // We only store it when TinyGPSPlus says it is valid.
      if (gnss.location.isUpdated() && gnss.location.isValid()) {
        currentLat = gnss.location.lat();
        currentLng = gnss.location.lng();
        gnssHasFix = true;
        lastGNSSUpdate = millis();
        gnssTimeoutReported = false;
        gnssStatus = "valid GNSS fix";
      } else if (!gnss.location.isValid()) {
        // We are receiving GNSS data, but the module does not have a valid
        // position yet. This can happen indoors or shortly after startup.
        gnssHasFix = false;
        currentLat = NAN;
        currentLng = NAN;
        gnssStatus = "GNSS data received, waiting for valid location fix";
      }
    }
  }

  // If we have waited a few seconds and TinyGPSPlus has processed almost
  // nothing, the XIAO is probably not receiving data from the GNSS module.
  //
  // Common causes:
  // - GNSS module has no power
  // - GND is not connected
  // - TX/RX wiring is wrong
  // - wrong RX pin
  // - wrong baud rate
  if (millis() > 5000 && gnss.charsProcessed() < 10) {
    gnssHasFix = false;
    currentLat = NAN;
    currentLng = NAN;
    gnssStatus = "no GNSS data received; check wiring, power, and RX/TX pins";
  }

  // A fix can become old. If we once had a valid fix, but no new valid update
  // has arrived for 30 seconds, mark the fix as invalid.
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

// Convenience function: return only the current latitude.
// Returns NAN if there is no valid fix.
double getLatitude() {
  return currentLat;
}

// Convenience function: return only the current longitude.
// Returns NAN if there is no valid fix.
double getLongitude() {
  return currentLng;
}

// Convenience function: true when there is a valid GNSS fix.
bool hasGNSSFix() {
  return gnssHasFix;
}

// Return all GNSS information in one package.
//
// This is the most useful function for later integration:
// - Stap4_GNSS.ino can print this data for debugging.
// - The integration step can use this data to build GeoJSON.
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
