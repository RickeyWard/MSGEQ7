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

##Initalize in setup
 ```
  //analog right, analog left, strobe, reset
  eq.init(A0, A1, 5, 6);
  ```

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
