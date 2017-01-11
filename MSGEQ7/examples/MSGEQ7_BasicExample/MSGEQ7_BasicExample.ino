//Rickey Ward (c) 1/10/2017
//MSGEQ7 basic example

//Include Lib
#include <MSGEQ7.h>

//Create Instance
MSGEQ7 eq;

void setup() {

  //initialize pins;
  //Right, Left, Reset, Strobe
  eq.init(A0, A1, 6, 7);

  Serial.begin(115200);
}

void loop() {

  //Call this once per "frame" and eq.rawData_L/R[0-6] and eq.data_L/R[0-6] will be refreshed.
  eq.update();

  //Print the results to the serial terminal.
  char c[3];
  for(int i = 0; i < NUM_FREQUENCY_BANDS; i++)
  {
    Serial.print(" R");
    Serial.print(i);
    Serial.print(":");
    sprintf(c, "%03d", eq.Data_R[i]);
    Serial.print(c);
  }
  for(int i = 0; i < NUM_FREQUENCY_BANDS; i++)
  {
    Serial.print(" L");
    Serial.print(i);
    Serial.print(":");
    sprintf(c, "%03d", eq.Data_L[i]);
    Serial.print(c);
  }
  Serial.println();

  delay(50);

}
