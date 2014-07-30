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
    _delayLen = (delayLen * Global::samplerate);
    
    _buffer = new double[_delayLen];
    
    _write = _buffer;
    _end = _capacity = _buffer + _delayLen;
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
    
    _readInt = (int) _delayLen;
    _readFract = _delayLen - (double) _readInt;
}

void Delay::setDecayRate(const double &decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    _decayRate = decayRate;
}

void Delay::setDelayLen(double delayLen)
{
    if (delayLen < 0)
    { throw std::invalid_argument("Delay length cannot be negative!"); }
    
    delayLen *= Global::samplerate;
    
    // automatic conversion to int
    _delayLen = delayLen;
    
    // First assign the new buffer end point
    _end = _buffer + _delayLen;
    
    // Then we check if this end point is too far out
    if (_capacity - _end <= 0)
    { throw std::invalid_argument("Delay end position cannot be higher than maximum delay line length!"); }
    
    _readInt = (int) delayLen;
    _readFract = delayLen - (double) _readInt;
    
    _calcDecay();
}

void Delay::setFeedback(const double& feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    { throw std::invalid_argument("Feedback level must be between 0 and 1!"); }
    
    _feedback = feedbackLevel;
}

void Delay::setDecayTime(double decayTime)
{
    if (decayTime < 0)
    { throw std::invalid_argument("Decay time must be greater 0!"); }
    
    _decayTime = decayTime * Global::samplerate;
    
    _calcDecay();
}

void Delay::_calcDecay()
{
    double decayExponent = ((double) _delayLen) / _decayTime;
    
    _decayValue = pow(_decayRate, decayExponent);
}

void Delay::_incr()
{
    if (++_write >= _end)
    { _write = _buffer; }
}

double Delay::process(const double& sample)
{
    iterator read = _write - _readInt;
    
    // Check if we need to wrap around
    if (read < _buffer)
    { read += _delayLen; }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (_readInt == 0)
    {
        *_write = sample;
        
        _incr();
    }
    
    // First add integer part
    double output = *read;
    
    // Then decrement read position (and check)
    if (--read < _buffer)
    { read = _end; }
    
    // And finally add the fractional part of the
    // previous sample
    output += (*read - output) * _readFract;
    
    // Apply decay
    output *= _decayValue;

    // If the sample hasn't been written yet, write it now
    if (_readInt > 0)
    {
        *_write = sample + (output * _feedback);
        
        _incr();
    }
    
    return _dryWet(sample, output);
}

Delay::~Delay()
{
    delete [] _buffer;
}

double AllPassDelay::process(const double &sample)
{
    iterator read = _write - _readInt;
    
    // Check if we need to wrap around
    if (read < _buffer)
    { read += _delayLen; }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (_readInt == 0)
    {
        *_write = sample;
        
        _incr();
    }
    
    // First add integer part
    double outputA = *read;
    
    // Then decrement read position (and check)
    if (--read < _buffer)
    { read = _end; }
    
    // And finally add the fractional part of the
    // previous sample
    outputA += (*read - outputA) * _readFract;
    
    double outputB = sample - (outputA * _decayValue);
    
    // If the sample hasn't been written yet, write it now
    if (_readInt > 0)
    {
        *_write = outputB;
        
        _incr();
    }
    
    return outputA + (outputB * _decayValue);
}

double Echo::process(const double &sample)
{
    return sample + Delay::process(sample);
}