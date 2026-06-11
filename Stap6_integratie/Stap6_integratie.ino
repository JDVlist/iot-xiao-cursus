#include <ArduinoJson.h>

#include "../helpers/logging.h"
#include "../helpers/iot_wifi.h"
#include "../helpers/gnss.h"
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"

// Print one GeoJSON message every 5 seconds.
// The loop itself keeps running quickly so GNSS and WiFi can update in between.
const unsigned long GEOJSON_PRINT_INTERVAL_MS = 5000;

unsigned long lastGeoJsonPrint = 0;

// Build one GeoJSON Feature as a String.
//
// This String is the payload that we print now. In a later step, the same
// variable can be sent to a backend with an HTTP request.
String buildGeoJson(GNSSData gps, WiFiData wifi, float temperature, float humidity) {
  StaticJsonDocument<768> doc;

  // A GeoJSON message is usually a Feature.
  // It contains geometry for the location and properties for measurements.
  doc["type"] = "Feature";

  if (gps.hasFix) {
    JsonObject geometry = doc.createNestedObject("geometry");
    geometry["type"] = "Point";

    // GeoJSON coordinates use WGS 84 and this order:
    // [longitude, latitude]
    //
    // This is the opposite of how people often say coordinates out loud.
    JsonArray coordinates = geometry.createNestedArray("coordinates");
    coordinates.add(gps.lng);
    coordinates.add(gps.lat);
  } else {
    // Without a GNSS fix there is no valid location yet.
    // geometry: null keeps the JSON valid.
    doc["geometry"] = nullptr;
  }

  JsonObject properties = doc.createNestedObject("properties");

  // Some sensors return NaN when a reading fails.
  // JSON has no NaN value, so we write null instead.
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

  // Serialize the JSON document into one String variable.
  String geoJson;
  serializeJson(doc, geoJson);
  return geoJson;
}

// Read all current data, build the GeoJSON payload, and print it.
// Later this is the place where an HTTP request can be added.
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
  // Start all reusable modules.
  setupLogging();
  setupSensor();
  setupGNSS();
  setupWiFi();

  Serial.println("[SYS ] Integration step ready");
}

void loop() {
  // Keep updating modules as often as possible.
  updateGNSS();
  updateWiFi();

  // Only build and print the GeoJSON every 5 seconds.
  if (millis() - lastGeoJsonPrint >= GEOJSON_PRINT_INTERVAL_MS) {
    lastGeoJsonPrint = millis();
    printGeoJson();
  }
}
