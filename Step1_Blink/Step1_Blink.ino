// XIAO has no built-in LED.
#define LED_NOTBUILTIN D10

void setup() {
  // initialize digital pin LED_NOTBUILTIN as an output.
  pinMode(LED_NOTBUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_NOTBUILTIN, HIGH);  // turn the LED on
  delay(1000);                         // wait one second
  digitalWrite(LED_NOTBUILTIN, LOW);   // turn the LED off
  delay(1000);                         // wait one second
}
