# Het wordt straffer!

## Positie bepaling via GPS - of beter: GNSS[1]

We hier tenslotte bij Geonovum en vinden locatie leuk!

## Het breadboard:

First things first! We gebruiken een breadboard en draden om verbindingen te leggen tussen componenten.

(Afbeelding van https://www.budgetronics.eu/nl/hoe-werkt-een-breadboard)

![1__38__ms1 rvf_1](https://github.com/user-attachments/assets/c3629196-3a47-4481-82ab-f3e0f5d9ce59)

<img width="670" height="408" alt="1__38__ms1 rvf_2" src="https://github.com/user-attachments/assets/8b4b6081-9574-4e0f-bcb9-459f583d8935" />

![1__38__ms1 rvf_3](https://github.com/user-attachments/assets/fcf70c9c-df58-4496-9675-d9782186cd3f)

Belangrijk: verbindt massa nooit direct met de voeding! Als je het wel doet, dan ruik je het snel....

## Het Thing en de GPS - hoe werkt dat?

De GPS (of meer algemeen: GNSS[1]) stuurt een serieel (1 draad voor zenden (TX, de blauwe draad) en 1 draad (optioneel) voor ontvangen) bytes door volgens het [NMEA protocol](https://nl.wikipedia.org/wiki/NMEA-0183).

<img width="890" height="636" alt="XIAO_GNSS_bb" src="https://github.com/user-attachments/assets/d13e407d-406a-4770-bb04-f43a2fa420d2" />

>Let op: de GPS wordt gevoed met 5V - het signaal terug naar de XIAO is slecht 3.3V

De blauwe TX draad (van de GPS) gaat naar het RX contact (pin D7) van de XIAO die intern verbonden is met de [UART](https://nl.wikipedia.org/wiki/UART).

<img width="1280" height="720" alt="pin_map-2" src="https://github.com/user-attachments/assets/50a36612-d3dc-4736-bbd5-2393f92ff132" />


## Programmeren maar

Open ArduinoIDE en laad het `Stap3_GNSS.ino` bestand; compileer en upload!

[1] GPS, Beidou, Glonass, Galileo, QZSS, SBAS ...











