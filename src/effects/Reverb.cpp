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
  _delays(new Delay[4]),
  _allPasses(new AllPassDelay [2])
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2) _allPasses[i].setActive(true);
        
        _delays[i].setActive(true);
    }
    
    _delays[0].setDelayTime(0.0437);
    _delays[1].setDelayTime(0.0411);
    _delays[2].setDelayTime(0.0371);
    _delays[3].setDelayTime(0.0297);
    
    _allPasses[0].setDecayTime(0.0050);
    _allPasses[0].setDelayTime(0.09638);
    
    _allPasses[1].setDecayTime(0.0017);
    _allPasses[1].setDelayTime(0.03292);
    
    setReverbTime(reverbTime);
    setReverbRate(reverbRate);
    setDryWet(dryWet);
    
    // Initialize ModDocks
    _mods[REVERB_RATE].setHigherBoundary(1);
    _mods[REVERB_RATE].setLowerBoundary(0);
    _mods[REVERB_RATE].setBaseValue(reverbRate);
    
    _mods[REVERB_TIME].setHigherBoundary(100);
    _mods[REVERB_TIME].setLowerBoundary(0);
    _mods[REVERB_TIME].setBaseValue(reverbTime);
    
    _mods[DRYWET].setHigherBoundary(1);
    _mods[DRYWET].setLowerBoundary(0);
    _mods[DRYWET].setBaseValue(dryWet);
}

Reverb::Reverb(const Reverb& other)
: EffectUnit(other),
  _delays(new Delay [4]),
  _allPasses(new AllPassDelay [2]),
  _reverbRate(other._reverbRate),
  _reverbTime(other._reverbTime),
  _attenuation(other._attenuation)
{
    for (unsigned short i = 0; i < 4; ++i)
    {
        if (i < 2)
        {
            _allPasses[i] = other._allPasses[i];
        }
        
        _delays[i] = other._delays[i];
    }
}

Reverb& Reverb::operator= (const Reverb& other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        _reverbTime = other._reverbTime;
        
        _reverbRate = other._reverbRate;
        
        _attenuation = other._attenuation;
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            if (i < 2)
            {
                _allPasses[i] = other._allPasses[i];
            }
            
            _delays[i] = other._delays[i];
        }
    }
    
    return *this;
}

void Reverb::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet control must be between 0 and 1!"); }
    
    _mods[DRYWET].setBaseValue(dw);
    
    _dw = dw;
    
    _attenuation = 1 - _dw;
    
    if (_attenuation < 0.25)
    { _attenuation = 0.25; }
}

double Reverb::getDryWet() const
{
    if (_mods[DRYWET].inUse())
    {
        return _mods[DRYWET].getBaseValue();
    }
    
    else return _dw;
}

double Reverb::process(double sample)
{
    // Modulate time
    if (_mods[REVERB_TIME].inUse())
    {
        double newReverbTime = _mods[REVERB_TIME].tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            _delays[i].setDecayTime(newReverbTime);
        }
    }
    
    // Modulate rate
    if (_mods[REVERB_RATE].inUse())
    {
        double newReverbRate = _mods[REVERB_RATE].tick();
        
        for (unsigned short i = 0; i < 4; ++i)
        {
            _delays[i].setDecayRate(newReverbRate);
        }
    }
    
    // Modulate the dry/wet
    if (_mods[DRYWET].inUse())
    {
        _dw = _mods[DRYWET].tick();
    }
    
    sample *= _attenuation;
    
    double output = 0;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        output += _delays[i].process(sample);
    }
    
    output = _allPasses[1].process(_allPasses[0].process(output));
    
    return _dryWet(sample, output);
}

void Reverb::setReverbRate(double reverbRate)
{
    if (reverbRate < 0 || reverbRate > 1)
    { throw std::invalid_argument("Reverb rate must be between 0 and 1!"); }
    
    _mods[REVERB_RATE].setBaseValue(reverbRate);
    
    _reverbRate = reverbRate;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        _delays[i].setDecayRate(reverbRate);
    }
}

double Reverb::getReverbRate() const
{
    if (_mods[REVERB_RATE].inUse())
    {
        return _mods[REVERB_RATE].getBaseValue();
    }
    
    else return _reverbRate;
}

void Reverb::setReverbTime(double reverbTime)
{
    if (reverbTime < 0 || reverbTime > 100)
    { throw std::invalid_argument("Reverb time must be between 0 and 100!"); }
    
    _mods[REVERB_TIME].setBaseValue(reverbTime);
    
    _reverbTime = reverbTime;
    
    for (unsigned short i = 0; i < 4; ++i)
    {
        _delays[i].setDecayTime(reverbTime);
    }
}

double Reverb::getReverbTime() const
{
    if (_mods[REVERB_TIME].inUse())
    {
        return _mods[REVERB_TIME].getBaseValue();
    }
    
    else return _reverbTime;
}