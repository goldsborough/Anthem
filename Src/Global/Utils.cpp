//
//  Utils.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Utils.h"
#include <cmath>
#include <sstream>

std::string getDate()
{
    char date[100];
    
    time_t now = time(0);
    
    tm * ltm = localtime(&now);
    
    strftime(date, 100, "%d %m %Y", ltm);
    
    return std::string(date);
}


double noteToFreq(unsigned short n)
{
    // Source: http://en.wikipedia.org/wiki/Piano_key_frequencies
    
    double exp = (n - 49) / 12;
    
    return pow(2, exp) * 440;
}


unsigned short freqToNote(double freq)
{
    return 12 * log2( (freq / 440) ) + 49;
}

double centToFreq(double baseFreq, int centOffset)
{
    return pow(2, centOffset / 1200) * baseFreq;
}

double semiToFreq(double baseFreq, int semiToneOffset)
{
    return pow(2, semiToneOffset / 12) * baseFreq;
}

float getPassedTime(clock_t start)
{
    return (clock() - start) / (double) CLOCKS_PER_SEC;
}