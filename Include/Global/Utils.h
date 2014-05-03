//
//  Utils.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Utils__
#define __Synth__Utils__

#include <string>

std::string getDate();

double noteToFreq(unsigned short n);

unsigned short freqToNote(double freq);

double centToFreq(double baseFreq, int centOffset);

double semiToFreq(double baseFreq, int semiToneOffset);

float getPassedTime(unsigned long start);


#endif /* defined(__Synth__Utils__) */
