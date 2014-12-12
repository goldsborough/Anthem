#include "Flanger.hpp"
#include "LFO.hpp"
#include "Delay.hpp"
#include "Global.hpp"

Flanger::Flanger(const double& center,
                 const double& depth,
                 const double& rate,
                 const double& feedback)
: EffectUnit(),
center_(center), depth_(depth/2),
feedback_(feedback),
lfo_(new LFO(0,rate)),
delay_( new Delay(5,1,1,0) )
{ }

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
center_(other.center_), depth_(other.depth_),
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
        
        depth_ = other.depth_;
        
        *lfo_ = *other.lfo_;
        
        *delay_ = *other.delay_;
    }
    
    return *this;
}

void Flanger::setRate(const double& rate)
{
    lfo_->setFrequency(rate);
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
    
    double val = center_ + (depth_ * lfo_->tick());
    
    delay_->setDelayLen(val);
    
    output += delay_->process(output);
    
    return dryWet_(sample, output);
}