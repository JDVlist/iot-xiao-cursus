# Step 7: Send GeoJSON with POST

In this step, we send the GeoJSON from step 6 to a backend with an HTTP POST request.

The backend does not need to be fully finished yet. The code is already prepared, so later you only need to update the URL.

## Important: Do Not Use localhost

Do not use `localhost` on the XIAO.

On your laptop, `localhost` means your laptop.

On the XIAO, `localhost` means the XIAO itself.

Use the IP address of your laptop on the same Wi-Fi network instead:

```cpp
const char* BACKEND_URL = "http://192.168.0.100:8000/measurements";
```

Replace `192.168.0.100` with the local IP address of your own laptop.

## Docker/backend

If your backend runs in Docker, the port must be published to your laptop:

```text
8000:8000
```

The backend must listen on:

```text
0.0.0.0
```

Not only on:

```text
127.0.0.1
```

## Request Body

The GeoJSON is sent as the body of the POST request.

The code sets this header:

```text
Content-Type: application/geo+json
```

The body looks roughly like this:

```json
{"type":"Feature","geometry":{"type":"Point","coordinates":[4.897106,52.110352]},"properties":{"sensor_name":"temp","value":21.5,"unit":"C","deviceId":"B0:A6:04:07:A4:9C","temperatureC":21.5,"humidityRH":56,"gnssHasFix":true,"gnssStatus":"valid GNSS fix","gnssAgeMs":3,"wifiConnected":true,"wifiStatus":"connected","wifiSsid":"iot-workshop","wifiIpAddress":"192.168.0.101","wifiRssi":-24}}
```

## Open the Code

Open this file in the Arduino IDE:

```text
Stap7_request/Stap7_request.ino
```

Upload it to the XIAO ESP32C3 and open the Serial Monitor at:

```text
115200 baud
```

Every 5 seconds:

- the XIAO reads sensor, GNSS, and Wi-Fi data
- it builds a GeoJSON body
- it prints the body
- it tries to send a POST request

If the backend is not running yet, you will see an HTTP error in the Serial Monitor. That is fine for now: it means the request code and body are ready.
