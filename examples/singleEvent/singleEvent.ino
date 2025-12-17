/*

This example shows how to measure the average time between 
subsequent executions of the same event in the program loops
Open the Serial Monitor to see the callback function printing the result

*/
#include "Chronograph.h"

Chronograph chrono;

void setup() {
  Serial.begin(115200);
  chrono.begin(500, 800, printResult);
}

void loop() {
  delay(7);
  chrono.timedEvent();
}

void printResult(float averageTime) {
  Serial.print ("Average time ");
  Serial.print(averageTime);
  Serial.println(" ms");
}

