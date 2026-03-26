// The sensor can measure carbon monoxide gas concentration qualitatively and 
// is suitable for carbon monoxide leak alarm and portable carbon monoxide detector.

// NOTE: The module needs to be warmed up for more than 5 minutes when powered on for the 
// first time. 
// It is recommended to warm up for more than 24 hours if it has not been used for a long time.

const uint8_t measurePin = A0; // analoge uitgang van versterkt microfoon signaal

void setupSensor() {
  pinMode(measurePin, INPUT); 
}

void loopSensor() {
  delay(100);
  
  auto value = analogRead(measurePin);         //Read and save analog value from potentiometer

 // Serial.print("level: ");
 // Serial.println(value);

      const static uint datastreamId = x;

  transmitValue(value, " CO", datastreamId);
}
