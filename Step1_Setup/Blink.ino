// XIAO has no built-in LED.
#define LED_NOTBUILTIN D2

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_NOTBUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_NOTBUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait one second
  digitalWrite(LED_NOTBUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);              // wait one second
}
