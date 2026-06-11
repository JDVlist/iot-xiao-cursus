# Iot Workshop 

Create a arduino_secrets.h containing your wifi creds (dont sync it with git):

```cpp
#pragma once

#define SECRET_SSID "iot-workshop"
#define SECRET_PASS "94411140"
```

## Arduino libraries

Install these libraries in the Arduino IDE via **Sketch > Include Library > Manage Libraries**:

- **ArduinoJson** by Benoit Blanchon, used in `Stap6_integratie` to build the GeoJSON message.
- **Adafruit AM2320 sensor library** by Adafruit, and all it's dependencies. Used for the AM-2320 temperature and humidity sensor.
- **Adafruit Unified Sensor** by Adafruit, required by the AM2320 sensor library.
- **TinyGPSPlus** by Mikal Hart, used in `Stap4_GNSS` to read GNSS/GPS data.

## FAQ

### Arduino IDE is mijn board vergeten. Hoe stel ik de XIAO ESP32C3 opnieuw in?

Soms onthoudt de Arduino IDE het gekozen board of de COM-poort niet meer. Stel ze dan opnieuw in:

1. Sluit de XIAO ESP32C3 aan met een USB-C kabel die ook data ondersteunt.
2. Open de Arduino IDE.
3. Controleer of de ESP32 board package is geinstalleerd:
   - Ga naar **Tools > Board > Boards Manager...**
   - Zoek op `esp32`
   - Installeer **esp32** by **Espressif Systems** als die nog niet is geinstalleerd.
4. Kies het board:
   - Ga naar **Tools > Board > ESP32 Arduino**
   - Kies **XIAO_ESP32C3**
5. Kies de poort:
   - Ga naar **Tools > Port**
   - Kies de COM-poort die verschijnt als de XIAO is aangesloten, bijvoorbeeld `COM3`, `COM4` of hoger.
6. Klik op **Verify** om te compileren.
7. Klik op **Upload** om de sketch op het board te zetten.

Als uploaden blijft hangen op `Connecting...`, probeer dan:

- sluit de Serial Monitor
- trek de USB-kabel los en sluit opnieuw aan
- kies opnieuw de juiste poort
- druk kort op **RESET** zodra `Connecting...` verschijnt
- probeer een andere USB-kabel als het blijft misgaan

Meer achtergrond staat in de Seeed Studio documentatie voor de XIAO ESP32C3:

https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/

## Context
Introductie in de wereld van IoT, van Thing tot (interoperable) service.

## Omgeving
We zitten in een veilige omgeving, er zijn geen domme vragen!
Begrijpt u iets niet, vraag het!

Geniet van de dag samen.

## Content
- Introductie IoT (in de Arduino) wereld
- Basis aansluiting en programmeren van het Thing
- Thing laten praten met een GPS/GNSS
- Ons Thing en het Internet via WiFi
- Thing laten praten met een sensor
- Opzetten van een Docker webservice voor de metingen
- Thing laten meten en meeting versturen
- Kijken naar al onze data
- Conclusie



https://api.riot-os.org/group__boards__seeedstudio-xiao-esp32c3.html 
