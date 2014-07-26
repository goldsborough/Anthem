//
//  Util.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Util.h"
#include <cmath>
#include <sstream>

namespace Util
{
    std::string getDate()
    {
        char date[100];
        
        time_t now = time(0);
        
        tm * ltm = localtime(&now);
        
        strftime(date, 100, "%d %m %Y", ltm);
        
        return std::string(date);
    }


    double noteToFreq(const unsigned short& n)
    {
        // Source: http://en.wikipedia.org/wiki/Piano_key_frequencies
        
        double exp = (n - 49) / 12;
        
        return pow(2, exp) * 440;
    }


    unsigned short freqToNote(const double& freq)
    {
        return 12 * log2( (freq / 440) ) + 49;
    }

    double centToFreq(const double& baseFreq, const int& centOffset)
    {
        return pow(2, centOffset / 1200) * baseFreq;
    }

    double semiToFreq(const double& baseFreq, const int& semiToneOffset)
    {
        return pow(2, semiToneOffset / 12.0) * baseFreq;
    }

    float getPassedTime(const clock_t& start)
    {
        return (clock() - start) / (double) CLOCKS_PER_SEC;
    }
    
    double dbToAmp(const double& baseAmp, const double& dB)
    {
        return baseAmp * pow(10, (dB/20));
    }
}