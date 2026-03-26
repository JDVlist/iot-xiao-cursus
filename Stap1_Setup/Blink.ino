// XIAO heeft geen ingebouwde LED
#define LED_NOTBUILTIN D2

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_NOTBUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_NOTBUILTIN, HIGH);  // zet LED aan (HIGH is the voltage level)
  delay(1000);              // wacht een seconde
  digitalWrite(LED_NOTBUILTIN, LOW);   // zet LED uit by making the voltage LOW
  delay(1000);              // wacht een seconde
}
