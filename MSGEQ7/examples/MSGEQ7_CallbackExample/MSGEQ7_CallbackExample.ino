//Rickey Ward (c) 1/10/2017
//MSGEQ7 callback example

//Include Lib
#include <MSGEQ7.h>

//Create Instance
MSGEQ7 eq;

void setup() {
  Serial.begin(115200);

  //initialize pins;
  //Right, Left, Reset, Strobe
  eq.init(A0, A1, 6, 7);
}

void MyCallback(uint8_t left,uint8_t right, uint8_t band)
{
    char c[3];
    
    Serial.print("  L");
    Serial.print(band);
    Serial.print(":");
    sprintf(c, "%03d", left);
    Serial.print(c);
    Serial.print(" R");
    Serial.print(band);
    Serial.print(":");
    sprintf(c, "%03d", right);
    Serial.print(c);
    
    if(band == 6)
      Serial.println();
}

void loop() {

  //Call this once per "frame"
  //MyCallback will be called 7 times, once for each band
  eq.update(MyCallback);
  
  delay(50);
}

