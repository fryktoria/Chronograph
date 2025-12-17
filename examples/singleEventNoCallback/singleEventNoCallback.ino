/*

This example shows the option to not use a callback function.
Open the Serial Monitor to see the callback function printing the result

*/
#include "Chronograph.h"

Chronograph chrono;

void setup() {
  Serial.begin(115200);
  chrono.begin(500, 800);
}

void loop() {
  delay(7);
  chrono.timedEvent();
  delay(2);
  if (chrono.validResult()) {
    Serial.println(chrono.movingAverageTime());
  }
}


