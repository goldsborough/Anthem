/********************************************************************************************//*!
*
*  @file        Delay.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Delay.hpp"
#include "Global.hpp"
#include "ModDock.hpp"

#include <cmath>
#include <stdexcept>


#include <iostream>




Delay::Delay(double delayLength,
             double decayTime,
             double decayRate,
             double feedbackLevel,
             double capacity)
: EffectUnit(4,1),
  _buffer(capacity * Global::samplerate,0)
{
    _write = _buffer.begin();
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDelayTime(delayLength);
    setDecayTime(decayTime);
    
    // Initialize mod docks
    _mods[DECAY_TIME].setHigherBoundary(_buffer.size());
    _mods[DECAY_TIME].setLowerBoundary(0);
    _mods[DECAY_TIME].setBaseValue(_decayTime);
    
    _mods[DECAY_RATE].setHigherBoundary(1);
    _mods[DECAY_RATE].setLowerBoundary(0);
    _mods[DECAY_RATE].setBaseValue(_decayRate);
    
    _mods[FEEDBACK].setHigherBoundary(1);
    _mods[FEEDBACK].setLowerBoundary(0);
    _mods[FEEDBACK].setBaseValue(_feedback);
    
    _mods[DRYWET].setHigherBoundary(1);
    _mods[DRYWET].setLowerBoundary(0);
    _mods[DRYWET].setBaseValue(1);
}

Delay::Delay(const Delay& other)
: EffectUnit(other),
  _buffer(other._buffer),
  _decayRate(other._decayRate),
  _decayTime(other._decayTime),
  _decayValue(other._decayValue),
  _readIntegral(other._readIntegral),
  _readFractional(other._readFractional),
  _feedback(other._feedback)
{
    _buffer.reserve(other._buffer.capacity());
    
    // Convert iterator to const_iterator because other is const
    const_iterator itr = other._write;
    
    _write = _buffer.begin() + std::distance(other._buffer.begin(), itr);
}

Delay& Delay::operator=(const Delay &other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        _buffer = other._buffer;
        
        _buffer.reserve(other._buffer.capacity());
        
        _decayRate = other._decayRate;
        
        _decayTime = other._decayTime;
        
        _decayValue = other._decayValue;
        
        _readIntegral = other._readIntegral;
        
        _readFractional = other._readFractional;
        
        _feedback = other._feedback;
        
        // Convert iterator to const_iterator because other is const
        const_iterator itr = other._write;
        
        _write = _buffer.begin() + std::distance(other._buffer.begin(), itr);
    }
    
    return *this;
}

void Delay::setDryWet(double dw)
{
    // For error checking
    EffectUnit::setDryWet(dw);

    _mods[DRYWET].setBaseValue(dw);
}

double Delay::getDryWet() const
{
    if (_mods[DRYWET].inUse())
    {
        return _mods[DRYWET].getBaseValue();
    }
    
    else return _dw;
}

void Delay::setDecayRate(double decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    _mods[DECAY_RATE].setBaseValue(decayRate);
    
    _decayRate = decayRate;
}

double Delay::getDecayRate() const
{
    if (_mods[DECAY_RATE].inUse())
    {
        return _mods[DECAY_RATE].getBaseValue();
    }
    
    else return _decayRate;
}

void Delay::setDelayTime(double delayTime)
{
    delayTime *= Global::samplerate;
    
    if (delayTime < 0 || delayTime >= _buffer.size())
    {
        throw std::invalid_argument("Delay line length cannot be less than 0 or greater the delay line capacity!");
    }
    
    // Cast to int
    _readIntegral = delayTime;
    
    // delayLength is double so get the fractional part
    // by subtracting the integer part
    _readFractional = delayTime - _readIntegral;
    
    _calcDecay();
}

double Delay::getDelayTime() const
{
    // seconds not samples
    return _buffer.size() / (static_cast<double>(Global::samplerate));
}

void Delay::setFeedback(double feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    {
        throw std::invalid_argument("Feedback level must be between 0 and 1!");
    }

    _mods[FEEDBACK].setBaseValue(feedbackLevel);
    
    _feedback = feedbackLevel;
}

double Delay::getFeedback() const
{
    if (_mods[FEEDBACK].inUse())
    {
        return _mods[FEEDBACK].getBaseValue();
    }
    
    else return _feedback;
}

void Delay::setDecayTime(double decayTime)
{
    decayTime *= Global::samplerate;
    
    if (decayTime < 0)
    { throw std::invalid_argument("Decay time must be greater or equal 0!"); }

    _mods[DECAY_TIME].setBaseValue(decayTime);
    
    _decayTime = decayTime;
    
    _calcDecay();
}

double Delay::getDecayTime() const
{
    if (_mods[DECAY_TIME].inUse())
    {
        return _mods[DECAY_TIME].getBaseValue() / Global::samplerate;
    }
    
    // return seconds, not samples
    else return _decayTime / Global::samplerate;
}

void Delay::_calcDecay()
{
    if (! _decayTime)
    {
        _decayValue = 1;
    }
    
    else
    {
        double decayExponent = static_cast<double>(_readIntegral) / _decayTime;
    
        _decayValue = pow(_decayRate, decayExponent);
    }
}

double Delay::offset(unsigned int offset)
{
    iterator ret = _write - offset;
    
    if (ret < _buffer.begin())
    {
        ret += _buffer.size();
    }
    
    return *ret;
}

void Delay::_writeAndIncrement(double sample)
{
    *_write = sample;
    
    if (++_write >= _buffer.end())
    {
        _write -= _buffer.size();
    }
}

double Delay::process(double sample)
{
    if (_mods[DECAY_TIME].inUse() ||
        _mods[DECAY_RATE].inUse())
    {
        // Modulate decay time
        if (_mods[DECAY_TIME].inUse())
        {
            _decayTime = _mods[DECAY_TIME].tick();
        }
        
        // Modulate decay rate
        if (_mods[DECAY_RATE].inUse())
        {
            _decayRate = _mods[DECAY_RATE].tick();
        }
        
        _calcDecay();
    }
    
    // Modulate feedback value
    if (_mods[FEEDBACK].inUse())
    {
        _feedback = _mods[FEEDBACK].tick();
    }
    
    // Modulate dry wet value
    if (_mods[DRYWET].inUse())
    {
        _dw = _mods[DRYWET].tick();
    }
    
    const_iterator read = _write - _readIntegral;
    
    // Check if we need to wrap around
    if (read < _buffer.begin())
    {
        read += _buffer.size();
    }
    
    // If the read index is equal to the write index
    // we need to first write the new sample and increment
    if (! _readIntegral)
    {
        _writeAndIncrement(sample);
    }
    
    // First add integer part
    double output = *read;
    
    // Check if the current read position is the beginning
    // of the buffer (in which case incrementing means going
    // to the last index), else the iterator is decremented
    if (--read < _buffer.begin())
    {
        read += _buffer.size();
    }
    
    // And finally add the fractional part of the
    // previous sample
    output += (*read - output) * _readFractional;
    
    // Apply decay
    output *= _decayValue;

    // If the sample hasn't been written yet, write it now
    if (_readIntegral)
    {
        _writeAndIncrement(sample + (output * _feedback));
    }
    
    return _dryWet(sample, output);
}

double AllPassDelay::process(double sample)
{
    const_iterator read = _write - _readIntegral;
    
    // Check if we need to wrap around
    if (read < _buffer.begin())
    {
        read += _buffer.size();
    }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (! _readIntegral)
    {
        _writeAndIncrement(sample);
    }
    
    // First add integer part
    double outputA = *read;
    
    // Check if the current read position is the beginning
    // of the buffer (in which case incrementing means going
    // to the last index), else the iterator is decremented
    if (read-- == _buffer.begin())
    {
        read = _buffer.end() - 1;
    }
    
    // And finally add the fractional part of the
    // previous sample
    outputA += (*read - outputA) * _readFractional;
    
    double outputB = sample - (outputA * _decayValue);
    
    // If the sample hasn't been written yet, write it now
    if (_readIntegral)
    {
        _writeAndIncrement(outputB);
    }
    
    return outputA + (outputB * _decayValue);
}
