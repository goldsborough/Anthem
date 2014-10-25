//
//  Delay.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Delay.h"
#include "Global.h"
#include "ModDock.h"

#include <cmath>
#include <stdexcept>

Delay::Delay(double delayLen,
             double decayTime,
             double decayRate,
             double feedbackLevel,
             double capacity)
: EffectUnit(4,1),
  buffer_(capacity * Global::samplerate)
{
    write_ = buffer_.begin();
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
    setDelayLen(delayLen);
    
    // Initialize mod docks
    mods_[DECAY_TIME]->setHigherBoundary(buffer_.size());
    mods_[DECAY_TIME]->setLowerBoundary(0);
    mods_[DECAY_TIME]->setBaseValue(decayTime_);
    
    mods_[DECAY_RATE]->setHigherBoundary(1);
    mods_[DECAY_RATE]->setLowerBoundary(0);
    mods_[DECAY_RATE]->setBaseValue(decayRate_);
    
    mods_[FEEDBACK]->setHigherBoundary(1);
    mods_[FEEDBACK]->setLowerBoundary(0);
    mods_[FEEDBACK]->setBaseValue(feedback_);
    
    mods_[DRYWET]->setHigherBoundary(1);
    mods_[DRYWET]->setLowerBoundary(0);
    mods_[DRYWET]->setBaseValue(1);
}

Delay::Delay(const Delay& other)
: EffectUnit(other),
  buffer_(other.buffer_),
  delayLen_(other.delayLen_),
  decayRate_(other.decayRate_),
  decayTime_(other.decayTime_),
  decayValue_(other.decayValue_),
  readInt_(other.readInt_),
  readFract_(other.readFract_),
  feedback_(other.feedback_)
{
    // Convert iterator to const_iterator because other is const
    const_iterator itr = other.write_;
    
    write_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
    
    itr = other.end_;
    
    end_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
}

Delay& Delay::operator=(const Delay &other)
{
    if (this != &other)
    {
        EffectUnit::operator=(other);
        
        buffer_ = other.buffer_;
        
        decayRate_ = other.decayRate_;
        
        decayTime_ = other.decayTime_;
        
        decayValue_ = other.decayValue_;
        
        delayLen_ = other.delayLen_;
        
        readInt_ = other.readInt_;
        
        readFract_ = other.readFract_;
        
        feedback_ = other.feedback_;
        
        // Convert iterator to const_iterator because other is const
        const_iterator itr = other.write_;
        
        write_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
        
        itr = other.end_;
        
        end_ = buffer_.begin() + std::distance(other.buffer_.begin(), itr);
    }
    
    return *this;
}

void Delay::setDryWet(double dw)
{
    if (mods_[DRYWET]->inUse())
    {
        mods_[DRYWET]->setBaseValue(dw);
    }
    
    // For error checking
    EffectUnit::setDryWet(dw);
}

void Delay::setDecayRate(double decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    if (mods_[DECAY_RATE]->inUse())
    {
        mods_[DECAY_RATE]->setBaseValue(decayRate);
    }
    
    decayRate_ = decayRate;
}

double Delay::getDecayRate() const
{
    if (mods_[DECAY_RATE]->inUse())
    {
        return mods_[DECAY_RATE]->getBaseValue();
    }
    
    else return decayRate_;
}

void Delay::setDelayLen(double delayLen)
{
    if (delayLen < 0)
    { throw std::invalid_argument("Delay length cannot be negative!"); }
    
    delayLen *= Global::samplerate;
    
    if (delayLen > buffer_.size())
    { throw std::invalid_argument("Delay end position cannot be higher than maximum delay line length!"); }
    
    readInt_ = static_cast<int>(delayLen);
    readFract_ = delayLen - static_cast<double>(readInt_);
    
    // automatic conversion to int
    delayLen_ = delayLen;
    
    // First assign the new buffer end point
    end_ = buffer_.begin() + delayLen_;
    
    calcDecay_(decayRate_,decayTime_, delayLen_);
}

double Delay::getDelayLen() const
{
    // seconds not samples
    return delayLen_ / (static_cast<double>(Global::samplerate));
}

