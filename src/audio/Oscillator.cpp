/********************************************************************************************//*!
*
*  @file        Oscillator.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Oscillator.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "Wavetable.hpp"

#include <stdexcept>

Oscillator::Oscillator(unsigned short wt,
                       double freq,
                       short phaseOffset)
: _index(0),
  _phaseOffset(phaseOffset),
  _wavetable(wavetableDatabase[wt])
{
    setPhaseOffset(phaseOffset);
    
    setFrequency(freq);
}

Oscillator::Oscillator(const Oscillator& other)
: _index(other._index),
  _phaseOffset(other._phaseOffset),
  _freq(other._freq),
  _incr(other._incr),
   _wavetable(other._wavetable)
{ }

Oscillator& Oscillator::operator=(const Oscillator &other)
{
    if (&other != this)
    {
        _index = other._index;
        
        _incr = other._incr;
        
        _phaseOffset = other._phaseOffset;
        
        _freq = other._freq;
        
        _wavetable.reset(new Wavetable(*other._wavetable));
    }
    
    return *this;
}

Oscillator::~Oscillator() = default;

void Oscillator::setWavetable(unsigned short id)
{
    _wavetable = wavetableDatabase[id];
}

std::shared_ptr<Wavetable> Oscillator::getWavetable() const
{
    return _wavetable;
}

void Oscillator::setFrequency(double Hz)
{
    //if (Hz < 0 || Hz > Global::nyquistLimit)
    //{ throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    _freq = Hz;
    
    _incr = Global::tableIncrement * Hz;
}

double Oscillator::getFrequency() const
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
    _index -= _phaseOffset;
    
    // The wavetable holds 360 degrees, so divide the degrees
    // by 360 to get e.g. 1/4 and multiply by the wavetable's length
    // to get the number of samples to shift by
    _phaseOffset = ((Global::wavetableLength + 1) * degrees) / 360.0;
    
    // Add new offset
    _index += _phaseOffset;
}

double Oscillator::getPhaseOffset() const
{
    return (_phaseOffset * 360) / (Global::wavetableLength + 1);
}

void Oscillator::reset()
{
    _index = _phaseOffset;
}

void Oscillator::_increment(double value)
{
    // Increment wavetable index
    _index += value;
    
    // Check index against wavetable length
    if ( _index >= Global::wavetableLength)
    { _index -= Global::wavetableLength; }
    
    if ( _index < 0)
    { _index += Global::wavetableLength; }
}

void Oscillator::update()
{
    _increment(_incr);
}

double Oscillator::tick()
{
    // Grab a value through interpolation from the wavetable
    return _wavetable->interpolate(_index);
}
