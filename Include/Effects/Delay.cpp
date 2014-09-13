//
//  Delay.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Delay.h"
#include "Global.h"
#include "ModDock.h"

#include <cmath>
#include <stdexcept>

Delay::Delay(const double& delayLen,
             const double& decayTime,
             const double& decayRate,
             const double& feedbackLevel)
: _delayCapacity(delayLen * Global::samplerate)
{
    _delayLen = _delayCapacity;
    
    _buffer = new double[_delayLen];
    
    _write = _buffer;
    _end = _capacity = _buffer + _delayLen;
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
    
    _readInt = (int) _delayLen;
    _readFract = _delayLen - (double) _readInt;
}

void Delay::_initModDocks()
{
    _mods = {
             new ModDock(2),
             new ModDock(2),
             new ModDock(2),
             new ModDock(2)
        };
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
    { _write -= _delayLen; }
}

double Delay::offset(const unsigned int &offset)
{
    double * ret = _write - offset;
    
    if (ret < _buffer)
    { ret += _delayLen; }
    
    return *ret;
}

double Delay::process(double sample)
{
    // Modulate decay time
    if (_mods[TIME]->inUse())
    { setDecayTime(_mods[TIME]->checkAndTick(_decayTime, 0, _delayLen)); }
    
    // Modulate decay rate
    if (_mods[RATE]->inUse())
    { setDecayRate(_mods[RATE]->checkAndTick(_decayRate, 0, 1)); }
    
    // Modulate feedback value
    if (_mods[FEEDBACK]->inUse())
    { setDecayRate(_mods[FEEDBACK]->checkAndTick(_feedback, 0, 1)); }
    
    // Modulate dry wet value
    if (_mods[DRYWET]->inUse())
    { setDryWet(_mods[DRYWET]->checkAndTick(_dw, 0, 1)); }
    
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

    // If the sample hasn't been written yet, write it now
    if (_readInt > 0)
    {
        *_write = sample + (output * _feedback);
        
        _incr();
    }
    
    // Apply decay
    output *= _decayValue;
    
    return _dryWet(sample, output);
}

Delay::~Delay()
{
    delete [] _buffer;
}

double AllPassDelay::process(double sample)
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
    { read += _delayLen; }
    
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