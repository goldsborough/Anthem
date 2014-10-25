#include "Noise.h"
#include "Filter.h"
#include "ModDock.h"

#include <ctime>
#include <stdexcept>

Noise::Noise(unsigned short color, double amp)
: GenUnit(1,amp), filter_(new Filter(Filter::LOW_PASS,1,0.1)),
  dist_(-1,1)
{
    // Seed random number generator
    rgen_.seed((unsigned)time(0));
    
    setColor(color);
    
    mods_[AMP]->setHigherBoundary(1);
    mods_[AMP]->setLowerBoundary(0);
    mods_[AMP]->setBaseValue(amp);
}

void Noise::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    GenUnit::setAmp(amp);
    
    if (mods_[AMP]->inUse())
    {
        mods_[AMP]->setBaseValue(amp);
    }
}

double Noise::getAmp() const
{
    if (mods_[AMP]->inUse())
    {
        return mods_[AMP]->getBaseValue();
    }
    
    else return amp_;
}

void Noise::setColor(unsigned short color)
{
    // Check if color argument is out of range
    if (color > VIOLET)
    { throw std::invalid_argument("Invalid noise color!"); }
    
    // Adjust filter settings according to noise color
    // Values determined empirically
    switch (color)
    {
        case PINK:
        {
            // Pink noise has a decrease of 3dB/Octave
            
            filter_->setMode(Filter::LOW_PASS);
            filter_->setCutoff(10000);
            filter_->setGain(6);
            
            break;
        }
            
        case RED:
        {
            // Red noise has a decrease of 6dB/Octave
            
            filter_->setMode(Filter::LOW_PASS);
            filter_->setCutoff(1500);
            filter_->setGain(14);
            
            break;
        }
            
        case BLUE:
        {
            // Blue noise has an increase of 3dB/Octave
            
            filter_->setMode(Filter::HIGH_PASS);
            filter_->setCutoff(1000);
            filter_->setQ(0.2);
            filter_->setGain(-3);
            
            break;
        }
            
        case VIOLET:
        {
            // Violet noise has an increase of 6dB/Octave
            
            filter_->setMode(Filter::HIGH_PASS);
            filter_->setCutoff(6000);
            filter_->setQ(0.2);
            filter_->setGain(3);
        }
            
        default:
            break;
    }
    
    color_ = color;
}

unsigned short Noise::getColor() const
{
    return color_;
}

void Noise::increment()
{
    // Get random value
    rval_ = dist_(rgen_);
    
    // All noise colors except white noise are filtered
    if (color_ != WHITE)
    {
        rval_  = filter_->process(rval_);
    }
}

double Noise::tick()
{
    // Check modulation dock for the amplitude parameter
    if (mods_[AMP]->inUse())
    {
        amp_ = mods_[AMP]->tick();
    }
    
    return rval_ * amp_;
}