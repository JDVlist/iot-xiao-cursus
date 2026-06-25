# Step 1: Blink

In the setup document we uploaded an empty sketch. The board was programmed, but nothing visible happened.

Now we make the first visible program: a blinking LED.

## How an Arduino Sketch Runs

Arduino sketches have two important functions:

```c
void setup()
{
}

void loop()
{
}
```

`setup()` runs once when the board starts.

`loop()` runs again and again for as long as the board has power.

Behind the scenes, Arduino does something like this:

```c
main()
{
  setup();

  while (true)
  {
    loop();
  }
}
```

That is why code in `setup()` is used for initialization, and code in `loop()` is used for repeated behavior.

## Connect the LED

The XIAO ESP32C3 does not have a built-in LED that we can use for the standard Blink example. We connect an external LED instead.

Connect the parts like this:

```text
D10 -> 330 ohm resistor -> LED -> GND
```

Pay attention to the direction of the LED:

- the long leg is the positive side
- the short leg goes toward `GND`
- the 10 kohm resistors from the component list are not suitable for this LED

## The Blink Code

Open `Step1_Blink.ino` in this folder, or copy this sketch into the Arduino IDE:

```cpp
// XIAO has no built-in LED.
#define LED_NOTBUILTIN D10

void setup() {
  // initialize digital pin LED_NOTBUILTIN as an output.
  pinMode(LED_NOTBUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_NOTBUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_NOTBUILTIN, LOW);
  delay(1000);
}
```

## What the Code Does

```cpp
#define LED_NOTBUILTIN D10
```

This gives pin `D10` a readable name. We use `LED_NOTBUILTIN` because this board does not have the usual built-in LED for Blink.

```cpp
pinMode(LED_NOTBUILTIN, OUTPUT);
```

This tells the board that pin `D10` will send out a signal.

```cpp
digitalWrite(LED_NOTBUILTIN, HIGH);
```

This puts 3.3 V on the pin. The LED turns on.

```cpp
delay(1000);
```

This waits for 1000 milliseconds, which is 1 second.

```cpp
digitalWrite(LED_NOTBUILTIN, LOW);
```

This turns the pin off. The LED turns off.

Because this code is inside `loop()`, the board keeps repeating it:

1. LED on
2. wait 1 second
3. LED off
4. wait 1 second

## Upload the Sketch

Check that the Arduino IDE still has the correct board and port selected. Then click **Upload**.

The Arduino IDE compiles the code and sends it to the XIAO ESP32C3. If everything is connected correctly, the LED starts blinking.

## Try It

Change the delay values:

```cpp
delay(250);
```

Upload again. The LED should blink faster.

Try different values and see what changes.

## Next

Continue with [Step 2](../Step2_Log/README.md), where the board sends text back to the computer.
