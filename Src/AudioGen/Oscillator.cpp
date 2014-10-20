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

Oscillator::Oscillator(short wt,
                       double frq,
                       double amp,
                       short phaseOffset)

: amp_(amp), ind_(0), phaseOffset_(phaseOffset)

{
    wt_ = wavetableDB[wt];
    
    setPhaseOffset(phaseOffset);
    
    setFreq(frq);
}

void Oscillator::setWavetable(short wt)
{
    wt_ = wavetableDB[wt];
}

short Oscillator::getWavetableID() const
{
    return wt_.id();
}

void Oscillator::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    amp_ = amp;
}

double Oscillator::getAmp() const
{
    return amp_;
}

void Oscillator::setSemitoneOffset(short semitoneOffset)
{
    // Prevent unnecesessary changes
    if (! semitoneOffset) return;
    
    // Add or subtract the semitones from the current frequency
    double newFreq = Util::semiToFreq(freq_, semitoneOffset);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
    
    // calculate new index increment
    indIncr_ = Global::tableIncr * newFreq;
    
    semitoneOffset_ = semitoneOffset;
}

short Oscillator::getSemitoneOffset() const
{
    return semitoneOffset_;
}

void Oscillator::setCentOffset(short centOffset)
{
    // Prevent unnecesessary changes
    if (! centOffset) return;
    
    // Add or subtract the centOffset from the current frequency
    double newFreq = Util::centToFreq(freq_, centOffset);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { newFreq = Global::nyquistLimit; }
        
    // calculate new index increment
    indIncr_ = Global::tableIncr * newFreq;
    
    centOffset_ = centOffset;
}

short Oscillator::getCentOffset() const
{
    return centOffset_;
}

void Oscillator::setFreq(double Hz)
{
    if (Hz < 0 || Hz > Global::nyquistLimit)
    { throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    freq_ = Hz;
    indIncr_ = Global::tableIncr * Hz;
}

double Oscillator::getFreq() const
{
    return freq_;
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
    ind_ -= phaseOffset_;
    
    // The wavetable holds 360 degrees, so divide the degrees
    // by 360 to get e.g. 1/4 and multiply by the wavetable's length
    // to get the number of samples to shift by
    phaseOffset_ = ((Global::wtLen + 1) * degrees) / 360.0;
    
    // Add new offset
    ind_ += phaseOffset_;
}

double Oscillator::getPhaseOffset() const
{
    return (phaseOffset_ * 360) / (Global::wtLen + 1);
}

void Oscillator::reset()
{
    ind_ = phaseOffset_;
}

void Oscillator::increment()
{
    // Increment wavetable index
    ind_ += indIncr_;
    
    // Check index against wavetable length
    if ( ind_ >= Global::wtLen)
    { ind_ -= Global::wtLen; }
}

double Oscillator::tick()
{
    if (wt_.id() == -1) return 0;
        
    // Grab a value through interpolation from the wavetable
    return wt_.interpolate(ind_) * amp_;
}
