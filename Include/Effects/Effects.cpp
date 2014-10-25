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
    delays_ = new Delay*[4];
    allPasses_ = new AllPassDelay*[2];
    
    delays_[0] = new Delay(0.0437,reverbTime);
    delays_[1] = new Delay(0.0411,reverbTime);
    delays_[2] = new Delay(0.0371,reverbTime);
    delays_[3] = new Delay(0.0297,reverbTime);
    
    allPasses_[0] = new AllPassDelay(0.09638,0.0050);
    allPasses_[1] = new AllPassDelay(0.03292,0.0017);
    
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
    
    attenuation_ = 1 - dw_;
    
    if (attenuation_ < 0.25)
    { attenuation_ = 0.25; }
}

double Reverb::process(double sample)
{
    // Modulate time
    if (mods_[REVERB_TIME]->inUse())
    {
        double newReverbTime = mods_[REVERB_TIME]->tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            delays_[i]->setDecayTime(newReverbTime);
        }
    }
    
    // Modulate rate
    if (mods_[REVERB_RATE]->inUse())
    {
        double newReverbRate = mods_[REVERB_RATE]->tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            delays_[i]->setDecayRate(newReverbRate);
        }
    }
    
    sample *= attenuation_;
    
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += delays_[i]->process(sample);
    }
    
    output = allPasses_[1]->process(allPasses_[0]->process(output));
    
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
    
    reverbRate_ = reverbRate;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        delays_[i]->setDecayRate(reverbRate);
    }
}

void Reverb::setReverbTime(double reverbTime)
{
    if (reverbTime < 0 || reverbTime > 100)
    { throw std::invalid_argument("Reverb time must be between 0 and 100!"); }
    
    mods_[REVERB_TIME]->setBaseValue(reverbTime);
    
    reverbTime_ = reverbTime;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        delays_[i]->setDecayTime(reverbTime);
    }
}

Reverb::~Reverb()
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2)
        { delete allPasses_[i]; }
        
        delete delays_[i];
    }
    
    delete [] delays_;
    delete [] allPasses_;
}

Flanger::Flanger(const double& center,
                 const double& depth,
                 const double& rate,
                 const double& feedback)
: center_(center), depth_(depth/2),
  feedback_(feedback), lfo_(new LFO(0,rate)),
  delay_(10,1,1,0)
{
    
}

void Flanger::setRate(const double& rate)
{
    lfo_->setFreq(rate);
}

void Flanger::setCenter(const double& center)
{
    center_ = center;
    
    delay_.setDelayLen(center_);
}

void Flanger::setDepth(const double& depth)
{
    depth_ = depth;
}

void Flanger::setFeedback(const double& feedback)
{
    if (feedback < 0 || feedback > 1)
    { throw std::invalid_argument("Feedback must be > 0 and and <= 1!"); }
    
    feedback_ = feedback;
}

double Flanger::process(double sample)
{
    double output = sample;
    
    if (feedback_)
    { output -= delay_.offset(center_ * Global::samplerate) * feedback_; }
    
    //double val = center_ + (depth_ * lfo_->tick());
    
    //delay_.setDelayLen(val);
    
    output += delay_.process(output);
    
    return dryWet_(sample, output);
}

Flanger::~Flanger()
{
    delete lfo_;
}