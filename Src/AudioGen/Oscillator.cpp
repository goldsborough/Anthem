//
//  Oscillator.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Oscillator.h"
#include "Global.h"
#include "Util.h"
#include "Wavetable.h"

Oscillator::Oscillator(const int mode, double frq,
                       double amp, short phaseOffset)
: _ind(0), _indIncr(0), _phaseOffset(0)
{
    _amp = amp;
    
    _mode = mode;
    
    setPhaseOffset(phaseOffset);
    
    _WT = wavetableDB[_mode];
    
    setFreq(frq);
};

void Oscillator::setSemis(short semis, bool permanent)
{
    if (semis == 0) return;
    
    double newFreq = Util::semiToFreq(_freq, semis);
    
    if (newFreq < Global::nyquistLimit)
    {
        _indIncr = Global::tableIncr * newFreq;
        
        if (permanent) _freq = newFreq;
    }
    
}

void Oscillator::setCents(short cents, bool permanent)
{
    if (cents == 0) return;
    
    double newFreq = Util::centToFreq(_freq, cents);
    
    if (newFreq < Global::nyquistLimit)
    {
        _indIncr = Global::tableIncr * newFreq;
        
        if (permanent) _freq = newFreq;
    }
}

void Oscillator::setFreq(double Hz)
{
    _freq = Hz;
    _indIncr = Global::tableIncr * Hz;
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
    
    _phaseOffset = ((Global::wtLen + 1) * degrees) / 360.0;
    
    // Add new offset
    _ind += _phaseOffset;
}

double Oscillator::tick()
{
    if (_mode == WavetableDB::NONE)
        return 0;
    
    double value = _WT.interpolate(_ind);
    
    _ind += _indIncr;
    
    if ( _ind >= Global::wtLen)
        _ind -= Global::wtLen;
    
    return value * _amp;
}