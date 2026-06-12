#include <HTTPClient.h>

#include "../helpers/logging.h"
#include "../helpers/iot_wifi.h"
#include "../helpers/gnss.h"
#include "../helpers/geojson.h"
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"

// Temporary backend URL.
//
// Replace 192.168.0.100 with the IP address of your laptop on the same WiFi.
// Do not use localhost here: on the XIAO, localhost means the XIAO itself.
const char* BACKEND_URL = "http://192.168.0.100:8000/measurements";

// Send one request every 5 seconds.
// The loop itself keeps running quickly so GNSS and WiFi can update in between.
const unsigned long REQUEST_INTERVAL_MS = 5000;

unsigned long lastRequest = 0;

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

void sendGeoJson(String geoJson) {
  if (!isWiFiConnected()) {
    Serial.println("[HTTP] Not sent: WiFi is not connected");
    return;
  }

  HTTPClient http;

  Serial.print("[HTTP] POST ");
  Serial.println(BACKEND_URL);

  http.begin(BACKEND_URL);
  http.addHeader("Content-Type", "application/geo+json");

  int statusCode = http.POST(geoJson);

  Serial.print("[HTTP] Status code: ");
  Serial.println(statusCode);

  if (statusCode > 0) {
    String response = http.getString();
    Serial.print("[HTTP] Response: ");
    Serial.println(response);
  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(http.errorToString(statusCode));
  }

  http.end();
}

void setup() {
  setupLogging();
  setupSensor();
  setupGNSS();
  setupWiFi();

  Serial.println("[SYS ] Request step ready");
}

void loop() {
  updateGNSS();
  updateWiFi();

  if (millis() - lastRequest >= REQUEST_INTERVAL_MS) {
    lastRequest = millis();

    GeoJsonMeasurement measurement = getCurrentMeasurement();
    String geoJson = buildGeoJson(measurement);

    Serial.println();
    Serial.println("=== GeoJSON body ===");
    Serial.println(geoJson);

    sendGeoJson(geoJson);
  }
}
