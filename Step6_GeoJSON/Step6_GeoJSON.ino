#include "../helpers/logging.h"
#include "../helpers/iot_wifi.h"
#include "../helpers/gnss.h"
#include "../helpers/geojson.h"
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"

// Print one GeoJSON message every 5 seconds.
// The loop itself keeps running quickly so GNSS and WiFi can update in between.
const unsigned long GEOJSON_PRINT_INTERVAL_MS = 5000;

unsigned long lastGeoJsonPrint = 0;

GeoJsonMeasurement getCurrentMeasurement() {
  GNSSData gps = getGNSSData();
  WiFiData wifi = getWiFiData();
  float temperature = getTemperature();
  float humidity = getHumidity();

  GeoJsonMeasurement measurement;

  measurement.hasLocation = gps.hasFix;
  measurement.latitude = gps.lat;
  measurement.longitude = gps.lng;

  measurement.temperatureC = temperature;
  measurement.humidityRH = humidity;

  measurement.sensorName = "temp";
  measurement.value = temperature;
  measurement.unit = "C";

  measurement.deviceId = wifi.macAddress;
  measurement.gnssStatus = gps.status;
  measurement.gnssAgeMs = gps.age;

  measurement.wifiConnected = wifi.connected;
  measurement.wifiStatus = wifi.status;
  measurement.wifiSsid = wifi.ssid;
  measurement.wifiIpAddress = wifi.ipAddress;
  measurement.wifiRssi = wifi.rssi;

  return measurement;
}


void setup() {
  // Start all reusable modules.
  setupLogging();
  setupSensor();
  setupGNSS();
  setupWiFi();

  Serial.println("[SYS ] GeoJSON step ready");
}

void loop() {
  // Keep updating modules as often as possible.
  updateGNSS();
  updateWiFi();

  // Only build and print the GeoJSON every 5 seconds.
  if (millis() - lastGeoJsonPrint >= GEOJSON_PRINT_INTERVAL_MS) {
    lastGeoJsonPrint = millis();

    GeoJsonMeasurement measurement = getCurrentMeasurement();
    String geoJson = buildGeoJson(measurement);

    Serial.println();
    Serial.println("=== GeoJSON ===");
    Serial.println(geoJson);
  }
}
