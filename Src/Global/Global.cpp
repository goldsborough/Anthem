//
//  Global.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Global.h"

/*******************************************************************************
 * Interestingly enough this is the only way I got it to work.
 * I want to use a namespace instead of an object for the global
 * variables as it is a lot nicer and more C++y, however the variables
 * in the namespace must either be const or extern, in this case extern
 * because samplereate,nyquistLimit etc. are user-defined. Because they
 * are extern, they need to be defined 'externally', in a .cpp file (this
 * one), however, this doesn't work if they are defined in a function (init)
 * which is why I define them all to 0 here and then when the init function
 * is called they get their actual values.
 *******************************************************************************/

namespace Global
{
    unsigned int samplerate = 0,
                 nyquistLimit = 0,
                 wtLen = 0;
    
    double tableIncr = 0;
    
    void init(const unsigned int smplr, const unsigned int wavetableLen)
    {
        samplerate = smplr;
        nyquistLimit = smplr/2;
        
        wtLen = wavetableLen;
        
        tableIncr = ((double) wtLen) / smplr;
        
        srand((unsigned)time(0));
    }
}