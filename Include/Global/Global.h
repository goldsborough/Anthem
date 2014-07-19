//
//  Global.h
//  Synth
//
//  Created by Peter Goldsborough on 22/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Global__
#define __Synth__Global__

#include <ctime>
#include <cstdlib>

// http://goo.gl/748HMW

namespace Global
{
    const double pi = 3.14159265358979;
    const double twoPi = 6.28318530717958;
    
    extern unsigned int samplerate;
    
    extern unsigned int nyquistLimit;
    
    extern unsigned int wtLen;
    
    extern double tableIncr;
    
    extern void init(const unsigned int smplr, const unsigned int wavetableLen);
};

#endif /* defined(__Synth__Globals__) */
