// https://cdn-shop.adafruit.com/datasheets/MAX4465-MAX4469.pdf

// A0 is the analog output pin that delivers the amplified sound signal.
// Please note that the output will have a DC bias equal to VCC/2, so when it’s perfectly quiet,
// the voltage will remain steady at a VCC/2 level.

// If you adjust the MAX4466 so the ADC reads 4095 when the audio volume is 130db,
// the ADC reading of 1 will represent an audio volume of about 60db.


const uint8_t soundPin = A0;  // analoge uitgang van versterkt microfoon signaal

const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)

void setupSensor() {
  pinMode(soundPin, INPUT);
}

void loopSensor() {
  auto startMillis = millis();  // Start of sample window
  auto peakToPeak = 0;          // peak-to-peak level

  auto signalMax = 0;
  auto signalMin = 1024;

  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow) {
    auto sample = analogRead(soundPin);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
                                       // Serial.println(peakToPeak);

  const static uint datastreamId = x;

  transmitValue(peakToPeak, "Clap!", datastreamId);
}
