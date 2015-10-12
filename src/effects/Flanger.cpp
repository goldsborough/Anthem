#include "Flanger.hpp"
#include "LFO.hpp"
#include "Delay.hpp"
#include "Global.hpp"

#include <stdexcept>

Flanger::Flanger(double center,
                 double depth,
                 double rate,
                 double feedback)
: EffectUnit(),
  _center(center),
  _feedback(feedback),
  _lfo(new LFO(WavetableDatabase::SINE,rate,depth)),
  _delay(new Delay(center,0,0,0,1))
{
    _lfo->setActive(true);
}

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
  _center(other._center),
  _feedback(other._feedback),
  _lfo(new LFO(*other._lfo)),
  _delay(new Delay(*other._delay))
{ }

Flanger::~Flanger()
{
    
}

Flanger& Flanger::operator= (const Flanger& other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        _center = other._center;
        
        _feedback = other._feedback;
        
        *_lfo = *other._lfo;
        
        *_delay = *other._delay;
    }
    
    return *this;
}

void Flanger::setRate(double rate)
{
    _lfo->setFrequency(rate);
}

double Flanger::getRate() const
{
    return _lfo->getFrequency();
}

void Flanger::setCenter(double center)
{
    _center = center;
    
    _delay->setDelayTime(_center);
}

double Flanger::getCenter() const
{
    return _center;
}

void Flanger::setDepth(double depth)
{
    _lfo->setAmp(depth);
}

double Flanger::getDepth() const
{
    return _lfo->getAmp();
}

void Flanger::setFeedback(double feedback)
{
    if (feedback < 0 || feedback > 1)
    { throw std::invalid_argument("Flanger feedback must be between 0 and 1!"); }
    
    _feedback = feedback;
}

double Flanger::process(double sample)
{
    double output = sample;
    
    // Check for feedback
    if (_feedback)
    {
        output -= _delay->offset(_center) * _feedback;
    }
    
    // Calculate new length by modulation. Modulation
    // depth and maximum are 1 because the LFO's amplitude
    // is the delay depth value
    double length = _lfo->modulate(_center, 1, 1);
    
    // Increment LFO
    _lfo->update();
    
    // Set the new length
    _delay->setDelayTime(length);
    
    // Retrieve new sample
    output += _delay->process(output);
    
    // Apply dry/wet
    return _dryWet(sample, output);
}
