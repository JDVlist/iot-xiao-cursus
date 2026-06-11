#include <ArduinoJson.h>

#include "../helpers/logging.h"
#include "../helpers/iot_wifi.h"
#include "../helpers/gnss.h"
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"

const unsigned long GEOJSON_PRINT_INTERVAL_MS = 5000;

unsigned long lastGeoJsonPrint = 0;

String buildGeoJson(GNSSData gps, WiFiData wifi, float temperature, float humidity) {
  StaticJsonDocument<768> doc;

  doc["type"] = "Feature";

  if (gps.hasFix) {
    JsonObject geometry = doc.createNestedObject("geometry");
    geometry["type"] = "Point";

    JsonArray coordinates = geometry.createNestedArray("coordinates");
    coordinates.add(gps.lng);
    coordinates.add(gps.lat);
  } else {
    doc["geometry"] = nullptr;
  }

  JsonObject properties = doc.createNestedObject("properties");

  if (isnan(temperature)) {
    properties["temperatureC"] = nullptr;
  } else {
    properties["temperatureC"] = temperature;
  }

  if (isnan(humidity)) {
    properties["humidityRH"] = nullptr;
  } else {
    properties["humidityRH"] = humidity;
  }

  properties["gnssHasFix"] = gps.hasFix;
  properties["gnssStatus"] = gps.status;
  properties["gnssAgeMs"] = gps.age;
  properties["wifiConnected"] = wifi.connected;
  properties["wifiStatus"] = wifi.status;
  properties["wifiSsid"] = wifi.ssid;
  properties["wifiIpAddress"] = wifi.ipAddress;
  properties["wifiRssi"] = wifi.rssi;

  String geoJson;
  serializeJson(doc, geoJson);
  return geoJson;
}

void printGeoJson() {
  GNSSData gps = getGNSSData();
  WiFiData wifi = getWiFiData();

  float temperature = getTemperature();
  float humidity = getHumidity();

  String geoJson = buildGeoJson(gps, wifi, temperature, humidity);

  Serial.println();
  Serial.println("=== GeoJSON ===");
  Serial.println(geoJson);

  if (wifi.connected) {
    Serial.println("[HTTP] Backend not online yet; GeoJSON is printed instead of sent");
  } else {
    Serial.println("[HTTP] WiFi not connected; GeoJSON is printed only");
  }
}

void setup() {
  setupLogging();
  setupSensor();
  setupGNSS();
  setupWiFi();

  Serial.println("[SYS ] Integration step ready");
}

void loop() {
  updateGNSS();
  updateWiFi();

  if (millis() - lastGeoJsonPrint >= GEOJSON_PRINT_INTERVAL_MS) {
    lastGeoJsonPrint = millis();
    printGeoJson();
  }
}
