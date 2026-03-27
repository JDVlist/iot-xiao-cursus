//  MAX-4466 PIN layout
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       VCC         RED
//       |o  |       GND         BLACK
//       |o  |       OUT         White
//       +---+
//
// Observed Properties:
// - Volt (or Peak sound level)
//
// Unit of Measure:
// - Volt

// https://cdn-shop.adafruit.com/datasheets/MAX4465-MAX4469.pdf

// A0 is the analog output pin that delivers the amplified sound signal.
// Please note that the output will have a DC bias equal to VCC/2, so when it’s perfectly quiet,
// the voltage will remain steady at a VCC/2 level.

// If you adjust the MAX4466 so the ADC reads 4095 when the audio volume is 130db,
// the ADC reading of 1 will represent an audio volume of about 60db.


const uint8_t soundPin = A0;  // analoge uitgang van versterkt microfoon signaal

const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
const int adcResolution = 4095;

int audioInRaw;
int audioInRectified;
#define samples (256)

void setupSensor() {
  pinMode(soundPin, INPUT);
}


float audioIn() {
  int audioAverage = 0, audioMax = 0, audioMin = adcResolution;

  for (int i = 0; i < samples; i++) {
    int audioInRaw = analogRead(A0);

    audioInRectified = abs(audioInRaw - 337);  // level shift for 3,3V
    audioMin = min(audioMin, audioInRaw);
    audioMax = max(audioMax, audioInRaw);
    audioAverage += audioInRaw;
  }

  audioAverage /= samples;
  double volts = ((audioMax - audioMin) * 3.3f) / adcResolution;  // convert to volts

  /*
  Serial.print("audioInRectified:"); Serial.print(audioInRectified); Serial.print(" ");
  Serial.print("audioMin:"); Serial.print(audioMin); Serial.print(" ");
  Serial.print("audioMax:"); Serial.print(audioMax); Serial.print(" ");
  Serial.print("audioAverage:"); Serial.print(audioAverage); Serial.print(" ");
  Serial.print("volts:"); Serial.println(volts);
*/

   return volts;
}

void loopSensor() {
  // Serial.println(peakToPeak);
  transmitValue(audioIn(), "V");
}
