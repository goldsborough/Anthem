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

Oscillator::Oscillator(short wt, double frq,
                       double amp, short phaseOffset)
: GenUnit(0,amp), _ind(0), _phaseOffset(phaseOffset)
{
    setWavetable(wt);
    
    setPhaseOffset(phaseOffset);
    
    setFreq(frq);
}

void Oscillator::setWavetable(short wt)
{
    _wt = wavetableDB[wt];
}

void Oscillator::setSemitoneOffset(short semitoneOffset)
{
    // Prevent unnecesessary changes
    if (! semitoneOffset) return;
    
    // Add or subtract the semitones from the current frequency
    double newFreq = Util::semiToFreq(_freq, semitoneOffset);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
    
    // calculate new index increment
    _indIncr = Global::tableIncr * newFreq;
    
    _semitoneOffset = semitoneOffset;
}

short Oscillator::getSemitoneOffset() const
{
    return _semitoneOffset;
}

void Oscillator::setCentOffset(short centOffset)
{
    // Prevent unnecesessary changes
    if (! centOffset) return;
    
    // Add or subtract the centOffset from the current frequency
    double newFreq = Util::centToFreq(_freq, centOffset);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
        
    // calculate new index increment
    _indIncr = Global::tableIncr * newFreq;
    
    _centOffset = centOffset;
}

short Oscillator::getCentOffset() const
{
    return _centOffset;
}

void Oscillator::setFreq(double Hz)
{
    if (Hz < 0 || Hz > Global::nyquistLimit)
    { throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    _freq = Hz;
    _indIncr = Global::tableIncr * Hz;
}

double Oscillator::getFreq() const
{
    return _freq;
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

void Oscillator::reset()
{
    _ind = _phaseOffset;
}

double Oscillator::tick()
{
    // Grab a value through interpolation from the wavetable
    double value = _wt.interpolate(_ind);
    
    // Increment wavetable index
    _ind += _indIncr;
    
    // Check index against wavetable length
    if ( _ind >= Global::wtLen)
    { _ind -= Global::wtLen; }
    
    return value * _amp;
}