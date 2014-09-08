//
//  Noise.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Noise.h"
#include "Filter.h"

#include <cstdlib>
#include <stdexcept>

Noise::Noise(const unsigned short& type, const double& amp)
: _filter(new Filter(Filter::LOW_PASS,1,0.1)), _amp(amp)
{
    setType(type);
}

Noise::~Noise()
{ delete _filter; }

void Noise::setAmp(const double& amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    _amp = amp;
}

void Noise::setType(const unsigned short& type)
{
    // Check if type argument is out of range
    if (type > RED)
    { throw std::invalid_argument("Invalid noise type!"); }
    
    // Adjust filter parameters according to noise color
    if (type == PINK && _type != PINK)
    {
        _filter->setCutoff(10000);
        _filter->setGain(6);
    }
    
    else if (type == RED && _type != RED)
    {
        _filter->setCutoff(1500);
        _filter->setGain(14);
    }
    
    _type = type;
}

double Noise::tick()
{
    static double randHalf = RAND_MAX/2.0;
    
    // Get random noise
    double value = (rand() - randHalf) / randHalf;
    
    // All noise colors except white noise are filtered
    if (_type != WHITE)
    { value  = _filter->process(value); }
    
    return value * _amp;
}