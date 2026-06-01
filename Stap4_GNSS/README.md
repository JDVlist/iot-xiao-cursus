# Stap 4: GNSS

In deze stap lezen we een GNSS-module uit met de XIAO ESP32C3.

GNSS staat voor Global Navigation Satellite System. GPS is daar een bekend voorbeeld van, maar moderne modules kunnen vaak ook signalen van andere satellietsystemen gebruiken. De module geeft uiteindelijk een positie terug in WGS 84: latitude en longitude in graden.

In deze stap bouwen we nog geen GeoJSON. We testen alleen of de XIAO goede locatiegegevens uit de GNSS-module kan lezen. De herbruikbare code staat in `../helpers/gnss.h`; de testcode voor deze stap staat in `Stap4_GNSS.ino`.

## Library installeren

Deze stap gebruikt de Arduino-library `TinyGPSPlus`.

Installeer die via de Arduino IDE:

1. Ga naar **Sketch > Include Library > Manage Libraries...**
2. Zoek naar `TinyGPSPlus`
3. Installeer **TinyGPSPlus** van **Mikal Hart**

Als deze library ontbreekt, krijg je een fout zoals:

```text
fatal error: TinyGPSPlus.h: No such file or directory
```

## Aansluiten

De GNSS-module communiceert via een seriele verbinding. Daarvoor gebruiken we de UART van de XIAO.

Gebruik deze aansluiting:

```text
GNSS VCC  -> XIAO 5V
GNSS GND  -> XIAO GND
GNSS TX   -> XIAO D7 / RX
```

Voor deze stap lezen we alleen data uit de GNSS-module. Daarom is de `TX`-draad van de GNSS-module genoeg.

## RX en TX

`TX` betekent transmit: deze pin verzendt data.

`RX` betekent receive: deze pin ontvangt data.

Daarom sluit je niet `TX` op `TX` aan. De data moet juist van de ene kant naar de andere kant:

```text
GNSS TX  ->  XIAO RX
```

De GNSS-module praat, de XIAO luistert. De zend-pin van de GNSS-module moet dus naar de ontvang-pin van de XIAO.

Als je later ook commando's naar de GNSS-module wilt sturen, gebruik je ook de andere richting:

```text
XIAO TX  ->  GNSS RX
```

Maar voor deze cursus lezen we vooral locatiegegevens uit. Dan is `GNSS TX -> XIAO RX` het belangrijkste.

In de code staat:

```cpp
#define RXPin D7
#define TXPin D6
```

Dat betekent: de XIAO luistert naar de GNSS-module op `D7`.

## Code

Open in de Arduino IDE:

```text
Stap4_GNSS/Stap4_GNSS.ino
```

De sketch doet drie dingen:

```cpp
setupLogging();
setupGNSS();
updateGNSS();
```

Daarna haalt hij de laatste GNSS-data op:

```cpp
GNSSData gps = getGNSSData();
```

Die data komt uit `../helpers/gnss.h`.

## Wat is een fix?

Een fix betekent dat de GNSS-module genoeg satellietinformatie heeft om een positie te berekenen.

Er zijn dus twee verschillende stappen:

```text
1. De XIAO ontvangt seriele data van de GNSS-module
2. De GNSS-module heeft een geldige positie berekend
```

De eerste stap kun je zien aan `Chars processed`. Als die waarde oploopt, ontvangt de XIAO data.

De tweede stap zie je aan `Fix: yes`. Dan zijn latitude en longitude geldig.

## Output

Open na het uploaden de Serial Monitor op `115200 baud`.

Als alles werkt, zie je ongeveer dit:

```text
=== GNSS status ===
Fix: yes
Status: valid GNSS fix
Latitude: 52.110352
Longitude: 4.897106
Age ms: 3

--- Debug ---
Chars processed: 9261
Sentences with fix: 48
Failed checksums: 25
```

De coordinaten zijn WGS 84:

```text
Latitude  = breedtegraad
Longitude = lengtegraad
```

Voor GeoJSON gebruiken we later dezelfde waarden, maar dan in deze volgorde:

```json
"coordinates": [longitude, latitude]
```

GeoJSON gebruikt dus eerst longitude en daarna latitude.

## Debuggen

Gebruik de debugregels om te bepalen waar het probleem zit.

### Geen data

```text
Fix: no
Status: no GNSS data received; check wiring, power, and RX/TX pins
Chars processed: 0
Sentences with fix: 0
Failed checksums: 0
```

De XIAO ontvangt geen data van de GNSS-module.

Controleer dan:

- krijgt de GNSS-module voeding?
- zit `GND` op `GND`?
- zit `GNSS TX` op `XIAO D7 / RX`?
- is de draad niet los?
- staat de baudrate op `9600`?

### Wel data, geen fix

```text
Fix: no
Status: GNSS data received, waiting for valid location fix
Chars processed: 1200
Sentences with fix: 0
```

De verbinding werkt, maar de GNSS-module heeft nog geen positie.

Probeer dan:

- leg de module dichter bij een raam
- ga naar buiten
- leg de antenne naar boven
- wacht een paar minuten

Een eerste fix kan traag zijn, vooral binnen.

### Wel fix

```text
Fix: yes
Status: valid GNSS fix
```

Dan is de stap gelukt. De latitude en longitude zijn bruikbaar voor de integratiestap.

## Checksum failures

`Failed checksums` betekent dat sommige ontvangen NMEA-zinnen beschadigd of onvolledig waren.

Een paar checksum failures is niet meteen erg. Als de waarde heel snel blijft oplopen, controleer dan de draadjes, voeding en verbindingen.

Als `Fix: yes` verschijnt en de coordinaten stabiel zijn, is de GNSS-stap goed genoeg om verder te gaan.
