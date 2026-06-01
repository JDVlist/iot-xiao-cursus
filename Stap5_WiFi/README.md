# WiFi

We gaan verbinden aan de WiFi.

De XIAO ESP32 C3 heeft een WiFi/IP stack onboard en kan direct gebruikt worden.

## Programmeren ...

Open `Stap4_WiFi.ino` in de Arduino IDE

### Het WiFi SSID en Wachtwoord!

Omdat deze repo publiek staat, is het "Good practice" om dat niet publiek te maken.

Open een simpele editor (je kan er niet bij via de Arduino IDE, gebruik daarom `notepad`, `VS Code` of ...) en open het bestand `arduino_secrets.h`, pas daarin het SSID en Wachtwoord aan.

<img width="549" height="172" alt="Screenshot 2025-08-24 at 13 24 23" src="https://github.com/user-attachments/assets/2c41e506-1cc1-4197-891e-92c6e61aaca6" />

Compileer en upload!

Als alles goed gaat, zie je

```
[WiFi] Connected to Geonovum-Gast
[WiFi] IP Address: 192.168.1.113
```

Als er staat:

```
[WiFi] Disconnected
[WiFi] SSID not found
...
[WiFi] SSID not found
[WiFi] Failed to connect
```

dan is waarschijnlijk het SSID of het wachtwoord fout, of.... heb je een andere probleem.

# Andere IoT protocollen

- Bluetooth
- LoRa
- ZigBee
- Thread
- LoRaWan
- NB-IoT
- Z-Wave 
