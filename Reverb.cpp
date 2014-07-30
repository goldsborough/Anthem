//
//  Reverb.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 30/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Reverb.h"
#include "Delay.h"

#include <stdexcept>
#include <cmath>

Reverb::Reverb(const double& reverbTime,
               const double& reverbRate,
               const double& dryWet)
{
    _delays = new Delay*[4];
    _allPasses = new AllPassDelay*[2];
    
    _delays[0] = new Delay(0.0437,reverbTime);
    _delays[1] = new Delay(0.0411,reverbTime);
    _delays[2] = new Delay(0.0371,reverbTime);
    _delays[3] = new Delay(0.0297,reverbTime);
    
    _allPasses[0] = new AllPassDelay(0.09638,0.0050);
    _allPasses[1] = new AllPassDelay(0.03292,0.0017);
    
    setReverbRate(reverbRate);
    setReverbTime(reverbTime);
    setDryWet(dryWet);
}

void Reverb::setDryWet(const double& dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet control must be between 0 and 1!"); }
    
    _dw = dw;
    
    _attenuation = 1 - _dw;
    
    if (_attenuation < 0.25)
    { _attenuation = 0.25; }
}

double Reverb::process(const double& sample)
{
    double copy = sample * _attenuation;
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += _delays[i]->process(copy);
    }
    
    output = _allPasses[1]->process(_allPasses[0]->process(output));
    
    return _dryWet(sample, output);
}

void Reverb::setReverbRate(const double& reverbRate)
{
    if (reverbRate <= 0 || reverbRate > 1)
    { throw std::invalid_argument("Reverb rate must be between 0 and 1!"); }
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        _delays[i]->setDecayRate(reverbRate);
    }
}

void Reverb::setReverbTime(const double &reverbTime)
{
    if (reverbTime < 0)
    { throw std::invalid_argument("Reverb time must be greater 0"); }
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        _delays[i]->setDecayTime(reverbTime);
    }
}

Reverb::~Reverb()
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2)
        { delete _allPasses[i]; }
        
        delete _delays[i];
    }
    
    delete [] _delays;
    delete [] _allPasses;
}