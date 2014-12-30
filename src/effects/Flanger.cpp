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
  center_(center),
  feedback_(feedback),
  lfo_(new LFO(WavetableDatabase::SINE,rate,depth)),
  delay_(new Delay(center,0,0,0,1))
{
    lfo_->setActive(true);
}

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
  center_(other.center_),
  feedback_(other.feedback_),
  lfo_(new LFO(*other.lfo_)),
  delay_(new Delay(*other.delay_))
{ }

Flanger::~Flanger()
{
    
}

Flanger& Flanger::operator= (const Flanger& other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        center_ = other.center_;
        
        feedback_ = other.feedback_;
        
        *lfo_ = *other.lfo_;
        
        *delay_ = *other.delay_;
    }
    
    return *this;
}

void Flanger::setRate(double rate)
{
    lfo_->setFrequency(rate);
}

double Flanger::getRate() const
{
    return lfo_->getFrequency();
}

void Flanger::setCenter(double center)
{
    center_ = center;
    
    delay_->setDelayTime(center_);
}

double Flanger::getCenter() const
{
    return center_;
}

void Flanger::setDepth(double depth)
{
    lfo_->setAmp(depth);
}

double Flanger::getDepth() const
{
    return lfo_->getAmp();
}

void Flanger::setFeedback(double feedback)
{
    if (feedback < 0 || feedback > 1)
    { throw std::invalid_argument("Flanger feedback must be between 0 and 1!"); }
    
    feedback_ = feedback;
}

double Flanger::process(double sample)
{
    double output = sample;
    
    // Check for feedback
    if (feedback_)
    {
        output -= delay_->offset(center_) * feedback_;
    }
    
    // Calculate new length by modulation. Modulation
    // depth and maximum are 1 because the LFO's amplitude
    // is the delay depth value
    double length = lfo_->modulate(center_, 1, 1);
    
    // Increment LFO
    lfo_->update();
    
    // Set the new length
    delay_->setDelayTime(length);
    
    // Retrieve new sample
    output += delay_->process(output);
    
    // Apply dry/wet
    return dryWet_(sample, output);
}
