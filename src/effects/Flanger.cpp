#include "Flanger.hpp"
#include "LFO.hpp"
#include "Delay.hpp"
#include "Global.hpp"

Flanger::Flanger(double center,
                 double depth,
                 double rate,
                 double feedback)
: EffectUnit(),
center_(center),
depth_(depth/2),
feedback_(feedback),
lfo_(new LFO(WavetableDatabase::SINE,rate)),
delay_(new Delay(center,0,0,0,2))
{
    lfo_->setActive(true);
}

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
center_(other.center_),
depth_(other.depth_),
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
    depth_ = depth/2;
}

void Flanger::setFeedback(const double& feedback)
{
    if (feedback < 0 || feedback > 1)
    { throw std::invalid_argument("Feedback must be > 0 and and <= 1!"); }
    
    feedback_ = feedback;
}

double Flanger::process(double sample)
{
    delay_->setDelayLen(center_ + (depth_ * lfo_->tick()));
    
    lfo_->update();
    
    return sample + delay_->process(sample);
}
