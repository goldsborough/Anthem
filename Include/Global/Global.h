//
//  Global.h
//  Synth
//
//  Created by Peter Goldsborough on 22/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Global__
#define __Synth__Global__

#define PI 3.14159265358979
#define twoPI 6.28318530717958

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <vector>

class Global
{
public:
    
    void Init(const unsigned int smplr, const unsigned int wavetableLen)
    {
        samplerate = smplr;
        nyquistLimit = smplr/2;
        
        wtLen = wavetableLen;
        
        tableIncr = ((double) wtLen) / smplr;
        
        srand((unsigned)time(0));
    }
    
    unsigned int samplerate;
    
    unsigned int nyquistLimit;
    
    unsigned int wtLen;
    
    double tableIncr;
    
};

extern Global global;

#endif /* defined(__Synth__Globals__) */
