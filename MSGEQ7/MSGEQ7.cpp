
//Rickey Ward (c) Jan 10 2017
//MSGEQ7 Lib

//Based on example code provided on github with following license

/*
	Copyright (c) 2015, Peter Easton, Whizoo
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	* Neither Whizoo nor Peter Easton may be used to endorse or promote products
	derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL PETER EASTON BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

 */

#include "MSGEQ7.h"

void MSGEQ7::init(uint8_t analog_R_pin, uint8_t analog_L_pin, uint8_t strobePin, uint8_t resetPin)
{
    this->MSGEQ7_ANALOG_PIN_R = analog_R_pin;
    this->MSGEQ7_ANALOG_PIN_L = analog_L_pin;
    this->MSGEQ7_RESET_PIN = resetPin;
    this->MSGEQ7_STROBE_PIN = strobePin;

    pinMode(this->MSGEQ7_ANALOG_PIN_R, INPUT);
    pinMode(this->MSGEQ7_ANALOG_PIN_L, INPUT);
    pinMode(this->MSGEQ7_STROBE_PIN, OUTPUT);
    pinMode(this->MSGEQ7_RESET_PIN, OUTPUT);
    digitalWrite(this->MSGEQ7_RESET_PIN, LOW);
    digitalWrite(this->MSGEQ7_STROBE_PIN, HIGH);

    for(int i = 0; i < NUM_FREQUENCY_BANDS; i ++)
    {
        this->RawData_L[i] = 0;
        this->RawData_R[i] = 0;
        this->Data_L[i] = 0;
        this->Data_R[i] = 0;
    }

    this->initialized = true;
}

void MSGEQ7::reset(void)
{
    digitalWrite(this->MSGEQ7_RESET_PIN, HIGH);
    digitalWrite(this->MSGEQ7_RESET_PIN, LOW);
}

bandData MSGEQ7::readOneBand(void)
{
    return this->readOneBand(NULL);
}

bandData MSGEQ7::readOneBand(void (*callback)(uint8_t lval, uint8_t rval, uint8_t col))
{
    bandData newData;

    if(this->ReadOneBandCounter > 7 || !initialized)
    {
        newData.valid = false;
        return newData;
    }

    newData.valid = true;


    digitalWrite(this->MSGEQ7_STROBE_PIN, LOW);
    delayMicroseconds(30); // Allow the output to settle
    newData.rawDataR = analogRead(this->MSGEQ7_ANALOG_PIN_R);
    newData.rawDataL = analogRead(this->MSGEQ7_ANALOG_PIN_L);

    this->RawData_R[this->ReadOneBandCounter] = newData.rawDataR;
    this->RawData_L[this->ReadOneBandCounter] = newData.rawDataL;
    
    digitalWrite(this->MSGEQ7_STROBE_PIN, HIGH);

    newData.dataR = newData.rawDataR >> 2;
    newData.dataL = newData.rawDataL >> 2;
    
    // attenuate by 7 every read.
    this->Data_R[this->ReadOneBandCounter] = this->Data_R[this->ReadOneBandCounter] > 7 ? this->Data_R[this->ReadOneBandCounter] - 7 : 0;
    this->Data_L[this->ReadOneBandCounter] = this->Data_L[this->ReadOneBandCounter] > 7 ? this->Data_L[this->ReadOneBandCounter] - 7 : 0;
    
    // Don't show the lower values
    if (newData.dataR > 40) { //30 means a 11.7% noise floor
        // If the new volume is greater than that currently being showed then show the higher volume
        if (newData.dataR > this->Data_R[this->ReadOneBandCounter])
            this->Data_R[this->ReadOneBandCounter] = newData.dataR;
    }

    if (newData.dataL > 40) { //30 means a 11.7% noise floor
        // If the new volume is greater than that currently being showed then show the higher volume
        if (newData.dataL > Data_L[this->ReadOneBandCounter])
            this->Data_L[this->ReadOneBandCounter] = newData.dataL;
    }

    if(callback != NULL)
        callback(this->Data_L[this->ReadOneBandCounter], this->Data_R[this->ReadOneBandCounter], this->ReadOneBandCounter);

    this->ReadOneBandCounter++;
    
    return newData;
}

void MSGEQ7::update(void)
{
    this->update(NULL);
}

//queries the MSGEQ7 for all 7 bands on both left and right channels
//if a callback pointer is passed, it will be called once for each of the 7 bands;
void MSGEQ7::update(void (*callback)(uint8_t lval, uint8_t rval, uint8_t col))
{
    if(!initialized)
        return;

      int frequencyBandVolume_R, frequencyBandVolume_L;
      // Toggle the RESET pin of the MSGEQ7 to start reading from the lowest frequency band
      digitalWrite(this->MSGEQ7_RESET_PIN, HIGH);
      digitalWrite(this->MSGEQ7_RESET_PIN, LOW);

      for (int i=0; i<NUM_FREQUENCY_BANDS; i++)
      {
        digitalWrite(this->MSGEQ7_STROBE_PIN, LOW);
        delayMicroseconds(30); // Allow the output to settle
        frequencyBandVolume_R = analogRead(this->MSGEQ7_ANALOG_PIN_R);
        frequencyBandVolume_L = analogRead(this->MSGEQ7_ANALOG_PIN_L);

        this->RawData_R[i] = frequencyBandVolume_R;
        this->RawData_L[i] = frequencyBandVolume_L;
        
        digitalWrite(this->MSGEQ7_STROBE_PIN, HIGH);
        digitalWrite(this->MSGEQ7_STROBE_PIN, HIGH);
        
        // The read value is 10-bit (0 to 1024).  PWM needs a value from 0 to 255, so divide by 4
        frequencyBandVolume_R = frequencyBandVolume_R >> 2;
        frequencyBandVolume_L = frequencyBandVolume_L >> 2;
        
        // attenuate by 7 every read.
        this->Data_R[i] = this->Data_R[i] > 7 ? this->Data_R[i] - 7 : 0;
        this->Data_L[i] = this->Data_L[i] > 7 ? this->Data_L[i] - 7 : 0;
        
        // Don't show the lower values
        if (frequencyBandVolume_R > 40) { //30 means a 11.7% noise floor
          // If the new volume is greater than that currently being showed then show the higher volume
          if (frequencyBandVolume_R > this->Data_R[i])
            this->Data_R[i] = frequencyBandVolume_R;
        }
    
        if (frequencyBandVolume_L > 40) { //30 means a 11.7% noise floor
          // If the new volume is greater than that currently being showed then show the higher volume
          if (frequencyBandVolume_L > Data_L[i])
            this->Data_L[i] = frequencyBandVolume_L;
        }
        
        if(callback != NULL)
            callback(this->Data_L[i], this->Data_R[i], i);
    
      }
}