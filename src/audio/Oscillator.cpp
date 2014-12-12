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

Oscillator::Oscillator(unsigned short wt, double frq, short phaseOffset)
: ind_(0), phaseOffset_(phaseOffset),
  wt_(new Wavetable(wavetableDatabase[wt]))
{
    setPhaseOffset(phaseOffset);
    
    setFrequency(frq);
}

Oscillator::Oscillator(const Oscillator& other)
: ind_(other.ind_), phaseOffset_(other.phaseOffset_),
  freq_(other.freq_), indIncr_(other.indIncr_),
  wt_(new Wavetable(*other.wt_))
{ }

Oscillator& Oscillator::operator=(const Oscillator &other)
{
    if (&other != this)
    {
        ind_ = other.ind_;
        
        indIncr_ = other.indIncr_;
        
        phaseOffset_ = other.phaseOffset_;
        
        freq_ = other.freq_;
        
        wt_.reset(new Wavetable(*other.wt_));
    }
    
    return *this;
}

Oscillator::~Oscillator()
{
    
}

void Oscillator::setWavetable(short wt)
{
    wt_.reset(new Wavetable(wavetableDatabase[wt]));
}

short Oscillator::getWavetableID() const
{
    return wt_->id();
}

void Oscillator::setFrequency(double Hz)
{
    //if (Hz < 0 || Hz > Global::nyquistLimit)
    //{ throw std::invalid_argument("Frequency must be greater 0 and less than the nyquist limit!"); }
    
    freq_ = Hz;
    
    indIncr_ = Global::tableIncr * Hz;
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
    ind_ -= phaseOffset_;
    
    // The wavetable holds 360 degrees, so divide the degrees
    // by 360 to get e.g. 1/4 and multiply by the wavetable's length
    // to get the number of samples to shift by
    phaseOffset_ = ((Global::wtLen + 1) * degrees) / 360.0;
    
    // Add new offset
    ind_ += phaseOffset_;
}

double Oscillator::getPhaseOffset() const
{
    return (phaseOffset_ * 360) / (Global::wtLen + 1);
}

void Oscillator::reset()
{
    ind_ = phaseOffset_;
}

void Oscillator::increment_(double value)
{
    // Increment wavetable index
    ind_ += value;
    
    // Check index against wavetable length
    if ( ind_ >= Global::wtLen)
    { ind_ -= Global::wtLen; }
    
    if ( ind_ < 0)
    { ind_ += Global::wtLen; }
}

void Oscillator::update()
{
    increment_(indIncr_);
}

double Oscillator::tick()
{
    // Grab a value through interpolation from the wavetable
    return wt_->interpolate(ind_);
}
