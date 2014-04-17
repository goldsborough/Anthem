//
//  Noise.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Noise.h"
#include "Global.h"

double Noise::tick()
{
    double sample = 0.0;
    
    int indexBase = (int) _ind;         // The truncated integer part
    double indexFract = _ind - indexBase;    // The remaining fractional part
    
    // grab the two items in-between which the actual value lies
    double value1 = _WT[indexBase];
    double value2 = _WT[indexBase+1];
    
    // interpolate
    double value = value1 + ((value2 - value1) * indexFract);
    
    sample = value * _amp;
    
    double randVal = rand() % (int)(20000 * _Color);
    
    _ind += global.tableIncr * randVal;
    
    if (_ind  >= global.wtLen)
        _ind -= global.wtLen;
    
    return sample;
}