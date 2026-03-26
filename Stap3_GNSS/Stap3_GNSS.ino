/*
 * GPS Module Test and Data Display
 * 
 * This sketch demonstrates how to use the TinyGPSPlus library to read
 * GPS data from a GPS module connected to an ESP32-C3.
 * 
 * Hardware Setup:
 * - GPS Module connected to D7 (RX) and D6 (TX)
 * - Baud rate: 9600
 * 
 * Features:
 * - Displays satellite count, HDOP, coordinates, altitude, speed, course
 * - Shows distance and direction to a reference point (Amersfoort)
 * - Monitors GPS data quality and connection status
 */

#include "../helpers/logging.h"
#include "../helpers/gnss.h"
#include "../helpers/mac.h"

void setup()
{
  setupLogging();

  Serial.println(F("GNSS Module Test Starting..."));
  Serial.println(F("================================"));

  // Initialize GPS serial communication
  Serial1.begin(GNSSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println(F("GNSS Serial1 initialized"));
 
  // Print column headers for GPS data display
  Serial.println(F("GNSS Data Display Format:"));
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GNSS ---  ---- to Amersfoort  ---  RX    RX        Fail"));
  Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
}

void loop()
{
  // Display GPS satellite information
  printInt(gnss.satellites.value(), gnss.satellites.isValid(), 5);
  
  // Display Horizontal Dilution of Precision (HDOP) - lower is better
  printFloat(gnss.hdop.hdop(), gnss.hdop.isValid(), 6, 1);
  
  // Display latitude and longitude
  printFloat(gnss.location.lat(), gnss.location.isValid(), 11, 6);
  printFloat(gnss.location.lng(), gnss.location.isValid(), 12, 6);
  
  // Display location age (how old the fix is)
  printInt(gnss.location.age(), gnss.location.isValid(), 5);
  
  // Display date and time from GPS
  printDateTime(gnss.date, gnss.time);
  
  // Display altitude in meters
  printFloat(gnss.altitude.meters(), gnss.altitude.isValid(), 7, 2);
  
  // Display course (heading) in degrees
  printFloat(gnss.course.deg(), gnss.course.isValid(), 7, 2);
  
  // Display speed in km/h
  printFloat(gnss.speed.kmph(), gnss.speed.isValid(), 6, 2);
  
  // Display cardinal direction (N, NE, E, SE, S, SW, W, NW)
  printStr(gnss.course.isValid() ? TinyGPSPlus::cardinal(gnss.course.deg()) : "*** ", 6);

  // Calculate distance to Amersfoort in kilometers
  unsigned long distanceKmToAmersfoort =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gnss.location.lat(),
      gnss.location.lng(),
      AMERSFOORT_LAT, 
      AMERSFOORT_LON) / 1000;
  printInt(distanceKmToAmersfoort, gnss.location.isValid(), 9);

  // Calculate course (bearing) to Amersfoort
  double courseToAmersfoort =
    TinyGPSPlus::courseTo(
      gnss.location.lat(),
      gnss.location.lng(),
      AMERSFOORT_LAT, 
      AMERSFOORT_LON);

  printFloat(courseToAmersfoort, gnss.location.isValid(), 7, 2);

  // Display cardinal direction to Amersfoort
  const char *cardinalToAmersfoort = TinyGPSPlus::cardinal(courseToAmersfoort);
  printStr(gnss.location.isValid() ? cardinalToAmersfoort : "*** ", 6);

  // Display GPS data quality metrics
  printInt(gnss.charsProcessed(), true, 6);        // Total characters processed
  printInt(gnss.sentencesWithFix(), true, 10);     // Sentences with valid fix
  printInt(gnss.failedChecksum(), true, 9);        // Failed checksum count
  
  Serial.println();  // End of data line
  
  // Smart delay that feeds GPS data while waiting
  smartDelay(1000);

  // Check if gnss is receiving data (after 5 seconds)
  if (millis() > 5000 && gnss.charsProcessed() < 10) {
    Serial.println(F("WARNING: No GNSS data received - check wiring and connections"));
    Serial.println(F("Make sure GNSS module is powered and connected to D7 (RX)"));
  }
}

