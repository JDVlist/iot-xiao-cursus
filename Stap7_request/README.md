# Stap 7: GeoJSON versturen met POST

In deze stap sturen we de GeoJSON uit stap 6 naar een backend met een HTTP POST-request.

De backend hoeft nog niet helemaal af te zijn. De code is alvast klaar zodat je straks alleen de URL hoeft aan te passen.

## Belangrijk: gebruik geen localhost

Gebruik op de XIAO geen `localhost`.

Op je laptop betekent `localhost` je laptop.

Op de XIAO betekent `localhost` de XIAO zelf.

Gebruik daarom het IP-adres van je laptop op hetzelfde WiFi-netwerk:

```cpp
const char* BACKEND_URL = "http://192.168.0.100:8000/measurements";
```

Vervang `192.168.0.100` door het lokale IP-adres van je eigen laptop.

## Docker/backend

Als je backend in Docker draait, moet de poort naar je laptop gepubliceerd zijn:

```text
8000:8000
```

De backend moet luisteren op:

```text
0.0.0.0
```

Niet alleen op:

```text
127.0.0.1
```

## Request body

De GeoJSON gaat mee als body van de POST-request.

De code zet deze header:

```text
Content-Type: application/geo+json
```

De body ziet er ongeveer zo uit:

```json
{"type":"Feature","geometry":{"type":"Point","coordinates":[4.897106,52.110352]},"properties":{"sensor_name":"temp","value":21.5,"unit":"C","deviceId":"B0:A6:04:07:A4:9C","temperatureC":21.5,"humidityRH":56,"gnssHasFix":true,"gnssStatus":"valid GNSS fix","gnssAgeMs":3,"wifiConnected":true,"wifiStatus":"connected","wifiSsid":"iot-workshop","wifiIpAddress":"192.168.0.101","wifiRssi":-24}}
```

## Code openen

Open in de Arduino IDE:

```text
Stap7_request/Stap7_request.ino
```

Upload naar de XIAO ESP32C3 en open de Serial Monitor op:

```text
115200 baud
```

Elke 5 seconden:

- haalt de XIAO sensor-, GNSS- en WiFi-data op
- bouwt hij een GeoJSON body
- print hij de body
- probeert hij een POST-request te sturen

Als de backend nog niet draait, zie je een HTTP-fout in de Serial Monitor. Dat is voor nu prima: dan weet je in elk geval dat de requestcode en body klaarstaan.
