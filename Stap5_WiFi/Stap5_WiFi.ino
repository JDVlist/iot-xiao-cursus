#include <WiFi.h>

#include "../arduino_secrets.h"
#include "../helpers/logging.h"

// --------------------------------------------------
// Configuration
// --------------------------------------------------

const int WIFI_CONNECT_RETRIES = 20;
const int WIFI_RETRY_DELAY_MS = 500;

// --------------------------------------------------
// Helper functions for readable debug output
// --------------------------------------------------

const char* wifiStatusToString(wl_status_t status) {
  switch (status) {
    case WL_IDLE_STATUS:    return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:  return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:      return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:   return "WL_DISCONNECTED";
    case WL_NO_SHIELD:      return "WL_NO_SHIELD";
    default:                return "UNKNOWN_STATUS";
  }
}

const char* encryptionTypeToString(wifi_auth_mode_t type) {
  switch (type) {
    case WIFI_AUTH_OPEN:           return "Open";
    case WIFI_AUTH_WEP:            return "WEP";
    case WIFI_AUTH_WPA_PSK:        return "WPA";
    case WIFI_AUTH_WPA2_PSK:       return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:   return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:return "WPA2 Enterprise";
    case WIFI_AUTH_WPA3_PSK:       return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:  return "WPA2/WPA3";
    default:                       return "Unknown";
  }
}

// --------------------------------------------------
// Debug helpers
// --------------------------------------------------

void printWiFiIntro() {
  Serial.println();
  Serial.println("=== ESP32 WiFi Debug Demo ===");
  Serial.printf("[WiFi] Target SSID       : %s\n", SECRET_SSID);
  Serial.printf("[WiFi] Password length   : %u\n", (unsigned int)strlen(SECRET_PASS));
  Serial.println("[WiFi] Note: password itself is not printed for security reasons");
}

void printDeviceInfo() {
  Serial.printf("[WiFi] Station MAC       : %s\n", WiFi.macAddress().c_str());
}

void scanNetworksAndHighlightTarget() {
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

  Serial.printf("[WiFi] Found %d network(s):\n", networkCount);

  bool targetFound = false;

  for (int i = 0; i < networkCount; i++) {
    String ssid = WiFi.SSID(i);
    int32_t rssi = WiFi.RSSI(i);
    wifi_auth_mode_t enc = WiFi.encryptionType(i);

    bool isTarget = (ssid == SECRET_SSID);
    if (isTarget) {
      targetFound = true;
    }

    Serial.printf(
      "  %2d. SSID: %-32s RSSI: %4d dBm  ENC: %-10s %s\n",
      i + 1,
      ssid.c_str(),
      rssi,
      encryptionTypeToString(enc),
      isTarget ? "<-- target" : ""
    );
  }

  if (targetFound) {
    Serial.printf("[WiFi] Target SSID '%s' was found in the scan\n", SECRET_SSID);
  } else {
    Serial.printf("[WiFi] Target SSID '%s' was NOT found in the scan\n", SECRET_SSID);
  }

  WiFi.scanDelete();
}

void printConnectionDetails() {
  Serial.println("[WiFi] Connection successful");
  Serial.printf("[WiFi] Connected SSID    : %s\n", WiFi.SSID().c_str());
  Serial.printf("[WiFi] IP address        : %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("[WiFi] Gateway           : %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.printf("[WiFi] Subnet mask       : %s\n", WiFi.subnetMask().toString().c_str());
  Serial.printf("[WiFi] DNS server        : %s\n", WiFi.dnsIP().toString().c_str());
  Serial.printf("[WiFi] Signal strength   : %d dBm\n", WiFi.RSSI());
}

void printFailureDiagnosis(wl_status_t status) {
  Serial.println("[WiFi] Connection failed");
  Serial.printf("[WiFi] Final status      : %s (%d)\n", wifiStatusToString(status), status);

  switch (status) {
    case WL_NO_SSID_AVAIL:
      Serial.println("[WiFi] Diagnosis        : SSID was not found");
      Serial.println("[WiFi] Check            : network name, router distance, 2.4 GHz availability");
      break;

    case WL_CONNECT_FAILED:
      Serial.println("[WiFi] Diagnosis        : authentication or association failed");
      Serial.println("[WiFi] Check            : password, router security mode, weak signal");
      break;

    case WL_CONNECTION_LOST:
      Serial.println("[WiFi] Diagnosis        : connection was established but then lost");
      Serial.println("[WiFi] Check            : unstable signal, power supply, router stability");
      break;

    case WL_DISCONNECTED:
      Serial.println("[WiFi] Diagnosis        : device never completed connection");
      Serial.println("[WiFi] Check            : signal quality, credentials, router settings");
      break;

    default:
      Serial.println("[WiFi] Diagnosis        : unknown failure mode");
      Serial.println("[WiFi] Check            : serial logs, scan results, router settings");
      break;
  }
}

// --------------------------------------------------
// WiFi connection logic
// --------------------------------------------------

bool connectToWiFi() {
  Serial.println("[WiFi] Starting station mode...");
  WiFi.mode(WIFI_STA);

  // Helpful during development/debugging
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  // Clear any previous connection state
  WiFi.disconnect(true);
  delay(1000);

  printDeviceInfo();
  scanNetworksAndHighlightTarget();

  Serial.printf("[WiFi] Connecting to '%s'...\n", SECRET_SSID);
  WiFi.begin(SECRET_SSID, SECRET_PASS);

  wl_status_t previousStatus = (wl_status_t)(-1);

  for (int attempt = 1; attempt <= WIFI_CONNECT_RETRIES; attempt++) {
    wl_status_t currentStatus = WiFi.status();

    if (currentStatus != previousStatus) {
      Serial.printf(
        "[WiFi] Attempt %02d/%02d | Status changed -> %s (%d)\n",
        attempt,
        WIFI_CONNECT_RETRIES,
        wifiStatusToString(currentStatus),
        currentStatus
      );
      previousStatus = currentStatus;
    } else {
      Serial.printf(
        "[WiFi] Attempt %02d/%02d | Status: %s (%d)\n",
        attempt,
        WIFI_CONNECT_RETRIES,
        wifiStatusToString(currentStatus),
        currentStatus
      );
    }

    if (currentStatus == WL_CONNECTED) {
      printConnectionDetails();
      return true;
    }

    delay(WIFI_RETRY_DELAY_MS);
  }

  printFailureDiagnosis(WiFi.status());
  return false;
}

// --------------------------------------------------
// Arduino entry points
// --------------------------------------------------

void setup() {
  setupLogging();
  delay(1000);

  printWiFiIntro();

  bool connected = connectToWiFi();

  if (!connected) {
    Serial.println("[WiFi] Tip              : Move closer to the router or try a phone hotspot");
  }
}

void loop() {
  // Empty for this demo.
}