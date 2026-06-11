# Stap 6: GeoJSON

In deze stap brengen we de vorige onderdelen samen tot een bericht:

- sensorwaarde lezen
- GNSS-locatie ophalen
- WiFi verbinden
- alles samenvoegen tot een GeoJSON-bericht

We versturen het bericht nog niet met HTTP. Dat doen we later.

In deze stap printen we de GeoJSON alleen in de Serial Monitor.

Belangrijk: de `.ino` laat zien waar de data vandaan komt. Het bouwen van de GeoJSON zelf staat in een helper:

```text
../helpers/geojson.h
```

De `.ino` start de modules op, haalt actuele waarden op, en roept daarna de helper aan.

## Libraries installeren

Deze stap gebruikt libraries uit eerdere stappen en een extra JSON-library.

Installeer via de Arduino IDE:

1. Ga naar **Sketch > Include Library > Manage Libraries...**
2. Installeer deze libraries:

- **ArduinoJson** by Benoit Blanchon
- **TinyGPSPlus** by Mikal Hart
- **Adafruit AM2320 sensor library** by Adafruit
- **Adafruit Unified Sensor** by Adafruit

Als een library mist, krijg je meestal een fout zoals:

```text
fatal error: ArduinoJson.h: No such file or directory
fatal error: TinyGPSPlus.h: No such file or directory
fatal error: Adafruit_Sensor.h: No such file or directory
```

## WiFi gegevens

De WiFi-naam en het wachtwoord staan niet direct in de sketch.

Maak of controleer het bestand `arduino_secrets.h` in de hoofdmap:

```cpp
#pragma once

#define SECRET_SSID "iot-workshop"
#define SECRET_PASS "94411140"
```

Gebruik natuurlijk de gegevens van het netwerk waarmee je XIAO moet verbinden.

## Code openen

Open in de Arduino IDE:

```text
Stap6_GeoJSON/Stap6_GeoJSON.ino
```

Upload de sketch naar de XIAO ESP32C3.

Open daarna de Serial Monitor op:

```text
115200 baud
```

## Wat doet de sketch?

In `setup()` starten we alle modules:

```cpp
setupLogging();
setupSensor();
setupGNSS();
setupWiFi();
```

In `loop()` blijven GNSS en WiFi steeds updaten:

```cpp
updateGNSS();
updateWiFi();
```

Elke 5 seconden haalt de sketch de nieuwste data op:

```cpp
GNSSData gps = getGNSSData();
WiFiData wifi = getWiFiData();
float temperature = getTemperature();
float humidity = getHumidity();
```

Daarna wordt daar een GeoJSON-string van gemaakt:

```cpp
GeoJsonMeasurement measurement = getCurrentMeasurement();
String geoJson = buildGeoJson(measurement);
```

Die variabele wordt nu alleen geprint:

```cpp
Serial.println(geoJson);
```

In een volgende stap kan dezelfde `geoJson` variabele naar een backend worden verstuurd.

## Code split

De code is gesplitst in twee verantwoordelijkheden.

`Stap6_GeoJSON.ino` regelt de hardware en timing:

```cpp
setupSensor();
setupGNSS();
setupWiFi();
updateGNSS();
updateWiFi();
```

Daarna vult de sketch zelf een `GeoJsonMeasurement`:

```cpp
GeoJsonMeasurement measurement = getCurrentMeasurement();
```

Die functie staat bewust in de `.ino`, zodat je in de cursus goed kunt volgen waar sensor-, GNSS- en WiFi-data vandaan komen.

Daarna regelt `../helpers/geojson.h` het dataformaat:

```cpp
String geoJson = buildGeoJson(measurement);
```

Daardoor blijft het dynamisch. De helper heeft geen hardcoded sensorwaarden of coordinaten. Elke keer dat `getCurrentMeasurement()` wordt aangeroepen, gaan de nieuwste sensor-, GNSS- en WiFi-waarden mee naar de GeoJSON-builder.

## GeoJSON

GeoJSON is JSON voor geografische data.

Deze stap maakt een `Feature`:

```json
{
  "type": "Feature",
  "geometry": {
    "type": "Point",
    "coordinates": [4.897106, 52.110352]
  },
  "properties": {
    "temperatureC": 21.5,
    "humidityRH": 56,
    "gnssHasFix": true,
    "wifiConnected": true
  }
}
```

Let op de volgorde van de coordinaten:

```text
[longitude, latitude]
```

Dat is de GeoJSON-volgorde. Mensen noemen coordinaten vaak andersom, maar GeoJSON verwacht eerst longitude en daarna latitude.

## Geen GNSS fix

Als de GNSS-module nog geen geldige locatie heeft, wordt de geometry:

```json
"geometry": null
```

De sensorwaarden en debugstatus staan dan nog steeds in `properties`.

Dat is handig, want dan kun je zien of de sensor en WiFi al werken terwijl de GNSS-module nog zoekt naar satellieten.

## Sensor kiezen

Standaard gebruikt stap 6 de echte AM-2320 sensor:

```cpp
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"
```

Wil je zonder sensorhardware testen, wissel deze regels dan om:

```cpp
// #include "../sensors/AM-2320.h"
#include "../sensors/dummy_sensor.h"
```

De dummy sensor geeft vaste testwaarden terug.

## Verwachte output

In de Serial Monitor verschijnt ongeveer elke 5 seconden:

```text
=== GeoJSON ===
{"type":"Feature","geometry":{"type":"Point","coordinates":[4.897106,52.110352]},"properties":{"deviceId":"B0:A6:04:07:A4:9C","temperatureC":21.5,"humidityRH":56,"gnssHasFix":true,"gnssStatus":"valid GNSS fix","gnssAgeMs":3,"wifiConnected":true,"wifiStatus":"connected","wifiSsid":"iot-workshop","wifiIpAddress":"192.168.0.101","wifiRssi":-24}}
```

## Debuggen

### WiFi werkt niet

Controleer:

- staat het juiste SSID in `arduino_secrets.h`?
- staat het juiste wachtwoord in `arduino_secrets.h`?
- is het netwerk 2.4 GHz?
- is de router of hotspot dichtbij genoeg?

### Geen GNSS fix

Controleer:

- zit `GNSS TX` op `XIAO D7 / RX`?
- zit `GND` op `GND`?
- krijgt de GNSS-module voeding?
- ligt de antenne bij een raam of buiten?
- wacht lang genoeg; een eerste fix kan enkele minuten duren

### Sensor geeft null

`temperatureC` of `humidityRH` wordt `null` als de sensor geen geldige waarde teruggeeft.

Controleer:

- VCC en GND
- I2C-aansluiting
- of de juiste sensor-header actief is
- of de Adafruit libraries zijn geinstalleerd

## Waarom deze stap belangrijk is

Vanaf nu hebben we een compleet meetbericht:

- waar is het gemeten?
- wat is er gemeten?
- is het apparaat online?
- wat is de status van GNSS en WiFi?

Dit is precies de vorm die later naar een webservice gestuurd kan worden.