void Delay::setFeedback(double feedbackLevel)
{
    if (feedbackLevel < 0 || feedbackLevel > 1)
    { throw std::invalid_argument("Feedback level must be between 0 and 1!"); }
    
    if (mods_[FEEDBACK]->inUse())
    {
        mods_[FEEDBACK]->setBaseValue(feedbackLevel);
    }
    
    feedback_ = feedbackLevel;
}

double Delay::getFeedback() const
{
    if (mods_[FEEDBACK]->inUse())
    {
        return mods_[FEEDBACK]->getBaseValue();
    }
    
    else return feedback_;
}

void Delay::setDecayTime(double decayTime)
{
    decayTime *= Global::samplerate;
    
    if (decayTime < 0 || decayTime > buffer_.size())
    { throw std::invalid_argument("Decay time must be greater 0 and less than capacity!"); }
    
    if (mods_[DECAY_TIME]->inUse())
    {
        mods_[DECAY_TIME]->setBaseValue(decayTime);
    }
    
    decayTime_ = decayTime;
    
    calcDecay_(decayRate_,decayTime_,delayLen_);
}

double Delay::getDecayTime() const
{
    if (mods_[DECAY_TIME]->inUse())
    {
        return mods_[DECAY_TIME]->getBaseValue() / Global::samplerate;
    }
    
    // return seconds, not samples
    else return decayTime_ / Global::samplerate;
}

void Delay::calcDecay_(double decayRate, double decayTime, double delayLen)
{
    double decayExponent = static_cast<double>(delayLen) / decayTime;
    
    decayValue_ = pow(decayRate, decayExponent);
}

void Delay::incr_()
{
    if (++write_ >= end_)
    { write_ -= delayLen_; }
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
    // Modulate decay time
    if (mods_[DECAY_TIME]->inUse())
    {
        double newDecayTime = mods_[DECAY_TIME]->tick();
        
        calcDecay_(decayRate_, newDecayTime, delayLen_);
    }
    
    // Modulate decay rate
    if (mods_[DECAY_RATE]->inUse())
    {
        double newDecayRate = mods_[DECAY_RATE]->tick();
        
        calcDecay_(newDecayRate, decayTime_, delayLen_);
    }
    
    iterator read = write_ - readInt_;
    
    // Check if we need to wrap around
    if (read < buffer_.begin())
    { read += delayLen_; }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (! readInt_)
    {
        *write_ = sample;
        
        incr_();
    }
    
    // First add integer part
    double output = *read;
    
    // Then decrement read position (and check)
    if (--read < buffer_.begin())
    { read = end_; }
    
    // And finally add the fractional part of the
    // previous sample
    output += (*read - output) * readFract_;
    
    // Modulate feedback value
    if (mods_[FEEDBACK]->inUse())
    {
        feedback_ = mods_[FEEDBACK]->tick();
    }

    // If the sample hasn't been written yet, write it now
    if (readInt_ > 0)
    {
        *write_ = sample + (output * feedback_);
        
        incr_();
    }
    
    // Apply decay
    output *= decayValue_;
    
    // Modulate dry wet value
    if (mods_[DRYWET]->inUse())
    {
        double newDryWet = mods_[DRYWET]->tick();
        
        return dryWet_(sample, output, newDryWet);
    }
    
    return dryWet_(sample, output);
}

double AllPassDelay::process(double sample)
{
    iterator read = write_ - readInt_;
    
    // Check if we need to wrap around
    if (read < buffer_.begin())
    { read += delayLen_; }
    
    // If the read index is equal to the write index
    // we need to first write the new sample
    if (readInt_ == 0)
    {
        *write_ = sample;
        
        incr_();
    }
    
    // First add integer part
    double outputA = *read;
    
    // Then decrement read position (and check)
    if (--read < buffer_.begin())
    { read += delayLen_; }
    
    // And finally add the fractional part of the
    // previous sample
    outputA += (*read - outputA) * readFract_;
    
    double outputB = sample - (outputA * decayValue_);
    
    // If the sample hasn't been written yet, write it now
    if (readInt_ > 0)
    {
        *write_ = outputB;
        
        incr_();
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
