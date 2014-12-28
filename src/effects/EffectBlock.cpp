#include "EffectBlock.hpp"
#include "Units.hpp"
#include "Reverb.hpp"
#include "Flanger.hpp"
#include "Delay.hpp"

#include <stdexcept>

EffectBlock::EffectBlock(unsigned short effect)
: reverb_(new Reverb),
  delay_(new Delay),
  echo_(new Echo),
  flanger_(new Flanger),
  active_(false)
{
    delay_->setActive(true);
    echo_->setActive(true);
    reverb_->setActive(true);
    flanger_->setActive(true);
}

EffectBlock::~EffectBlock()
{ }

double EffectBlock::process(double sample)
{
    if (! active_) return sample;
    
    if (! curr_)
    { throw std::invalid_argument("Effect is currently NONE!"); }
    
    return curr_->process(sample);
}

Delay& EffectBlock::delay() const
{
    return *delay_;
}

Echo& EffectBlock::echo() const
{
    return *echo_;
}

Reverb& EffectBlock::reverb() const
{
    return *reverb_;
}

Flanger& EffectBlock::flanger() const
{
    return *flanger_;
}

void EffectBlock::setActive(bool state)
{
    active_ = state;
}

bool EffectBlock::isActive() const
{
    return active_;
}

void EffectBlock::setEffectType(unsigned short effectType)
{
    switch (effectType)
    {
        case NONE:
            curr_ = nullptr;
            break;
            
        case DELAY:
            curr_ = &(*delay_);
            break;
            
        case ECHO:
            curr_ = &(*echo_);
            break;
            
        case REVERB:
            curr_ = &(*reverb_);
            break;
            
        case FLANGER:
            curr_ = &(*flanger_);
            break;
    }
    
    effectType_ = effectType;
}

unsigned short EffectBlock::getEffectType() const
{
    return effectType_;
}