# Step 6: GeoJSON

In this step, we combine the previous parts into one message:

- read the sensor value
- get the GNSS location
- connect to Wi-Fi
- combine everything into a GeoJSON message

We do not send the message with HTTP yet. That comes later.

In this step, we only print the GeoJSON in the Serial Monitor.

Important: the `.ino` shows where the data comes from. The GeoJSON builder itself is in a helper:

```text
../helpers/geojson.h
```

The `.ino` starts the modules, reads current values, and then calls the helper.

## Install Libraries

This step uses libraries from earlier steps and one extra JSON library.

Install them through the Arduino IDE:

1. Go to **Sketch > Include Library > Manage Libraries...**
2. Install these libraries:

- **ArduinoJson** by Benoit Blanchon
- **TinyGPSPlus** by Mikal Hart
- **Adafruit AM2320 sensor library** by Adafruit
- **Adafruit Unified Sensor** by Adafruit

If a library is missing, you usually get an error such as:

```text
fatal error: ArduinoJson.h: No such file or directory
fatal error: TinyGPSPlus.h: No such file or directory
fatal error: Adafruit_Sensor.h: No such file or directory
```

## Wi-Fi Credentials

The Wi-Fi name and password are not written directly in the sketch.

Create or check the `arduino_secrets.h` file in the repository root:

```cpp
#pragma once

#define SECRET_SSID "iot-workshop"
#define SECRET_PASS "94411140"
```

Use the credentials for the network that your XIAO should connect to.

## Open the Code

Open this file in the Arduino IDE:

```text
Stap6_GeoJSON/Stap6_GeoJSON.ino
```

Upload the sketch to the XIAO ESP32C3.

Then open the Serial Monitor at:

```text
115200 baud
```

## What Does the Sketch Do?

In `setup()`, we start all modules:

```cpp
setupLogging();
setupSensor();
setupGNSS();
setupWiFi();
```

In `loop()`, GNSS and Wi-Fi keep updating:

```cpp
updateGNSS();
updateWiFi();
```

Every 5 seconds, the sketch reads the latest data:

```cpp
GNSSData gps = getGNSSData();
WiFiData wifi = getWiFiData();
float temperature = getTemperature();
float humidity = getHumidity();
```

Then it turns that into a GeoJSON string:

```cpp
GeoJsonMeasurement measurement = getCurrentMeasurement();
String geoJson = buildGeoJson(measurement);
```

For now, that variable is only printed:

```cpp
Serial.println(geoJson);
```

In a later step, the same `geoJson` variable can be sent to a backend.

## Code Split

The code is split into two responsibilities.

`Stap6_GeoJSON.ino` handles hardware and timing:

```cpp
setupSensor();
setupGNSS();
setupWiFi();
updateGNSS();
updateWiFi();
```

Then the sketch fills a `GeoJsonMeasurement`:

```cpp
GeoJsonMeasurement measurement = getCurrentMeasurement();
```

That function is intentionally in the `.ino`, so you can clearly follow where sensor, GNSS, and Wi-Fi data come from during the course.

Then `../helpers/geojson.h` handles the data format:

```cpp
String geoJson = buildGeoJson(measurement);
```

This keeps it dynamic. The helper has no hardcoded sensor values or coordinates. Every time `getCurrentMeasurement()` is called, the latest sensor, GNSS, and Wi-Fi values are passed to the GeoJSON builder.

## GeoJSON

GeoJSON is JSON for geographic data.

This step creates a `Feature`:

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
    "sensor_name": "temp",
    "value": 21.5,
    "unit": "C"
  }
}
```

Note the coordinate order:

```text
[longitude, latitude]
```

That is the GeoJSON order. People often say coordinates the other way around, but GeoJSON expects longitude first and latitude second.

## No GNSS Fix

If the GNSS module does not have a valid location yet, the geometry becomes:

```json
"geometry": null
```

The sensor values and debug status are still included in `properties`.

That is useful because you can see whether the sensor and Wi-Fi are already working while the GNSS module is still searching for satellites.

## Choose a Sensor

By default, step 6 uses the real AM-2320 sensor:

```cpp
#include "../sensors/AM-2320.h"
// #include "../sensors/dummy_sensor.h"
```

If you want to test without sensor hardware, swap those lines:

```cpp
// #include "../sensors/AM-2320.h"
#include "../sensors/dummy_sensor.h"
```

The dummy sensor returns fixed test values.

## Expected Output

In the Serial Monitor, about every 5 seconds you will see:

```text
=== GeoJSON ===
{"type":"Feature","geometry":{"type":"Point","coordinates":[4.897106,52.110352]},"properties":{"sensor_name":"temp","value":21.5,"unit":"C","deviceId":"B0:A6:04:07:A4:9C","temperatureC":21.5,"humidityRH":56,"gnssHasFix":true,"gnssStatus":"valid GNSS fix","gnssAgeMs":3,"wifiConnected":true,"wifiStatus":"connected","wifiSsid":"iot-workshop","wifiIpAddress":"192.168.0.101","wifiRssi":-24}}
```

## Debugging

### Wi-Fi Does Not Work

Check:

- is the correct SSID in `arduino_secrets.h`?
- is the correct password in `arduino_secrets.h`?
- is the network 2.4 GHz?
- is the router or hotspot close enough?

### No GNSS Fix

Check:

- is `GNSS TX` connected to `XIAO D7 / RX`?
- is `GND` connected to `GND`?
- does the GNSS module have power?
- is the antenna near a window or outside?
- wait long enough; a first fix can take several minutes

### Sensor Returns Null

`temperatureC` or `humidityRH` becomes `null` if the sensor does not return a valid value.

Check:

- VCC and GND
- I2C wiring
- whether the correct sensor header is active
- whether the Adafruit libraries are installed

## Why This Step Matters

From now on, we have a complete measurement message:

- where was it measured?
- what was measured?
- is the device online?
- what is the GNSS and Wi-Fi status?

This is exactly the shape that can later be sent to a web service.
