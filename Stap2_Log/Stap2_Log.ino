#include <WiFi.h>

#include "../helpers/mac.h" // contains the mac2String function

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  delay(50);

  uint64_t chipid = ESP.getEfuseMac();  //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.println(mac2String((byte*)&chipid));
  // lookup MAC detail with https://macaddress.io
}

void loop() {
}