/*

This example shows the creation of two instances, sharing the same callback.
Open the Serial Monitor to see the callback function printing the result

*/
#include "Chronograph.h"

Chronograph chrono;
Chronograph chrono2;

void setup() {
  Serial.begin(115200);
  chrono.begin(500, 800, printResult, 1);
  chrono2.begin(100, 200, printResult, 2);
}

void loop() {
  chrono2.setStart();
  delay(7);
  chrono2.setEnd();
  delay(5);
  chrono.timedEvent();
  
}

void printResult(float averageTime, int id) {
  Serial.print("Task no: ");
  Serial.print(id);
  Serial.print (" Average time ");
  Serial.print(averageTime);
  Serial.println(" ms");
}

