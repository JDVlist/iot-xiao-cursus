#include "../helpers/logging.h"
#include "../helpers/gnss.h"

void setup() {
  setupLogging();
  setupGNSS();
}

void loop() {
  updateGNSS();

  GNSSData gps = getGNSSData();

  Serial.println();
  Serial.println("=== GNSS status ===");

  Serial.print("Fix: ");
  Serial.println(gps.hasFix ? "yes" : "no");

  Serial.print("Status: ");
  Serial.println(gps.status);

  if (gps.hasFix) {
    Serial.print("Latitude: ");
    Serial.println(gps.lat, 6);

    Serial.print("Longitude: ");
    Serial.println(gps.lng, 6);

    Serial.print("Age ms: ");
    Serial.println(gps.age);
  } else {
    Serial.println("Latitude: no valid fix");
    Serial.println("Longitude: no valid fix");
    Serial.println("Age ms: no valid fix");
  }

  Serial.println();
  Serial.println("--- Debug ---");

  Serial.print("Chars processed: ");
  Serial.println(gps.charsProcessed);

  Serial.print("Sentences with fix: ");
  Serial.println(gps.sentencesWithFix);

  Serial.print("Failed checksums: ");
  Serial.println(gps.failedChecksum);

  delay(1000);
}