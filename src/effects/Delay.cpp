/********************************************************************************************//*!
*
*  @file        Delay.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Delay.hpp"
#include "Global.hpp"
#include "ModDock.hpp"

#include <cmath>
#include <stdexcept>


#include <iostream>




Delay::Delay(double delayLen,
             double decayTime,
             double decayRate,
             double feedbackLevel,
             double capacity)
: EffectUnit(4,1)
{
    capacity *= Global::samplerate;
    
    buffer_.resize(capacity,0);
    
    write_ = buffer_.begin();
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDelayLen(delayLen);
    setDecayTime(decayTime);
    
    // Initialize mod docks
    mods_[DECAY_TIME].setHigherBoundary(buffer_.size());
    mods_[DECAY_TIME].setLowerBoundary(0);
    mods_[DECAY_TIME].setBaseValue(decayTime_);
    
    mods_[DECAY_RATE].setHigherBoundary(1);
    mods_[DECAY_RATE].setLowerBoundary(0);
    mods_[DECAY_RATE].setBaseValue(decayRate_);
    
    mods_[FEEDBACK].setHigherBoundary(1);
    mods_[FEEDBACK].setLowerBoundary(0);
    mods_[FEEDBACK].setBaseValue(feedback_);
    
    mods_[DRYWET].setHigherBoundary(1);
    mods_[DRYWET].setLowerBoundary(0);
    mods_[DRYWET].setBaseValue(1);
}

Delay::Delay(const Delay& other)
: EffectUnit(other),
  buffer_(other.buffer_),
  decayRate_(other.decayRate_),
  decayTime_(other.decayTime_),
  decayValue_(other.decayValue_),
  readInt_(other.readInt_),
  readFract_(other.readFract_),
  feedback_(other.feedback_)
{
    buffer_.reserve(other.buffer_.capacity());
    
    // Convert iterator to const_iterator because other is const
    const_iterator itr = other.write_;
    
    write_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
}

Delay& Delay::operator=(const Delay &other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        buffer_ = other.buffer_;
        
        buffer_.reserve(other.buffer_.capacity());
        
        decayRate_ = other.decayRate_;
        
        decayTime_ = other.decayTime_;
        
        decayValue_ = other.decayValue_;
        
        readInt_ = other.readInt_;
        
        readFract_ = other.readFract_;
        
        feedback_ = other.feedback_;
        
        // Convert iterator to const_iterator because other is const
        const_iterator itr = other.write_;
        
        write_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
    }
    
    return *this;
}

void Delay::setDryWet(double dw)
{
    // For error checking
    EffectUnit::setDryWet(dw);

    mods_[DRYWET].setBaseValue(dw);
}

double Delay::getDryWet() const
{
    if (mods_[DRYWET].inUse())
    {
        return mods_[DRYWET].getBaseValue();
    }
    
    else return dw_;
}

void Delay::setDecayRate(double decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    mods_[DECAY_RATE].setBaseValue(decayRate);
    
    decayRate_ = decayRate;
}

double Delay::getDecayRate() const
{
    if (mods_[DECAY_RATE].inUse())
    {
        return mods_[DECAY_RATE].getBaseValue();
    }
    
    else return decayRate_;
}

void Delay::setDelayLen(double delayLen)
{
    delayLen *= Global::samplerate;
    
    if (delayLen < 0 || delayLen >= buffer_.size())
    { throw std::invalid_argument("Delay line length cannot be less than 0 or greater/equal the delay line capacity!"); }
    
    // Resize the buffer to the new length
    end_ = buffer_.begin() + delayLen;
    
    if (write_ >= end_)
    {
        write_ = buffer_.begin();
    }
    
    else
    {
        //for(iterator itr = write_; itr != end_; ++itr)
        //{
          //  *itr = 0;
        //}
    }
    
    delayLen_ = delayLen;
    
    delayLen -= 1;
    
    // Convert to int
    readInt_ = delayLen;
    
    // delayLen is double so get the fractional part
    // by subtracting the integer part
    readFract_ = delayLen - readInt_;
    
    calcDecay_();
}

double Delay::getDelayLen() const
{
    // seconds not samples
    return buffer_.size() / (static_cast<double>(Global::samplerate));
}

void Delay::setFeedback(double feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    { throw std::invalid_argument("Feedback level must be between 0 and 1!"); }

    mods_[FEEDBACK].setBaseValue(feedbackLevel);
    
    feedback_ = feedbackLevel;
}

double Delay::getFeedback() const
{
    if (mods_[FEEDBACK].inUse())
    {
        return mods_[FEEDBACK].getBaseValue();
    }
    
    else return feedback_;
}

void Delay::setDecayTime(double decayTime)
{
    decayTime *= Global::samplerate;
    
    if (decayTime < 0)
    { throw std::invalid_argument("Decay time must be greater or equal 0!"); }

    mods_[DECAY_TIME].setBaseValue(decayTime);
    
    decayTime_ = decayTime;
    
    calcDecay_();
}

double Delay::getDecayTime() const
{
    if (mods_[DECAY_TIME].inUse())
    {
        return mods_[DECAY_TIME].getBaseValue() / Global::samplerate;
    }
    
    // return seconds, not samples
    else return decayTime_ / Global::samplerate;
}

void Delay::calcDecay_()
{
    if (! decayTime_)
    {
        decayValue_ = 1;
    }
    
    else
    {
        double decayExponent = static_cast<double>(delayLen_) / decayTime_;
    
        decayValue_ = pow(decayRate_, decayExponent);
    }
}

void Delay::update_()
{
    if (++write_ >= end_)
    {
        write_ = buffer_.begin();
    }
}

double Delay::offset(unsigned int offset)
{
    iterator ret = write_ - offset;
    
    if (ret < buffer_.begin())
    { ret += delayLen_; }
    
    return *ret;
}

double Delay::process(double sample)
{
    if (mods_[DECAY_TIME].inUse() ||
        mods_[DECAY_RATE].inUse())
    {
        // Modulate decay time
        if (mods_[DECAY_TIME].inUse())
        {
            decayTime_ = mods_[DECAY_TIME].tick();
        }
        
        // Modulate decay rate
        if (mods_[DECAY_RATE].inUse())
        {
            decayRate_ = mods_[DECAY_RATE].tick();
        }
        
        calcDecay_();
    }
    
    // Modulate feedback value
    if (mods_[FEEDBACK].inUse())
    {
        feedback_ = mods_[FEEDBACK].tick();
    }
    
    // Modulate dry wet value
    if (mods_[DRYWET].inUse())
    {
        dw_ = mods_[DRYWET].tick();
    }
    
    iterator read = write_ - readInt_;
    
    // Check if we need to wrap around
    if (read < buffer_.begin())
    {
        read += delayLen_;
    }
    
    // If the read index is equal to the write index
    // we need to first write the new sample and increment
    if (! readInt_)
    {
        *write_ = sample;
        
        update_();
    }
    
    // First add integer part
    double output = *read;
    
    // Check if the current read position is the beginning
    // of the buffer (in which case incrementing means going
    // to the last index), else the iterator is decremented
    if (--read < buffer_.begin())
    {
        read += delayLen_;
    }
    
    // And finally add the fractional part of the
    // previous sample
    output += (*read - output) * readFract_;
    
    // Apply decay
    output *= decayValue_;

    // If the sample hasn't been written yet, write it now
    if (readInt_)
    {
        *write_ = sample + (output * feedback_);
        
        update_();
    }
    
    return dryWet_(sample, output);
}

double AllPassDelay::process(double sample)
{
    iterator read = write_ - readInt_;
    
    // Check if we need to wrap around
    if (read < buffer_.begin())
    {
        read += buffer_.size();
    }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (! readInt_)
    {
        *write_ = sample;
        
        update_();
    }
    
    // First add integer part
    double outputA = *read;
    
    // Check if the current read position is the beginning
    // of the buffer (in which case incrementing means going
    // to the last index), else the iterator is decremented
    if (read-- == buffer_.begin())
    {
        read = buffer_.end() - 1;
    }
    
    // And finally add the fractional part of the
    // previous sample
    outputA += (*read - outputA) * readFract_;
    
    double outputB = sample - (outputA * decayValue_);
    
    // If the sample hasn't been written yet, write it now
    if (readInt_)
    {
        *write_ = outputB;
        
        update_();
    }
    
    return outputA + (outputB * decayValue_);
}

Echo::Echo(double delayLen,
           double decayTime,
           double decayRate,
           double feedbackLevel,
           double capacity)
: Delay(delayLen,decayTime,decayRate,feedbackLevel,capacity)
{ }

double Echo::process(double sample)
{
    return sample + Delay::process(sample);
}
