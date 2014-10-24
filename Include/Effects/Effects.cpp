//
//  Reverb.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 30/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Effects.h"
#include "Delay.h"
#include "LFO.h"
#include "Global.h"
#include "ModDock.h"

#include <stdexcept>
#include <cmath>

Reverb::Reverb(double reverbTime, double reverbRate, double dryWet)

: EffectUnit(3,1)

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
    
    // Initialize ModDocks
    mods_[REVERB_RATE]->setHigherBoundary(1);
    mods_[REVERB_RATE]->setLowerBoundary(0);
    mods_[REVERB_RATE]->setBaseValue(reverbRate);
    
    mods_[REVERB_TIME]->setHigherBoundary(100);
    mods_[REVERB_TIME]->setLowerBoundary(0);
    mods_[REVERB_TIME]->setBaseValue(reverbTime);
    
    mods_[DRYWET]->setHigherBoundary(1);
    mods_[DRYWET]->setLowerBoundary(0);
    mods_[DRYWET]->setBaseValue(dryWet);
}

void Reverb::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet control must be between 0 and 1!"); }
    
    mods_[DRYWET]->setBaseValue(dw);
    
    dw_ = dw;
    
    _attenuation = 1 - dw_;
    
    if (_attenuation < 0.25)
    { _attenuation = 0.25; }
}

double Reverb::process(double sample)
{
    // Modulate time
    if (mods_[REVERB_TIME]->inUse())
    {
        double newReverbTime = mods_[REVERB_TIME]->tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            _delays[i]->setDecayTime(newReverbTime);
        }
    }
    
    // Modulate rate
    if (mods_[REVERB_RATE]->inUse())
    {
        double newReverbRate = mods_[REVERB_RATE]->tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            _delays[i]->setDecayRate(newReverbRate);
        }
    }
    
    sample *= _attenuation;
    
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += _delays[i]->process(sample);
    }
    
    output = _allPasses[1]->process(_allPasses[0]->process(output));
    
    // Modulate the dry/wet
    if (mods_[DRYWET]->inUse())
    {
        double dw = mods_[DRYWET]->tick();
        
        // Call dryWet_ with custom dry/wet value (instead of dw_)
        return dryWet_(sample, output, dw);
    }
    
    return dryWet_(sample, output);
}

void Reverb::setReverbRate(double reverbRate)
{
    if (reverbRate < 0 || reverbRate > 1)
    { throw std::invalid_argument("Reverb rate must be between 0 and 1!"); }
    
    mods_[REVERB_RATE]->setBaseValue(reverbRate);
    
    _reverbRate = reverbRate;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        _delays[i]->setDecayRate(reverbRate);
    }
}

void Reverb::setReverbTime(double reverbTime)
{
    if (reverbTime < 0 || reverbTime > 100)
    { throw std::invalid_argument("Reverb time must be between 0 and 100!"); }
    
    mods_[REVERB_TIME]->setBaseValue(reverbTime);
    
    _reverbTime = reverbTime;
    
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

Flanger::Flanger(const double& center,
                 const double& depth,
                 const double& rate,
                 const double& feedback)
: _center(center), _depth(depth/2),
  _feedback(feedback), _lfo(new LFO(0,rate)),
  _delay(10,1,1,0)
{
    
}

void Flanger::setRate(const double& rate)
{
    _lfo->setFreq(rate);
}

void Flanger::setCenter(const double& center)
{
    _center = center;
    
    _delay.setDelayLen(_center);
}

void Flanger::setDepth(const double& depth)
{
    _depth = depth;
}

void Flanger::setFeedback(const double& feedback)
{
    if (feedback < 0 || feedback > 1)
    { throw std::invalid_argument("Feedback must be > 0 and and <= 1!"); }
    
    _feedback = feedback;
}

double Flanger::process(double sample)
{
    double output = sample;
    
    if (_feedback)
    { output -= _delay.offset(_center * Global::samplerate) * _feedback; }
    
    //double val = _center + (_depth * _lfo->tick());
    
    //_delay.setDelayLen(val);
    
    output += _delay.process(output);
    
    return dryWet_(sample, output);
}

Flanger::~Flanger()
{
    delete _lfo;
}