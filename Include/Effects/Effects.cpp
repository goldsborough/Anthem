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
: EffectUnit(3,1),
  delays_(new Delay[4]),
  allPasses_(new AllPassDelay [2])
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        delays_[i].setDecayTime(reverbTime);
    }
    
    delays_[0].setDelayLen(0.0437);
    delays_[1].setDelayLen(0.0411);
    delays_[2].setDelayLen(0.0371);
    delays_[3].setDelayLen(0.0297);
    
    allPasses_[0].setDecayTime(0.0050);
    allPasses_[0].setDelayLen(0.09638);
    
    allPasses_[1].setDecayTime(0.0017);
    allPasses_[1].setDelayLen(0.03292);
    
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

Reverb::Reverb(const Reverb& other)
: EffectUnit(other),
  delays_(new Delay [4]),
  allPasses_(new AllPassDelay [2]),
  reverbRate_(other.reverbRate_),
  reverbTime_(other.reverbTime_),
  attenuation_(other.attenuation_)
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2)
        {
            allPasses_[i] = other.allPasses_[i];
        }
        
        delays_[i] = other.delays_[i];
    }
}

Reverb& Reverb::operator= (const Reverb& other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        reverbTime_ = other.reverbTime_;
        
        reverbRate_ = other.reverbRate_;
        
        attenuation_ = other.attenuation_;
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            if (i < 2)
            {
                allPasses_[i] = other.allPasses_[i];
            }
            
            delays_[i] = other.delays_[i];
        }
    }
    
    return *this;
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
            delays_[i].setDecayTime(newReverbTime);
        }
    }
    
    // Modulate rate
    if (mods_[REVERB_RATE]->inUse())
    {
        double newReverbRate = mods_[REVERB_RATE]->tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            delays_[i].setDecayRate(newReverbRate);
        }
    }
    
    sample *= attenuation_;
    
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += delays_[i].process(sample);
    }
    
    output = allPasses_[1].process(allPasses_[0].process(output));
    
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
        delays_[i].setDecayRate(reverbRate);
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
        delays_[i].setDecayTime(reverbTime);
    }
}

Flanger::Flanger(const double& center,
                 const double& depth,
                 const double& rate,
                 const double& feedback)
: EffectUnit(),
  center_(center), depth_(depth/2),
  feedback_(feedback),
  lfo_(new LFO(0,rate)),
  delay_( new Delay(10,1,1,0) )
{ }

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
  center_(other.center_), depth_(other.depth_),
  feedback_(other.feedback_),
  lfo_(new LFO(*other.lfo_)),
  delay_(new Delay(*other.delay_))
{ }

Flanger& Flanger::operator= (const Flanger& other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        center_ = other.center_;
        
        feedback_ = other.feedback_;
        
        depth_ = other.depth_;
        
        *lfo_ = *other.lfo_;
        
        *delay_ = *other.delay_;
    }
    
    return *this;
}

void Flanger::setRate(const double& rate)
{
    lfo_->setFreq(rate);
}

void Flanger::setCenter(const double& center)
{
    center_ = center;
    
    delay_->setDelayLen(center_);
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
    { output -= delay_->offset(center_ * Global::samplerate) * feedback_; }
    
    //double val = center_ + (depth_ * lfo_->tick());
    
    //delay_.setDelayLen(val);
    
    output += delay_->process(output);
    
    return dryWet_(sample, output);
}