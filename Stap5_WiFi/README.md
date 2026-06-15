# Wi-Fi

We are going to connect to Wi-Fi.

The XIAO ESP32-C3 has an onboard Wi-Fi/IP stack and can be used directly.

## Programming

Open `Stap5_WiFi.ino` in the Arduino IDE.

### Wi-Fi SSID and Password

Because this repository is public, it is good practice not to publish credentials.

Open a simple editor. You cannot edit this file through the Arduino IDE, so use `notepad`, `VS Code`, or another editor. Open `arduino_secrets.h` and update the SSID and password there.

<img width="549" height="172" alt="Screenshot 2025-08-24 at 13 24 23" src="https://github.com/user-attachments/assets/2c41e506-1cc1-4197-891e-92c6e61aaca6" />

Compile and upload.

If everything works, you will see:

```text
[WiFi] Connected to Geonovum-Gast
[WiFi] IP Address: 192.168.1.113
```

If you see:

```text
[WiFi] Disconnected
[WiFi] SSID not found
...
[WiFi] SSID not found
[WiFi] Failed to connect
```

then the SSID or password is probably wrong, or there is another problem.

# Other IoT Protocols

- Bluetooth
- LoRa
- Zigbee
- Thread
- LoRaWAN
- NB-IoT
- Z-Wave
