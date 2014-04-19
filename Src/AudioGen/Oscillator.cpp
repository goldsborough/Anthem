//
//  Oscillator.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Oscillator.h"
#include "Global.h"
#include "Wavetable.h"

Oscillator::Oscillator(const int mode, double frq,
                       double amp, short phaseOffset)
{
    _amp = amp;
    
    _mode = mode;
    
    _ind = 0;
    _indIncr = 0;
    
    _phaseOffset = 0;
    
    setPhaseOffset(phaseOffset);
    
    _WT = wavetable.getWaveform(_mode);
    
    setFreq(frq);
};

void Oscillator::setFreq(double Hz)
{
    _freq = Hz;
    _indIncr = global.tableIncr * Hz;
}

void Oscillator::setPhaseOffset(short degrees)
{
    // convert degrees higher than 360 or
    // less than 0 to it's 0 - 360 degree
    // equivalent
    
    if (degrees < 0)
        degrees = -degrees;
    
    while (degrees > 360)
        degrees -= 360;
    
    // Return to original index (without offset), so
    // that setting a new offset doesn't add to the
    // old one but really set a new one
    _ind -= _phaseOffset;
    
    // The wavetable holds 360 degrees, so divide the degrees
    // by 360 to get e.g. 1/4 and multiply by the wavetablelength
    // to get the number of samples to shift by
    
    _phaseOffset = ((global.wtLen + 1) * degrees) / 360.0;
    
    // Add new offset
    _ind += _phaseOffset;
}

double Oscillator::tick()
{
    if (_mode == Wavetable::NONE)
        return 0;
    
    int indexBase = (int) _ind;         // The truncated integer part
    double indexFract = _ind - indexBase;    // The remaining fractional part
    
    // grab the two items in-between which the actual value lies
    double value1 = _WT[indexBase];
    double value2 = _WT[indexBase+1];
    
    // interpolate
    double value = value1 + ((value2 - value1) * indexFract);
    
    _ind += _indIncr;
    
    if ( _ind >= global.wtLen)
        _ind -= global.wtLen;
    
    return value * _amp;
}