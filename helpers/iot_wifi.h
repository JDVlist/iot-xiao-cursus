#pragma once

#include <WiFi.h>

#include "../arduino_secrets.h"

// How often the helper tries to reconnect when WiFi is disconnected.
const unsigned long WIFI_RECONNECT_INTERVAL_MS = 30000;

// How often the helper prints a short status line while connecting.
const unsigned long WIFI_STATUS_PRINT_INTERVAL_MS = 2000;

struct WiFiData {
  bool connected;
  const char* status;
  String ssid;
  String ipAddress;
  String gateway;
  String subnetMask;
  String dnsServer;
  String macAddress;
  int32_t rssi;
};

bool wifiWasConnected = false;
unsigned long lastWiFiReconnectAttempt = 0;
unsigned long lastWiFiStatusPrint = 0;
const char* wifiStatusText = "not started";

const char* wifiStatusToString(wl_status_t status) {
  switch (status) {
    case WL_IDLE_STATUS:     return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:   return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:  return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:       return "WL_CONNECTED";
    case WL_CONNECT_FAILED:  return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:    return "WL_DISCONNECTED";
    case WL_NO_SHIELD:       return "WL_NO_SHIELD";
    default:                 return "UNKNOWN_STATUS";
  }
}

const char* encryptionTypeToString(wifi_auth_mode_t type) {
  switch (type) {
    case WIFI_AUTH_OPEN:          return "Open";
    case WIFI_AUTH_WEP:           return "WEP";
    case WIFI_AUTH_WPA_PSK:       return "WPA";
    case WIFI_AUTH_WPA2_PSK:      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:  return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2 Enterprise";
    case WIFI_AUTH_WPA3_PSK:      return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
    default:                      return "Unknown";
  }
}

void printWiFiIntro() {
  Serial.println();
  Serial.println("=== WiFi status ===");
  Serial.print("[WiFi] Target SSID     : ");
  Serial.println(SECRET_SSID);
  Serial.print("[WiFi] Password length : ");
  Serial.println(strlen(SECRET_PASS));
  Serial.println("[WiFi] Password is not printed");
}

void scanWiFiNetworks() {
  Serial.println("[WiFi] Scanning for nearby networks...");

  int networkCount = WiFi.scanNetworks(false, true);

  if (networkCount < 0) {
    Serial.println("[WiFi] Scan failed");
    return;
  }

  if (networkCount == 0) {
    Serial.println("[WiFi] No networks found");
    return;
  }

  bool targetFound = false;

  Serial.print("[WiFi] Found networks: ");
  Serial.println(networkCount);

  for (int i = 0; i < networkCount; i++) {
    String ssid = WiFi.SSID(i);
    bool isTarget = (ssid == SECRET_SSID);

    if (isTarget) {
      targetFound = true;
    }

    Serial.print("  ");
    Serial.print(i + 1);
    Serial.print(". SSID: ");
    Serial.print(ssid);
    Serial.print(" | RSSI: ");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" dBm | Security: ");
    Serial.print(encryptionTypeToString(WiFi.encryptionType(i)));

    if (isTarget) {
      Serial.print(" <-- target");
    }

    Serial.println();
  }

  if (targetFound) {
    Serial.println("[WiFi] Target SSID was found");
  } else {
    Serial.println("[WiFi] Target SSID was NOT found");
  }

  WiFi.scanDelete();
}

void startWiFiConnection() {
  Serial.print("[WiFi] Connecting to ");
  Serial.println(SECRET_SSID);

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  lastWiFiReconnectAttempt = millis();
  lastWiFiStatusPrint = 0;
  wifiStatusText = "connecting";
}

void setupWiFi() {
  printWiFiIntro();

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  WiFi.disconnect(true);
  delay(500);

  Serial.print("[WiFi] MAC address     : ");
  Serial.println(WiFi.macAddress());

  scanWiFiNetworks();
  startWiFiConnection();
}

void updateWiFi() {
  wl_status_t currentStatus = WiFi.status();
  bool connectedNow = (currentStatus == WL_CONNECTED);

  if (connectedNow && !wifiWasConnected) {
    wifiWasConnected = true;
    wifiStatusText = "connected";

    Serial.println("[WiFi] Connected");
    Serial.print("[WiFi] IP address      : ");
    Serial.println(WiFi.localIP());
    Serial.print("[WiFi] Signal strength : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }

  if (!connectedNow && wifiWasConnected) {
    wifiWasConnected = false;
    wifiStatusText = "connection lost";

    Serial.println("[WiFi] Connection lost");
  }

  if (!connectedNow && millis() - lastWiFiStatusPrint > WIFI_STATUS_PRINT_INTERVAL_MS) {
    lastWiFiStatusPrint = millis();

    Serial.print("[WiFi] Status          : ");
    Serial.print(wifiStatusToString(currentStatus));
    Serial.print(" (");
    Serial.print(currentStatus);
    Serial.println(")");
  }

  if (!connectedNow && millis() - lastWiFiReconnectAttempt > WIFI_RECONNECT_INTERVAL_MS) {
    Serial.println("[WiFi] Trying to reconnect...");
    startWiFiConnection();
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

WiFiData getWiFiData() {
  WiFiData data;

  data.connected = isWiFiConnected();
  data.status = wifiStatusText;
  data.ssid = WiFi.SSID();
  data.macAddress = WiFi.macAddress();

  if (data.connected) {
    data.ipAddress = WiFi.localIP().toString();
    data.gateway = WiFi.gatewayIP().toString();
    data.subnetMask = WiFi.subnetMask().toString();
    data.dnsServer = WiFi.dnsIP().toString();
    data.rssi = WiFi.RSSI();
  } else {
    data.ipAddress = "";
    data.gateway = "";
    data.subnetMask = "";
    data.dnsServer = "";
    data.rssi = 0;
  }

  return data;
}