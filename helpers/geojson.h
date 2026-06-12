#pragma once

#include <Arduino.h>
#include <math.h>
#include <ArduinoJson.h>

// Data package for one measurement message.
//
// The sketch fills this struct with current sensor, GNSS, and WiFi values.
// This helper only turns that data into one GeoJSON String.
struct GeoJsonMeasurement {
  bool hasLocation;
  double latitude;
  double longitude;

  float temperatureC;
  float humidityRH;

  String sensorName;
  float value;
  String unit;

  String deviceId;
  String gnssStatus;
  unsigned long gnssAgeMs;

  bool wifiConnected;
  String wifiStatus;
  String wifiSsid;
  String wifiIpAddress;
  int32_t wifiRssi;
};

void addNullableFloat(JsonObject object, const char* key, float value) {
  if (isnan(value)) {
    object[key] = nullptr;
  } else {
    object[key] = value;
  }
}

String buildGeoJson(GeoJsonMeasurement measurement) {
  StaticJsonDocument<768> doc;

  doc["type"] = "Feature";

  if (measurement.hasLocation) {
    JsonObject geometry = doc.createNestedObject("geometry");
    geometry["type"] = "Point";

    JsonArray coordinates = geometry.createNestedArray("coordinates");
    coordinates.add(measurement.longitude);
    coordinates.add(measurement.latitude);
  } else {
    doc["geometry"] = nullptr;
  }

  JsonObject properties = doc.createNestedObject("properties");
  properties["sensor_name"] = measurement.sensorName;
  addNullableFloat(properties, "value", measurement.value);
  properties["unit"] = measurement.unit;

  properties["deviceId"] = measurement.deviceId;
  addNullableFloat(properties, "temperatureC", measurement.temperatureC);
  addNullableFloat(properties, "humidityRH", measurement.humidityRH);

  properties["gnssHasFix"] = measurement.hasLocation;
  properties["gnssStatus"] = measurement.gnssStatus;
  properties["gnssAgeMs"] = measurement.gnssAgeMs;

  properties["wifiConnected"] = measurement.wifiConnected;
  properties["wifiStatus"] = measurement.wifiStatus;
  properties["wifiSsid"] = measurement.wifiSsid;
  properties["wifiIpAddress"] = measurement.wifiIpAddress;
  properties["wifiRssi"] = measurement.wifiRssi;

  String geoJson;
  serializeJson(doc, geoJson);
  return geoJson;
}
