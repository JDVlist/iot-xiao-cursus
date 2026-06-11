#include "../helpers/logging.h"
#include "../helpers/iot_wifi.h"

unsigned long lastPrint = 0;

void setup() {
  setupLogging();
  setupWiFi();
}

void loop() {
  updateWiFi();

  if (millis() - lastPrint > 5000) {
    lastPrint = millis();

    WiFiData wifi = getWiFiData();

    Serial.println();
    Serial.println("=== WiFi data ===");

    Serial.print("Connected: ");
    Serial.println(wifi.connected ? "yes" : "no");

    Serial.print("Status: ");
    Serial.println(wifi.status);

    Serial.print("SSID: ");
    Serial.println(wifi.ssid);

    Serial.print("MAC address: ");
    Serial.println(wifi.macAddress);

    if (wifi.connected) {
      Serial.print("IP address: ");
      Serial.println(wifi.ipAddress);

      Serial.print("Gateway: ");
      Serial.println(wifi.gateway);

      Serial.print("Subnet mask: ");
      Serial.println(wifi.subnetMask);

      Serial.print("DNS server: ");
      Serial.println(wifi.dnsServer);

      Serial.print("Signal strength: ");
      Serial.print(wifi.rssi);
      Serial.println(" dBm");
    } else {
      Serial.println("IP address: no connection");
      Serial.println("Signal strength: no connection");
    }
  }
}