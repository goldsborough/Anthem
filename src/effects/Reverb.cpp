/********************************************************************************************//*!
*
*  @file        Effects.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Reverb.hpp"
#include "Delay.hpp"
#include "LFO.hpp"
#include "Global.hpp"
#include "ModDock.hpp"

#include <stdexcept>
#include <cmath>

Reverb::Reverb(double reverbTime, double reverbRate, double dryWet)
: EffectUnit(3),
  delays_(new Delay[4]),
  allPasses_(new AllPassDelay [2])
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2) allPasses_[i].setActive(true);
        
        delays_[i].setActive(true);
        delays_[i].setDecayTime(reverbTime);
    }
    
    delays_[0].setDelayTime(0.0437);
    delays_[1].setDelayTime(0.0411);
    delays_[2].setDelayTime(0.0371);
    delays_[3].setDelayTime(0.0297);
    
    allPasses_[0].setDecayTime(0.0050);
    allPasses_[0].setDelayTime(0.09638);
    
    allPasses_[1].setDecayTime(0.0017);
    allPasses_[1].setDelayTime(0.03292);
    
    setReverbRate(reverbRate);
    setReverbTime(reverbTime);
    setDryWet(dryWet);
    
    // Initialize ModDocks
    mods_[REVERB_RATE].setHigherBoundary(1);
    mods_[REVERB_RATE].setLowerBoundary(0);
    mods_[REVERB_RATE].setBaseValue(reverbRate);
    
    mods_[REVERB_TIME].setHigherBoundary(100);
    mods_[REVERB_TIME].setLowerBoundary(0);
    mods_[REVERB_TIME].setBaseValue(reverbTime);
    
    mods_[DRYWET].setHigherBoundary(1);
    mods_[DRYWET].setLowerBoundary(0);
    mods_[DRYWET].setBaseValue(dryWet);
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
    
    mods_[DRYWET].setBaseValue(dw);
    
    dw_ = dw;
    
    attenuation_ = 1 - dw_;
    
    if (attenuation_ < 0.25)
    { attenuation_ = 0.25; }
}

double Reverb::getDryWet() const
{
    if (mods_[DRYWET].inUse())
    {
        return mods_[DRYWET].getBaseValue();
    }
    
    else return dw_;
}

double Reverb::process(double sample)
{
    // Modulate time
    if (mods_[REVERB_TIME].inUse())
    {
        double newReverbTime = mods_[REVERB_TIME].tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            delays_[i].setDecayTime(newReverbTime);
        }
    }
    
    // Modulate rate
    if (mods_[REVERB_RATE].inUse())
    {
        double newReverbRate = mods_[REVERB_RATE].tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            delays_[i].setDecayRate(newReverbRate);
        }
    }
    
    // Modulate the dry/wet
    if (mods_[DRYWET].inUse())
    {
        dw_ = mods_[DRYWET].tick();
    }
    
    sample *= attenuation_;
    
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += delays_[i].process(sample);
    }
    
    output = allPasses_[1].process(allPasses_[0].process(output));
    
    return dryWet_(sample, output);
}

void Reverb::setReverbRate(double reverbRate)
{
    if (reverbRate < 0 || reverbRate > 1)
    { throw std::invalid_argument("Reverb rate must be between 0 and 1!"); }
    
    mods_[REVERB_RATE].setBaseValue(reverbRate);
    
    reverbRate_ = reverbRate;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        delays_[i].setDecayRate(reverbRate);
    }
}

double Reverb::getReverbRate() const
{
    if (mods_[REVERB_RATE].inUse())
    {
        return mods_[REVERB_RATE].getBaseValue();
    }
    
    else return reverbRate_;
}

void Reverb::setReverbTime(double reverbTime)
{
    if (reverbTime < 0 || reverbTime > 100)
    { throw std::invalid_argument("Reverb time must be between 0 and 100!"); }
    
    mods_[REVERB_TIME].setBaseValue(reverbTime);
    
    reverbTime_ = reverbTime;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        delays_[i].setDecayTime(reverbTime);
    }
}

double Reverb::getReverbTime() const
{
    if (mods_[REVERB_TIME].inUse())
    {
        return mods_[REVERB_TIME].getBaseValue();
    }
    
    else return reverbTime_;
}