//
//  Noise.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Noise.h"
#include "Filter.h"
#include "ModDock.h"

#include <cstdlib>
#include <stdexcept>

Noise::Noise(unsigned short color, double amp)
: GenUnit(1,amp), _filter(new Filter(Filter::LOW_PASS,1,0.1))
{
    setColor(color);
    
    mods_[AMP]->setHigherBoundary(1);
    mods_[AMP]->setLowerBoundary(0);
    mods_[AMP]->setBaseValue(amp);
}

Noise::~Noise()
{ delete _filter; }

void Noise::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    GenUnit::setAmp(amp);
    
    // Sets new base value for modulation
    mods_[AMP]->setBaseValue(amp);
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

double Noise::tick()
{
    static double randHalf = RAND_MAX/2.0;
    
    // Get random noise
    double value = (rand() - randHalf) / randHalf;
    
    // All noise colors except white noise are filtered
    if (_color != WHITE)
    { value  = _filter->process(value); }
    
    // Check modulation dock for the amplitude parameter
    if (mods_[AMP]->inUse())
    { return value * mods_[AMP]->tick(); }
    
    return value * amp_;
}