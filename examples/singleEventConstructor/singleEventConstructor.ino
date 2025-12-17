/*

This example shows the use of a cnstructor with parameters
for a timedEvent() case.
Open the Serial Monitor to see the callback function printing the result

*/
#include "Chronograph.h"

// Declare the callback function or define it before the constructor
void printResult(float averageTime);

// Use the constructor to pass parameters
Chronograph chrono(500, 800, printResult);

void setup() {
  Serial.begin(115200);
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

