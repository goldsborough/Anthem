#include "EffectBlock.hpp"
#include "Units.hpp"
#include "Reverb.hpp"
#include "Flanger.hpp"
#include "Delay.hpp"
#include "Echo.hpp"

#include <stdexcept>

EffectBlock::EffectBlock(unsigned short effect)
: _reverb(new Reverb),
  _delay(new Delay),
  _echo(new Echo),
  _flanger(new Flanger),
  _active(false)
{
    _delay->setActive(true);
    _echo->setActive(true);
    _reverb->setActive(true);
    _flanger->setActive(true);
}

EffectBlock::~EffectBlock()
{ }

double EffectBlock::process(double sample)
{
    if (! _active) return sample;
    
    if (! _curr)
    { throw std::invalid_argument("Effect is currently NONE!"); }
    
    return _curr->process(sample);
}

Delay& EffectBlock::delay() const
{
    return *_delay;
}

Echo& EffectBlock::echo() const
{
    return *_echo;
}

Reverb& EffectBlock::reverb() const
{
    return *_reverb;
}

Flanger& EffectBlock::flanger() const
{
    return *_flanger;
}

void EffectBlock::setActive(bool state)
{
    _active = state;
}

bool EffectBlock::isActive() const
{
    return _active;
}

void EffectBlock::setEffectType(unsigned short effectType)
{
    switch (effectType)
    {
        case NONE:
            _curr = nullptr;
            break;
            
        case DELAY:
            _curr = &(*_delay);
            break;
            
        case ECHO:
            _curr = &(*_echo);
            break;
            
        case REVERB:
            _curr = &(*_reverb);
            break;
            
        case FLANGER:
            _curr = &(*_flanger);
            break;
    }
    
    _effectType = effectType;
}

unsigned short EffectBlock::getEffectType() const
{
    return _effectType;
}