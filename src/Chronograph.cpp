#include "Chronograph.h"
#include <Arduino.h>

Chronograph::Chronograph(void) {}

Chronograph::Chronograph(unsigned int numMeasurementsToAverage, unsigned int numEventsToResult, void (*userCallbackFunction)(float)) 
{
  begin(numMeasurementsToAverage, numEventsToResult, userCallbackFunction);
}


void Chronograph::begin(unsigned int numMeasurementsToAverage, unsigned int numEventsToResult, void (*userCallbackFunction)(float)) 
{
  beginNoCallback(numMeasurementsToAverage, numEventsToResult);
  callback = userCallbackFunction;
}


void Chronograph::begin(unsigned int numMeasurementsToAverage, unsigned int numEventsToResult) 
{
  beginNoCallback(numMeasurementsToAverage, numEventsToResult);
}


void Chronograph::beginNoCallback(unsigned int numMeasurementsToAverage, unsigned int numEventsToResult) 
{
  _numMeasurementsToAverage = numMeasurementsToAverage;
  _numEventsToResult = numEventsToResult;
  // We will not calculate a result before we collect at least the specified minimum number of measurement
  if (numMeasurementsToAverage > numEventsToResult) {
    _numMeasurementsToAverage = numEventsToResult;
  }

  _eventCounter = 0;
  _measurementCounter = 0;
  _validResult = false;
  currentAverageTime = 0;
}


void Chronograph::timedEvent(void)
{
  calculate();
}


void Chronograph::setStart(void) 
{
  _lastEventTime = millis();
}


void Chronograph::setEnd(void) 
{
  calculate();
}


void Chronograph::calculate(void) 
{
  if (!_validResult) {
    _measurementCounter++;
    if (_measurementCounter > _numMeasurementsToAverage) {
    /*
      Calculate the cumulative average with as many measurements as 
      collected until now, or, in case we have already collected _numMeasurementsToAverage,
      hold the _measurementCounter to  _numMeasurementsToAverage.
    */
      _measurementCounter = _numMeasurementsToAverage;
      // We will not calculate a result before we collect at least the specified minimum number of measurement
      _validResult = true;
    }
  }

  unsigned long currentTime = millis();
  _movingAverageTime = ((currentTime - _lastEventTime) + _measurementCounter * _movingAverageTime) / (_measurementCounter + 1);
  if (_validResult) {
    currentAverageTime = _movingAverageTime; 
  }
  _lastEventTime = currentTime;

  _eventCounter++;
  if (_eventCounter > _numEventsToResult) {
    _eventCounter = 0;

    // and run the user registered function 
    if (callback) {
      callback(currentAverageTime); // Call user’s registered function
    }  
  }
}


bool Chronograph::validResult(void) 
{
  return _validResult; 
}


float Chronograph::movingAverageTime(void)
{
  return currentAverageTime;
}