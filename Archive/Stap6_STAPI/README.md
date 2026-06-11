# OGC Sensor Things API

Kort: STAPI

## Het STAPI model

Gebaseerd op ISO 19156 Observations & Measurements

<img width="2880" height="1982" alt="SensorThings_API_data_model svg" src="https://github.com/user-attachments/assets/15c94d29-3abd-47fb-b2f6-31d5a88997ca" />

### Key features

- `Thing` - `Sensor` seperation
- `Location` of `Thing` can be different from `Feature of Interest`
- `Observed Properties`
- `Datastream` ties everything together

## (OData) API en MQTT

De Standaard heeft 2 manieren om het model te bevragen: HTTP(S) ODATA API (REST API in ontwikkeling) en MQTT

## Andere protocollen

- CoAP (Constrained Application Protocol)
- AMQP (Advanced Message Queuing Protocol)
- DDS (Data Distribution Service)
- LwM2M (Lightweight M2M)

[Ga naar volgende stap](EenmaligeSetup.md)
