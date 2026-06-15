# IoT Workshop

Create an `arduino_secrets.h` file with your Wi-Fi credentials. Do not commit this file to Git:

```cpp
#pragma once

#define SECRET_SSID "iot-workshop"
#define SECRET_PASS "94411140"
```

## Arduino Libraries

Install these libraries in the Arduino IDE via **Sketch > Include Library > Manage Libraries**:

- **ArduinoJson** by Benoit Blanchon, used in `Stap6_GeoJSON` to build the GeoJSON message.
- **Adafruit AM2320 sensor library** by Adafruit, including all dependencies. Used for the AM-2320 temperature and humidity sensor.
- **Adafruit Unified Sensor** by Adafruit, required by the AM2320 sensor library.
- **TinyGPSPlus** by Mikal Hart, used in `Stap4_GNSS` to read GNSS/GPS data.

## FAQ

### The Arduino IDE forgot my board. How do I set up the XIAO ESP32C3 again?

Sometimes the Arduino IDE no longer remembers the selected board or COM port. Set them again:

1. Connect the XIAO ESP32C3 with a USB-C cable that supports data.
2. Open the Arduino IDE.
3. Check that the ESP32 board package is installed:
   - Go to **Tools > Board > Boards Manager...**
   - Search for `esp32`
   - Install **esp32** by **Espressif Systems** if it is not installed yet.
4. Select the board:
   - Go to **Tools > Board > ESP32 Arduino**
   - Select **XIAO_ESP32C3**
5. Select the port:
   - Go to **Tools > Port**
   - Select the COM port that appears when the XIAO is connected, for example `COM3`, `COM4`, or higher.
6. Click **Verify** to compile.
7. Click **Upload** to upload the sketch to the board.

If uploading gets stuck on `Connecting...`, try this:

- close the Serial Monitor
- unplug the USB cable and connect it again
- select the correct port again
- briefly press **RESET** when `Connecting...` appears
- try another USB cable if it still fails

More background is available in the Seeed Studio documentation for the XIAO ESP32C3:

https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/

## Context

An introduction to the world of IoT, from a Thing to an interoperable service.

## Environment

We are in a safe learning environment. There are no bad questions.
If you do not understand something, ask.

Enjoy the day together.

## Content

- Introduction to the IoT world with Arduino
- Basic wiring and programming for the Thing
- Make the Thing communicate with GPS/GNSS
- Connect our Thing to the internet via Wi-Fi
- Make the Thing communicate with a sensor
- Set up a Docker web service for measurements
- Make the Thing measure and send a measurement
- Inspect all our data
- Conclusion

https://api.riot-os.org/group__boards__seeedstudio-xiao-esp32c3.html
