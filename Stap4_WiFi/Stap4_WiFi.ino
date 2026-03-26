#include <WiFi.h>

#include "../arduino_secrets.h"

#include "../helpers/logging.h"
#include "../helpers/mac.h"

void setup() {
  setupLogging();

  // Set GPIO0 Boot button as input
  pinMode(0, INPUT);

  uint64_t chipid = ESP.getEfuseMac();  //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.print("[WiFi] MAC address: ");
  Serial.println(mac2String((byte *)&chipid));

  Serial.print("[WiFi] Connecting to ");
  Serial.println(SECRET_SSID);

  WiFi.begin(SECRET_SSID, SECRET_PASS);
  // Auto reconnect is set true as default
  // To set auto connect off, use the following function
  //    WiFi.setAutoReconnect(false);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;

  // Wait for the WiFi event
  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("[WiFi] SSID not found");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("[WiFi] Failed - WiFi not connected! Reason: Connection failed");
        return;
        break;
      case WL_CONNECTION_LOST:
        Serial.println("[WiFi] Connection was lost");
        break;
      case WL_SCAN_COMPLETED:
        Serial.println("[WiFi] Scan is completed");
        break;
      case WL_DISCONNECTED:
        Serial.println("[WiFi] Disconnected");
        break;
      case WL_CONNECTED:
        Serial.print("[WiFi] Connected to ");
        Serial.println(SECRET_SSID);
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        return;
        break;
      case WL_IDLE_STATUS:
        Serial.println("[WiFi] Idle state");
        break;
      case WL_NO_SHIELD:
        Serial.println("[WiFi] No WiFi shield is present");
        break;
      default:
        Serial.print("[WiFi] Status: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.println("[WiFi] Failed to connect");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    } else {
      numberOfTries--;
    }
  }
}

void loop() {}
