
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

#ifndef MSGEQ7_h
#define MSGEQ7_h


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define NUM_FREQUENCY_BANDS (7)

struct bandData
{
    uint8_t band;
    uint8_t dataL;
    uint8_t dataR;
    uint16_t rawDataL;
    uint16_t rawDataR;
    bool valid = false;
};

class MSGEQ7
{
    public:
        void init(uint8_t analog_R_pin, uint8_t analog_L_pin, uint8_t strobePin, uint8_t resetPin);
        void init(uint8_t analog_L_pin, uint8_t strobePin, uint8_t resetPin);
        void update(void);
        void update(void (*callback)(uint8_t lval, uint8_t rval, uint8_t col));
        void reset(void);
        bandData readOneBand(void);
        bandData readOneBand(void (*callback)(uint8_t lval, uint8_t rval, uint8_t col));

        uint16_t RawData_L[NUM_FREQUENCY_BANDS];
        uint16_t RawData_R[NUM_FREQUENCY_BANDS];
        uint8_t Data_L[NUM_FREQUENCY_BANDS];
        uint8_t Data_R[NUM_FREQUENCY_BANDS];
    private:

        bool initialized = false;
        bool stereo = false;

        uint8_t MSGEQ7_ANALOG_PIN_R = A1;
        uint8_t MSGEQ7_ANALOG_PIN_L = A0;
        uint8_t MSGEQ7_STROBE_PIN = 6;
        uint8_t MSGEQ7_RESET_PIN = 7;

        uint8_t ReadOneBandCounter = 0;
};

#endif