// A=0,B=0 --> Functioneert als Reset knop met hoge output (standaard)
// A=0,B=1 --> Functioneert als Aan/uit knop met hoge output
// A=1,B=0 --> Functioneert als Reset knop met lage output
// A=1,B=1 --> Functioneert als Aan/uit knop met lage output
//
// Let op: In de reset stand sensor kan maar maximaal ~7s een aanraking registeren,
// daarna zal de output weer laag (A=0,B=0) worden, of hoog worden (A=1,B=0).

#include <TouchSensorLib.h>

TouchSensor touch(D2);  // Connect TTP223 OUT to D4

void setupSensor() {
  touch.setDebounceTime(20);
  Serial.println("[TTP223B] Initialized");
}

void loopSensor() {
  delay(10);

  touch.update();

  if (touch.isTouched()) {
    Serial.println("[TTP223B] Touching...");

    const static uint datastreamId = x;

    transmitValue(1, "", datastreamId);
  }

  if (touch.wasTapped()) {
    Serial.println("[TTP223B] Tapped!");
  }

  if (touch.wasLongPress()) {
    Serial.println("[TTP223B] Long Press Detected!");
  }

  // Serial.print("level: ");
  // Serial.println(value);
}
