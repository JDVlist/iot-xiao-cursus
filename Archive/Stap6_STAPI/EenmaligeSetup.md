# Eenmalige setup

## Connectie met de STAPI Server

Eerst via de browser: https://iot.aardvark.myds.me/FROST-Server/v1.1

Maar dit is GET only en niet echt gebruiksvriendeljk - het kan beter met Bruno.

## Bruno!

We gebruiken Bruno om te communiceren met de STAPI Server

Open Bruno op je computer en open de `Bruno` folder van `Stap6_STAPI`

Klik op `Get Landing Page` (Zelfde vraag als via de browser hierboven).

> Note: als `{{Endpoint}}` rood is, klik dan op `No environment` en kies `Amersfoort`. {{Endpoint}} wordt nu groen.

<img width="1274" height="739" alt="Screenshot 2025-08-25 at 20 58 31" src="https://github.com/user-attachments/assets/a149c198-86fe-445f-867b-1151b31ddb69" />

## Aanmaken van het Thing

We hebben ons Thing op de tafel staan, maar er is nog geen 'afbeelding' gemaakt op de STAPI server.

In de folder `New Thing & Datastream` zie je verschillende HTTP calls voor het aanmaken van het Thing met zijn Sensor.

> Klik nog niet op execute (->)

### Kies de Sensor die je hebt, bv ADXL-345

```json
{
  "name": "???????",
  "description": "XIAO ESP32 C3",
  "properties": {},
  "Locations": [
    {
      "name": "xxx",
      "description": "yyy",
      "encodingType": "application/vnd.geo+json",
      "location": {
        "type": "Point",
        "coordinates": [
          6.020759,
          50.754699
        ]
      }
    }
  ],
  "Datastreams": [
    {
      "name": "Acceleration DS",
      "description": "Datastream for recording Acceleration",
      "observationType": "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement",
      "unitOfMeasurement": {
        "name": "Meter per square second",
        "symbol": "m/s²",
        "definition": "https://qudt.org/vocab/unit/M-PER-SEC2"
      },
      "Sensor": {
        "@iot.id": 1
      },
      "ObservedProperty": {
        "@iot.id": 9
      }
    }
  ]
}
```

Verander:
- naam van je Thing (????????) naar een naam die je leuk vindt. (je kan ook het MAC gebruiken, zie sticker op je breadboard)
- maak zelf een Locatie aan. Waarom, we hebben toch een GPS!? Ja, maar dan ligt alles op eenzelfde plek - zo saai straks.

### Execute HTTP call

Check eerst even met Bart en klik `->` maar 1 keer!!

## Kijken naar alle Things

[Things op een kaartje](https://raw.githack.com/lathoub/UISTA/main/index.html)
