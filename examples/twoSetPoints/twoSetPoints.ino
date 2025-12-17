/*

This example shows how to measure the average time between 
two set points within the same program loop.
Open the Serial Monitor to see the callback function printing the result

*/
#include "Chronograph.h"

Chronograph chrono;

void setup() {
  Serial.begin(115200);
  chrono.begin(200, 800, printResult);
}

void loop() {
  delay(5);
  chrono.setStart();
  delay(7);
  chrono.setEnd();
  delay(2);
}

void printResult(float averageTime) {
  Serial.print ("Average time ");
  Serial.print(averageTime);
  Serial.println(" ms");
}

