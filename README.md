# Chronograph
An Arduino library to measure a moving average of the time required to execute pieces of code in multiple iterations.


## About Chronograph
It is common during software development to measure the time that is required to run parts of the code. Extremely useful in code debugging, timing can provide insightful information about what the code does behind the scenes. In "normal" software which performs some sort of data processing, the program usually terminates and allows us to examine the duration between check points that we have set. Yet, in embedded systems, there is an endless loop that runs over and over, several hundreds or thousand times per second. If we try to measure a time duration in every iteration of the code, we will end up with thousands of results. Even worse, if we try to print those results or store them in some form of memory, we will interfere destructively in the operation of the system, because such printing operations take a large amount of time, perhaps even longer than the code we are debugging. 

The Chronograph library adds a tool in our debugging arsenal, by providing us a way to measure the **average time** that it takes to run multiple iterations of our code. When we initiate the library, we decide how many measurements will be used to calculate this average. This is the `minMeasurementsToAverage` parameter. Our average time is actually a Moving Average, where each new measurement will be used to calculate a new average time. For that purpose and in order to keep the code simple and to avoid wasting memory space with buffers or circular queues, we use the formula `NewAverage = (newMeasurement + numberOfMeasurements * PreviousAverage)/(numberOfMeasurements + 1)`. It is not intuitive, but it can be proven mathematically that it produces the same result as if we have kept each individual measurement and calculated the average by summing and dividing. Please check https://en.wikipedia.org/wiki/Moving_average for the mathematical proof.

We just need to understand that this formula calculates a **Cumulative Average** from the moment that we get the first measurement and until the number of our measurements reach the `minMeasurementsToAverage` parameter. Afterwards, each new measurement will contribute to the calculation of the moving average, using an average of a fixed number of `minMeasurementsToAverage` measurements. During the first initial stage, the calculation will produce an unusable value. Consider that if our process requires exactly 10ms, the first calculation will produce 5ms, since we average between a starting 0 and a value of 10. The second calculation will produce 6.67, the third 7.5, increasing after iteration and moving gradually towards the value of 10, without actually reaching it. This functionality is somewhat similar to the **Dichotomy paradox** of the philosopher Zeno (see  https://en.wikipedia.org/wiki/Zeno%27s_paradoxes). Doing the math, the process will go to 9.8 after 50 iterations. It is not exact but for a large amount of iterations, we will have a value that will allow us to make deductions and pinpoint problems of our code. The library will never trigger the callback function before the number of measurements reach `minMeasurementsToAverage`.

As stated above, printing multiple times may ruin time-critical operations of our code. Therefore, we need to get the result as rarely as possible. The parameter `numEventsToResult` controls how often we get a result, in the sense that the callback function is called every `numEventsToResult` iterations. Since we consider that the first `minMeasurementsToAverage` will not produce an acceptable moving average value, it is self evident that `numEventsToResult` must be at least equal or higher than `minMeasurementsToAverage`. 








## Reference

### Constructor
This library has two constructors, one can be called without parameters and the other with parameters. In case the constructor without parameters is used, the program should call the `begin()` function in order to supply the proper parameters.
In case the constructor with parameters is used, the begin() function should not be called. 
Care should be exercised when using the constructor with parameters, regarding the user specified callback function. Compiler will complain if the callback function has not been declared or defined before the constructor. Arduino programs should make sure that the userCallbackFunction has been defined before the call to the constructor or has been declared via a function prototype.

``` c++
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
```

### Parameters
The parameters can be set via the constructor or via the `begin()` method.
``` c++
  minMeasurementsToAverage: The moving average is calculated using minMeasurementsToAverage measurements. Also,
                            at least minMeasurementsToAverage are required in order to produce a valid result.
                            After minMeasurementsToAverage measurements are received, every single new measurement
                            will contribute to the calculation of a new moving average time
  numEventsToResult:        The user registered callback function will be called every numEventsToResult. 
                            Please note that the callback function will be called for the first time
                            only after minMeasurementsToAverage measurements have been collected.
  userCallbackFunction(result) : The callback function of the user space that will be activated every numEventsToResult                                                    
```

### Usage

#### Single setPoint
In case we want to measure how often a specific set point is repeated in subsequent loops, we use the `timedEvent` method. In the simple example below, there is a `delay(7)` statement that has a duration 7 milliseconds. We have set the timedEvent() method to measure the average time between subsequent executions of the `loop()`. Of course, instead of `delay()`, we could have entire pieces of code. 

``` c++
void loop() {
  delay(7);
  chrono.timedEvent();
}
```

If we have set the callback routine to print the average time on the Serial monitor, we should see something like:
``` c++
Average time 7.07 ms
Average time 7.07 ms
Average time 7.07 ms
Average time 7.07 ms
Average time 7.07 ms

```


Also, we could measure the average time between subsequent executions of any `while()` loop. In that case, we must make sure that the initial parameters of our constructor will be such, that the code will provide enough measurements to produce a valid result, i.e. if the while() will be executed at least 100 times, `minMeasurementsToAverage` and  `numEventsToResult` should be 100 or lower. 

``` c++
void loop() {
  ....
  while(i < numLoops) {
      .....
      chrono.timedEvent();
  }
  ......  
}
```

#### Two setPoints
Suppose that we want to measure the time it takes to run a function. In that case, we set `startTime()` before the function statement and `endTime()` after the function. The library will calculate the average time that the function requires.

``` c++
void loop() {
  ....
  chrono.startTime()
  timeConsumingFunction();
  chrono.endTime();
  ......  
}
```

### How to get the result
The library provides two methods to get the result.
The suggested method is to use a callback function. In the example below, the callback function `printResult` is registered in the `begin()` method.

``` c++
void setup() {
  Serial.begin(115200);
  chrono.begin(500, 800, printResult);
}


void printResult(float averageTime) {
  Serial.print ("Average time ");
  Serial.print(averageTime);
  Serial.println(" ms");
}
```

The second method uses the `movingAverageTime()` method. In order to make sure that the moving average time has been calculated using at least the minimum number of measurements that are required in order to have a meaningful result, we need to use the `validResult()` method.

``` c++
....
if (chrono.validResult()) {
  Serial.println(chrono.movingAverageTime());
}
....
```

### Working without a callback function
It is possible to use the library without registering a callback function. In such a case, use a constructor without parameters and utilize the form of `begin()` without a callback, such as `begin(num1, num2)`.

``` c++

Chronograph chrono;

void setup() {
  chrono.begin(100, 500);
}

void loop() {
  ....
  if (chrono.validResult()) {
    Serial.println(chrono.movingAverageTime());
  }
  ....
}  
```

Please note that the library does not provide a constructor without a callback function. You have to use `begin()`.
 

## Supported microcontrollers
This library does not depend on hardware specific code and does not have any peculiarities that would inhibit using on other platfors. Yet, it has been tested only on Arduino platform.

## How to install
This library can be installed in two ways, ether by using the manual mode or by using the Arduino library editor.

### Manual installation
Click on the "Clone or download" button in the upper right corner, and then "Download ZIP". Exctract the ZIP file, and move the extracted folder to the location "~/Documents/Arduino/libraries". Create the "libraries" folder if it doesn't exist. Open Arduino IDE, and a new library called "Chronograph" will show up under the "examples" menu.

### Library manager installation
The library is also available through Arduino's library manager. <br/> 
To open the library manager, in the IDE click on `Sketch` -> `Include Library` -> `Manage Libraries` <br/>
Then search for <b>Chronograph</b>.