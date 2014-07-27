//
//  Delay.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Delay.h"
#include "Global.h"

#include <cmath>
#include <stdexcept>

Delay::Delay(const double& delayLen,
             const double& decayTime,
             const double& decayRate,
             const double& feedbackLevel)
{
    size_t capacity = (int) (delayLen * Global::samplerate);
    
    _buffer = new double[capacity];
    
    _curr = _buffer;
    _end = _curr + capacity;
    _tap = _end;
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
}

void Delay::setDecayRate(const double &decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    _decayRate = decayRate;
}

void Delay::setDelayLen(const double& delayLen)
{
    // First assign the new buffer tap point
    _tap = _buffer + (int)(delayLen * Global::samplerate);
    
    // Then we check if this tap point is too far out
    if (_end - _tap <= 0)
    { throw std::invalid_argument("Tap position cannot be higher than maximum delay line length!"); }
    
    // If the write point is farther progressed than the new tap point, we need
    // to wrap the write pointer back to 0 again
    else if (_tap - _curr <= 0)
    { _curr = _buffer; }
}

void Delay::setFeedback(const double& feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    { throw std::invalid_argument("Feedback level must be between 0 and 1!"); }
    
    _feedback = feedbackLevel;
}

void Delay::setDecayTime(unsigned int decayTime)
{
    /******************************************************************
    *
    * The decay is specified as the base^decayLevel, where the base
    * is the level of attenuation we want once the decay is finished
    * (0.001 = -60dB) and the exponent the total delay time divided
    * by the decay time.
    *
    * Because a high decay value leads to a quicker amplitude rise
    * for low decay times, the decay time has to be somehow proportional
    * to the decay time. I found a range between 0 and 20 times the decay
    * time to be very appropriate.
    *
    *******************************************************************/
    
    if (decayTime > 20)
    { throw std::invalid_argument("Decay cannot be more than 20 times or less than the delay time!"); }
    
    unsigned long delayLen = _tap - _buffer;
    
    decayTime *= delayLen;
    
    double decayExponent = ((double) delayLen) / decayTime;
    
    _decayValue = pow(_decayRate, decayExponent);
}

void Delay::process(double &sample)
{
    double delay = *_curr * _decayValue;
    
    *_curr++ = sample + (delay * _feedback);
    
    if (_curr == _tap)
    { _curr = _buffer; }
    
    _dryWet(sample, delay);
}

Delay::~Delay()
{
    delete [] _buffer;
}

void Echo::process(double &sample)
{
    double delay = *_curr * _decayValue;
    
    *_curr++ = sample + (delay * _feedback);
    
    if (_curr == _tap)
    { _curr = _buffer; }
    
    delay += sample;
    
    _dryWet(sample, delay);
}