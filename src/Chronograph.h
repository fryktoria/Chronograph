
#ifndef _CHRONOGRAPH_H_
#define _CHRONOGRAPH_H_

class Chronograph 
{
private:
  void (*callback)(float averageTime);  // Pointer to a function that returns void and takes arguments the result, i.e. the average time
  float _movingAverageTime;
  unsigned int _numMeasurementsToAverage;
  unsigned int _numEventsToResult;
  unsigned long _lastEventTime;
  unsigned int _measurementCounter;
  unsigned int _eventCounter;
  bool _validResult;
  void calculate(void);


public:
  // Constructor without parameters. Need to call begin() later to set parameters
  Chronograph(void);
  /*
  // Constructor with parameters.
  // Arduino programs should make sure that the userCallbackFunction has been defined before the call to the constructor
  // or it has been declared via a function prototype


  minMeasurementsToAverage: The moving average is calculated using minMeasurementsToAverage measurements. Also,
                            at least minMeasurementsToAverage are required in order to produce a valid result.
                            After minMeasurementsToAverage measurements are received, every single new measurement
                            will contribute to the calculation of a new moving average time
  numEventsToResult:        The user registered callback function will be called every numEventsToResult. 
                            Please note that the callback function will be called for the first time
                            only after minMeasurementsToAverage measurements have been collected.
  userCallbackFunction(result) : The callback function of the user space that will be activated every numEventsToResult                                                    
  */
  Chronograph(unsigned int minMeasurementsToAverage, unsigned int numEventsToResult, void (*userCallbackFunction)(float result));

  // Public methods and variables
  // begin() can be called in Arduino setup(), in case the constructor is called without parameters
  void begin(unsigned int minMeasurementsToAverage, unsigned int numEventsToResult, void (*userCallbackFunction)(float result));

  // For timing the frequency of one single event in subsequent main program loops
  void timedEvent(void);

  // For timing the duration of a process within the same main program loop
  void setStart(void);
  void setEnd(void);

  /*
   The currently computed average time. 
   Although available as a public variable and method movingAverageTime(), its value will make sense 
   only after the first minMeasurementsToAverage is collected.
   After that, the value can be used normally. 
   One can interrogate the boolean function validResult() to check
   if the currentAverageTime has been calculated over at least the 
   minMeasurementsToAverage measurements. 
   Yet, it is suggested
   that the calling program gets the value of currentAverageTime via the callback function.
  */ 
  float currentAverageTime;
  float movingAverageTime(void);  
  bool validResult(void);

};

#endif
