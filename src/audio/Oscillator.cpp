/********************************************************************************************//*!
*
*  @file        Oscillator.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Oscillator.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "Wavetable.hpp"

#include <stdexcept>

Oscillator::Oscillator(unsigned short wt,
                       double freq,
                       short phaseOffset)
: index_(0),
  phaseOffset_(phaseOffset),
  wavetable_(wavetableDatabase[wt])
{
    setPhaseOffset(phaseOffset);
    
    setFrequency(freq);
}

Oscillator::Oscillator(const Oscillator& other)
: index_(other.index_),
  phaseOffset_(other.phaseOffset_),
  freq_(other.freq_),
  incr_(other.incr_),
   wavetable_(other.wavetable_)
{ }

Oscillator& Oscillator::operator=(const Oscillator &other)
{
    if (&other != this)
    {
        index_ = other.index_;
        
        incr_ = other.incr_;
        
        phaseOffset_ = other.phaseOffset_;
        
        freq_ = other.freq_;
        
        wavetable_.reset(new Wavetable(*other.wavetable_));
    }
    
    return *this;
}

Oscillator::~Oscillator() = default;

void Oscillator::setWavetable(unsigned short id)
{
    wavetable_ = wavetableDatabase[id];
}

std::shared_ptr<Wavetable> Oscillator::getWavetable() const
{
    return wavetable_;
}

void Oscillator::setFrequency(double Hz)
{
    //if (Hz < 0 || Hz > Global::nyquistLimit)
    //{ throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    freq_ = Hz;
    
    incr_ = Global::tableIncrement * Hz;
}

double Oscillator::getFrequency() const
{
    return freq_;
}

void Oscillator::setPhaseOffset(short degrees)
{
    // convert degrees higher or lower than 360 or
    // less than 0 to its 0 - 360 degree equivalent
    
    while (degrees < 0)
    { degrees += 360; }
    
    while (degrees > 360)
    { degrees -= 360; }
    
    // Return to original index (without offset), so
    // that setting a new offset doesn't add to the
    // old one but really set a new one
    index_ -= phaseOffset_;
    
    // The wavetable holds 360 degrees, so divide the degrees
    // by 360 to get e.g. 1/4 and multiply by the wavetable's length
    // to get the number of samples to shift by
    phaseOffset_ = ((Global::wavetableLength + 1) * degrees) / 360.0;
    
    // Add new offset
    index_ += phaseOffset_;
}

double Oscillator::getPhaseOffset() const
{
    return (phaseOffset_ * 360) / (Global::wavetableLength + 1);
}

void Oscillator::reset()
{
    index_ = phaseOffset_;
}

void Oscillator::increment_(double value)
{
    // Increment wavetable index
    index_ += value;
    
    // Check index against wavetable length
    if ( index_ >= Global::wavetableLength)
    { index_ -= Global::wavetableLength; }
    
    if ( index_ < 0)
    { index_ += Global::wavetableLength; }
}

void Oscillator::update()
{
    increment_(incr_);
}

double Oscillator::tick()
{
    // Grab a value through interpolation from the wavetable
    return wavetable_->interpolate(index_);
}
