/********************************************************************************************//*!
*
*  @file        Noise.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Noise.hpp"
#include "Filter.hpp"
#include "ModDock.hpp"

#include <ctime>
#include <stdexcept>

Noise::Noise(unsigned short color, double amp)
: GenUnit(1,amp), _filter(new Filter(Filter::LOW_PASS,1,0.1)),
  _dist(-1,1)
{
    // Seed random number generator
    _rgen.seed((unsigned)time(0));
    
    setColor(color);
    
    _mods[AMP].setHigherBoundary(1);
    _mods[AMP].setLowerBoundary(0);
    _mods[AMP].setBaseValue(amp);
}

Noise::Noise(const Noise& other)
: GenUnit(other),
  _dist(other._dist), _color(other._color),
  _filter(new Filter(*other._filter)),
  _rval(other._rval)
{
    // Seed random number generator
    _rgen.seed((unsigned)time(0));
}

Noise& Noise::operator= (const Noise& other)
{
    if (this != &other)
    {
        GenUnit::operator=(other);
        
        _dist = other._dist;
        
        _color = other._color;
        
        _rgen = other._rgen;
        
        _rval = other._rval;
        
        *_filter = *other._filter;
    }
    
    return *this;
}

void Noise::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    GenUnit::setAmp(amp);

    _mods[AMP].setBaseValue(amp);
}

double Noise::getAmp() const
{
    if (_mods[AMP].inUse())
    {
        return _mods[AMP].getBaseValue();
    }
    
    else return _amp;
}

void Noise::setColor(unsigned short color)
{
    // Check if color argument is out of range
    if (color > VIOLET)
    { throw std::invalid_argument("Invalid noise color!"); }
    
    // Adjust filter settings according to noise color
    // Values determined empirically
    switch (color)
    {
        case PINK:
        {
            // Pink noise has a decrease of 3dB/Octave
            
            _filter->setMode(Filter::LOW_PASS);
            _filter->setCutoff(10000);
            _filter->setGain(6);
            
            break;
        }
            
        case RED:
        {
            // Red noise has a decrease of 6dB/Octave
            
            _filter->setMode(Filter::LOW_PASS);
            _filter->setCutoff(1500);
            _filter->setGain(14);
            
            break;
        }
            
        case BLUE:
        {
            // Blue noise has an increase of 3dB/Octave
            
            _filter->setMode(Filter::HIGH_PASS);
            _filter->setCutoff(1000);
            _filter->setQ(0.2);
            _filter->setGain(-3);
            
            break;
        }
            
        case VIOLET:
        {
            // Violet noise has an increase of 6dB/Octave
            
            _filter->setMode(Filter::HIGH_PASS);
            _filter->setCutoff(6000);
            _filter->setQ(0.2);
            _filter->setGain(3);
        }
            
        default:
            break;
    }
    
    _color = color;
}

unsigned short Noise::getColor() const
{
    return _color;
}

void Noise::update()
{
    // Get random value
    _rval = _dist(_rgen);
    
    // All noise colors except white noise are filtered
    if (_color != WHITE)
    {
        _rval  = _filter->process(_rval);
    }
}

double Noise::tick()
{
    // Check modulation dock for the amplitude parameter
    if (_mods[AMP].inUse())
    {
        _amp = _mods[AMP].tick();
    }
    
    return _rval * _amp;
}