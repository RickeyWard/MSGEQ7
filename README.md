# MSGEQ7-ArduinoLibrary
>Arduino library for ease of use of the MSGEQ7 Chip / Module

##Include the library
  ```
  #include <MSGEQ7.h>
  ```

##Create an instance
  ```
  MSGEQ7 eq;
  ```

##Initialize in setup
 ```
  //analog right, analog left, strobe, reset
  eq.init(A0, A1, 5, 6);
  ```
###For using a single chip
> instead of a stereo module just pass in 1 analog value
  ```
  //analog, strobe, reset
  eq.init(A0, 5, 6);
  ```
 > both left and right data sets will be the same, use either.

##In the loop call update
  ```
  eq.update();
  ```

##Use the data
  ```
  int value_Left_0 = eq.rawDataL[0]; //0-6
  int value_Right_0 = eq.rawDataR[0]; //0-6
```

>look at code for advanced features such as callbacks and single value reads
