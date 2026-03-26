//  MAX30102 PIN layout  (left to right)
//  ==================================================
//   top view  DESCRIPTION     COLOR
//       +---+
//       |o  |       VCC        RED
//       |o  |       SDA        YELLOW
//       |o  |       SDL        GREEN
//       |o  |       GND        Black
//       +---+
//
//       +---+
//       |o  |       GND        Black
//       |o  |       RD
//       |o  |       IRD
//       |o  |       INT
//       +---+
//
// Observed Properties:
// - HeartRate
// - Sp02
//
// Unit of Measure:
// - bpm
// - %

#include "MAX30102_PulseOximeter.h"  // VEGA_MAX30102

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;
uint32_t tsLastReport = 0;
#define REPORTING_PERIOD_MS 1000

void onBeatDetected() {
  Serial.println("[MAX30102] Beat!");
  //   transmitValue(1, " beat");
}

void setupSensor() {
  // Initialize sensor
  if (!pox.begin()) {
    Serial.println("[MAX30102] Not found, check wiring");
    for (;;)
      ;
  } else {
    Serial.println("[MAX30102] Device connected");
  }
  // The default current for the IR LED is 50mA and is changed below
  pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loopSensor() {
  // Make sure to call update as fast as possible
  pox.update();
  // long irValue = pox.getHeartRate();
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    auto heartRate = pox.getHeartRate();
    auto spO2 = pox.getSpO2();
    /*
    Serial.print("[MAX30102] Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.print("%");
*/
    transmitValue(heartRate, " bpm");
    transmitValue(spO2, "%");

    // if (irValue < 70)
    // Serial.print(",  No finger?");
    //Serial.println();

    tsLastReport = millis();
  }
}
