#include "Flanger.hpp"
#include "LFO.hpp"
#include "Delay.hpp"
#include "Global.hpp"

Flanger::Flanger(double center,
                 double depth,
                 double rate)
: EffectUnit(),
center_(center),
lfo_(new LFO(WavetableDatabase::SINE,rate,depth/2)),
delay_(new Delay(center,0,0,0,1))
{
    lfo_->setActive(true);
}

Flanger::Flanger(const Flanger& other)
: EffectUnit(other),
center_(other.center_),
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
    lfo_->setAmp(depth/2);
}

double Flanger::process(double sample)
{
    double length = lfo_->modulate(center_, 1, 1);
    
    lfo_->update();
    
    delay_->setDelayLen(length);
    
    return sample + delay_->process(sample);
}
