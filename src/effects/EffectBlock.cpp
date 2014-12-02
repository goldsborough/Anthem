#include "EffectBlock.hpp"
#include "Units.hpp"
#include "Reverb.hpp"
#include "Delay.hpp"

EffectBlock::EffectBlock(unsigned short effect)
: reverb_(new Reverb),
  delay_(new Delay),
  echo_(new Echo),
  active_(false)
{ }

EffectBlock::~EffectBlock()
{ }

double EffectBlock::process(double sample)
{
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
    }
    
    effectType_ = effectType;
}

unsigned short EffectBlock::getEffectType() const
{
    return effectType_;
}