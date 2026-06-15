# Step 4: GNSS

In this step, we read a GNSS module with the XIAO ESP32C3.

GNSS stands for Global Navigation Satellite System. GPS is a well-known example, but modern modules can often use signals from other satellite systems too. The module eventually returns a position in WGS 84: latitude and longitude in degrees.

In this step, we do not build GeoJSON yet. We only test whether the XIAO can read good location data from the GNSS module. The reusable code is in `../helpers/gnss.h`; the test code for this step is in `Stap4_GNSS.ino`.

## Install the Library

This step uses the Arduino library `TinyGPSPlus`.

Install it through the Arduino IDE:

1. Go to **Sketch > Include Library > Manage Libraries...**
2. Search for `TinyGPSPlus`
3. Install **TinyGPSPlus** by **Mikal Hart**

If this library is missing, you will get an error such as:

```text
fatal error: TinyGPSPlus.h: No such file or directory
```

## Wiring

The GNSS module communicates over a serial connection. For that, we use the UART on the XIAO.

Use this wiring:

```text
GNSS VCC  -> XIAO 5V
GNSS GND  -> XIAO GND
GNSS TX   -> XIAO D7 / RX
```

For this step, we only read data from the GNSS module. That means the `TX` wire from the GNSS module is enough.

## RX and TX

`TX` means transmit: this pin sends data.

`RX` means receive: this pin receives data.

That is why you do not connect `TX` to `TX`. Data must go from one side to the other:

```text
GNSS TX  ->  XIAO RX
```

The GNSS module talks, and the XIAO listens. The transmit pin on the GNSS module must therefore connect to the receive pin on the XIAO.

If you later want to send commands to the GNSS module too, use the other direction as well:

```text
XIAO TX  ->  GNSS RX
```

For this course, we mostly read location data. That makes `GNSS TX -> XIAO RX` the important connection.

The code contains:

```cpp
#define RXPin D7
#define TXPin D6
```

This means the XIAO listens to the GNSS module on `D7`.

## Code

Open this file in the Arduino IDE:

```text
Stap4_GNSS/Stap4_GNSS.ino
```

The sketch does three things:

```cpp
setupLogging();
setupGNSS();
updateGNSS();
```

Then it retrieves the latest GNSS data:

```cpp
GNSSData gps = getGNSSData();
```

That data comes from `../helpers/gnss.h`.

## What Is a Fix?

A fix means the GNSS module has enough satellite information to calculate a position.

There are two different steps:

```text
1. The XIAO receives serial data from the GNSS module
2. The GNSS module has calculated a valid position
```

You can see the first step in `Chars processed`. If that value increases, the XIAO is receiving data.

You can see the second step in `Fix: yes`. Then latitude and longitude are valid.

## Output

After uploading, open the Serial Monitor at `115200 baud`.

If everything works, you will see something like this:

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

The coordinates are WGS 84:

```text
Latitude  = latitude
Longitude = longitude
```

For GeoJSON, we will later use the same values in this order:

```json
"coordinates": [longitude, latitude]
```

GeoJSON uses longitude first, then latitude.

## Debugging

Use the debug lines to find where the problem is.

### No Data

```text
Fix: no
Status: no GNSS data received; check wiring, power, and RX/TX pins
Chars processed: 0
Sentences with fix: 0
Failed checksums: 0
```

The XIAO is not receiving data from the GNSS module.

Check:

- does the GNSS module have power?
- is `GND` connected to `GND`?
- is `GNSS TX` connected to `XIAO D7 / RX`?
- is any wire loose?
- is the baud rate set to `9600`?

### Data, but No Fix

```text
Fix: no
Status: GNSS data received, waiting for valid location fix
Chars processed: 1200
Sentences with fix: 0
```

The connection works, but the GNSS module does not have a position yet.

Try this:

- place the module closer to a window
- go outside
- point the antenna upward
- wait a few minutes

A first fix can be slow, especially indoors.

### Valid Fix

```text
Fix: yes
Status: valid GNSS fix
```

Then this step is complete. The latitude and longitude can be used for the integration step.

## Checksum Failures

`Failed checksums` means some received NMEA sentences were damaged or incomplete.

A few checksum failures are not a problem. If the value keeps increasing quickly, check the wires, power, and connections.

If `Fix: yes` appears and the coordinates are stable, the GNSS step is good enough to continue.
