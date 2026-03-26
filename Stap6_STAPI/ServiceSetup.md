# FROST STAPI service setup

## FROST running in a Docker environment

### Docker Hub
https://hub.docker.com/r/fraunhoferiosb/frost-server

### Docker compose file:

(werkt op een Synology NAS (DSM 7.2.2) met Container Manager)

```yaml
services:
  web:
    image: fraunhoferiosb/frost-server:latest
    environment:
      - serviceRootUrl=https://xxxxxxxx/FROST-Server
      - plugins_multiDatastream.enable=false
      - http_cors_enable=true
      - http_cors_allowed_origins=*
      - persistence_db_driver=org.postgresql.Driver
      - persistence_db_url=jdbc:postgresql://database:5432/sensorthings
      - persistence_db_username=sensorthings
      - persistence_db_password=xxxxxxxx
      - persistence_autoUpdateDatabase=true
      - mqtt.Enabled=true
    ports:
      - 8080:8080
      - 1883:1883
    depends_on:
      database:
        condition: service_healthy

  database:
    image: postgis/postgis:16-3.4-alpine
    environment:
      - POSTGRES_DB=sensorthings
      - POSTGRES_USER=sensorthings
      - POSTGRES_PASSWORD=xxxxxxxx
    volumes:
      - postgis_volume:/var/lib/postgresql/data
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -d sensorthings -U sensorthings "]
      interval: 2s
      timeout: 2s
      retries: 10

volumes:
    postgis_volume:
```

## Toegang via het Internet

Via Reverse-Proxy of tunnel. 
