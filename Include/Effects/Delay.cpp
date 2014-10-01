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

Delay::Delay(double delayLen,
             double decayTime,
             double decayRate,
             double feedbackLevel,
             double capacity)
: EffectUnit(1,4), _delayCapacity(capacity * Global::samplerate),
  _delayLen(delayLen * Global::samplerate)
{
    if (_delayLen > _delayCapacity)
    { throw std::invalid_argument("Delay line length cannot exceed its capacity!"); }
        
    _buffer = new double[_delayCapacity];
    
    _write = _buffer;
    _end = _capacity = _buffer + _delayLen;
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
    
    _readInt = (int) _delayLen;
    _readFract = _delayLen - (double) _readInt;
    
    // Initialize mod docks
    _mods[DECAY_TIME]->setHigherBoundary(_delayCapacity);
    _mods[DECAY_TIME]->setLowerBoundary(0);
    _mods[DECAY_TIME]->setBaseValue(decayTime);
    
    _mods[DECAY_RATE]->setHigherBoundary(1);
    _mods[DECAY_RATE]->setLowerBoundary(0);
    _mods[DECAY_RATE]->setBaseValue(decayRate);
    
    _mods[FEEDBACK]->setHigherBoundary(1);
    _mods[FEEDBACK]->setLowerBoundary(0);
    _mods[FEEDBACK]->setBaseValue(feedbackLevel);
    
    _mods[DRYWET]->setHigherBoundary(1);
    _mods[DRYWET]->setLowerBoundary(0);
    _mods[DRYWET]->setBaseValue(1);
}

void Delay::setDryWet(double dw)
{
    _mods[DRYWET]->setBaseValue(dw);
    
    // For error checking
    EffectUnit::setDryWet(dw);
}

void Delay::setDecayRate(double decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    _mods[DECAY_RATE]->setBaseValue(decayRate);
    
    _decayRate = decayRate;
}

double Delay::getDecayRate() const
{
    return _decayRate;
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
    
    _calcDecay(_decayRate,_decayTime, _delayLen);
}

double Delay::getDelayLen() const
{
    // seconds not samples
    return _delayLen / ((double) Global::samplerate);
}

void Delay::setFeedback(double feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    { throw std::invalid_argument("Feedback level must be between 0 and 1!"); }
    
    _mods[FEEDBACK]->setBaseValue(feedbackLevel);
    
    _feedback = feedbackLevel;
}

double Delay::getFeedback() const
{
    return _feedback;
}

void Delay::setDecayTime(double decayTime)
{
    if (decayTime < 0 || decayTime > _delayCapacity)
    { throw std::invalid_argument("Decay time must be greater 0 and less than capacity!"); }
    
    _mods[DECAY_TIME]->setBaseValue(decayTime);
    
    _decayTime = decayTime * Global::samplerate;
    
    _calcDecay(_decayRate,_decayTime,_delayLen);
}

double Delay::getDecayTime() const
{
    // return seconds, not samples
    return _decayTime / Global::samplerate;
}

void Delay::_calcDecay(double decayRate, double decayTime, double delayLen)
{
    double decayExponent = ((double) delayLen) / decayTime;
    
    _decayValue = pow(decayRate, decayExponent);
}

void Delay::_incr()
{
    if (++_write >= _end)
    { _write -= _delayLen; }
}

double Delay::offset(unsigned int offset)
{
    double* ret = _write - offset;
    
    if (ret < _buffer)
    { ret += _delayLen; }
    
    return *ret;
}

double Delay::process(double sample)
{
    // Modulate decay time
    if (_mods[DECAY_TIME]->inUse())
    {
        double newDecayTime = _mods[DECAY_TIME]->tick();
        
        _calcDecay(_decayRate, newDecayTime, _delayLen);
    }
    
    // Modulate decay rate
    if (_mods[DECAY_RATE]->inUse())
    {
        double newDecayRate = _mods[DECAY_RATE]->tick();
        
        _calcDecay(newDecayRate, _decayTime, _delayLen);
    }
    
    iterator read = _write - _readInt;
    
    // Check if we need to wrap around
    if (read < _buffer)
    { read += _delayLen; }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (! _readInt)
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
    
    double feedback = _feedback;
    
    // Modulate feedback value
    if (_mods[FEEDBACK]->inUse())
    {
        feedback = _mods[FEEDBACK]->tick();
    }

    // If the sample hasn't been written yet, write it now
    if (_readInt > 0)
    {
        *_write = sample + (output * feedback);
        
        _incr();
    }
    
    // Apply decay
    output *= _decayValue;
    
    // Modulate dry wet value
    if (_mods[DRYWET]->inUse())
    {
        double newDryWet = _mods[DRYWET]->tick();
        
        return _dryWet(sample, output, newDryWet);
    }
    
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