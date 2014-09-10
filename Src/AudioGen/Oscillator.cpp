//
//  Oscillator.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Oscillator.h"
#include "Global.h"
#include "Util.h"
#include "Wavetable.h"

#include <stdexcept>

Oscillator::Oscillator(const unsigned short& wt, const double& frq,
                       const double& amp, const short& phaseOffset)
: _ind(0), _indIncr(0), _phaseOffset(0)
{
    _amp = amp;
    
    setPhaseOffset(phaseOffset);
    
    _wt = wavetableDB[wt];
    
    setFreq(frq);
};

void Oscillator::setSemis(const short& semis, bool permanent)
{
    // Prevent unnecesessary changes
    if (! semis) return;
    
    // Add or subtract the semitones from the current frequency
    double newFreq = Util::semiToFreq(_freq, semis);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
    
    // calculate new index increment
    _indIncr = Global::tableIncr * newFreq;
    
    // If the change should be permanent, set the new frequency
    // to the base frequency
    if (permanent) _freq = newFreq;
    
}

void Oscillator::setCents(const short& cents, bool permanent)
{
    // Prevent unnecesessary changes
    if (! cents) return;
    
    // Add or subtract the cents from the current frequency
    double newFreq = Util::centToFreq(_freq, cents);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
        
    // calculate new index increment
    _indIncr = Global::tableIncr * newFreq;
    
    // If the change should be permanent, set the new frequency
    // to the base frequency
    if (permanent) _freq = newFreq;
}

void Oscillator::setFreq(const double& Hz)
{
    if (Hz < 0 || Hz > Global::nyquistLimit)
    { throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    _freq = Hz;
    _indIncr = Global::tableIncr * Hz;
}

void Oscillator::setPhaseOffset(short degrees)
{
    // convert degrees higher or lower than 360 or
    // less than 0 to its 0 - 360 degree equivalent
    
    while (degrees < 0)
    { degrees += 360; }
    
    while (degrees > 360)
    { degrees -= 360; }
    
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
    if (_mode == WavetableDB::NONE) return 0;
    
    double value = _WT.interpolate(_ind);
    
    _ind += _indIncr;
    
    if ( _ind >= Global::wtLen)
        _ind -= Global::wtLen;
    
    return value * _amp;
}