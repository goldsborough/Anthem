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
: EffectUnit(4,1), delayCapacity_(capacity * Global::samplerate),
  delayLen_(delayLen * Global::samplerate)
{
    if (delayLen_ > delayCapacity_)
    { throw std::invalid_argument("Delay line length cannot exceed its capacity!"); }
        
    buffer_ = new double[delayCapacity_];
    
    write_ = buffer_;
    end_ = capacity_ = buffer_ + delayLen_;
    
    setFeedback(feedbackLevel);
    setDecayRate(decayRate);
    setDecayTime(decayTime);
    
    readInt_ = static_cast<int>(delayLen_);
    readFract_ = delayLen_ - static_cast<double>(readInt_);
    
    // Initialize mod docks
    mods_[DECAY_TIME]->setHigherBoundary(delayCapacity_);
    mods_[DECAY_TIME]->setLowerBoundary(0);
    mods_[DECAY_TIME]->setBaseValue(decayTime);
    
    mods_[DECAY_RATE]->setHigherBoundary(1);
    mods_[DECAY_RATE]->setLowerBoundary(0);
    mods_[DECAY_RATE]->setBaseValue(decayRate);
    
    mods_[FEEDBACK]->setHigherBoundary(1);
    mods_[FEEDBACK]->setLowerBoundary(0);
    mods_[FEEDBACK]->setBaseValue(feedbackLevel);
    
    mods_[DRYWET]->setHigherBoundary(1);
    mods_[DRYWET]->setLowerBoundary(0);
    mods_[DRYWET]->setBaseValue(1);
}

void Delay::setDryWet(double dw)
{
    mods_[DRYWET]->setBaseValue(dw);
    
    // For error checking
    EffectUnit::setDryWet(dw);
}

void Delay::setDecayRate(double decayRate)
{
    if (decayRate > 1 || decayRate < 0)
    { throw std::invalid_argument("Decay rate must be between 0 and 1!"); }
    
    mods_[DECAY_RATE]->setBaseValue(decayRate);
    
    decayRate_ = decayRate;
}

double Delay::getDecayRate() const
{
    return decayRate_;
}

void Delay::setDelayLen(double delayLen)
{
    if (delayLen < 0)
    { throw std::invalid_argument("Delay length cannot be negative!"); }
    
    delayLen *= Global::samplerate;
    
    // automatic conversion to int
    delayLen_ = delayLen;
    
    // First assign the new buffer end point
    end_ = buffer_ + delayLen_;
    
    // Then we check if this end point is too far out
    if (capacity_ - end_ <= 0)
    { throw std::invalid_argument("Delay end position cannot be higher than maximum delay line length!"); }
    
    readInt_ = static_cast<int>(delayLen);
    readFract_ = delayLen - static_cast<double>(readInt_);
    
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
    
    mods_[FEEDBACK]->setBaseValue(feedbackLevel);
    
    feedback_ = feedbackLevel;
}

double Delay::getFeedback() const
{
    return feedback_;
}

void Delay::setDecayTime(double decayTime)
{
    if (decayTime < 0 || decayTime > delayCapacity_)
    { throw std::invalid_argument("Decay time must be greater 0 and less than capacity!"); }
    
    mods_[DECAY_TIME]->setBaseValue(decayTime);
    
    decayTime_ = decayTime * Global::samplerate;
    
    calcDecay_(decayRate_,decayTime_,delayLen_);
}

double Delay::getDecayTime() const
{
    // return seconds, not samples
    return decayTime_ / Global::samplerate;
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
    double* ret = write_ - offset;
    
    if (ret < buffer_)
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
    if (read < buffer_)
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
    if (--read < buffer_)
    { read = end_; }
    
    // And finally add the fractional part of the
    // previous sample
    output += (*read - output) * readFract_;
    
    double feedback = feedback_;
    
    // Modulate feedback value
    if (mods_[FEEDBACK]->inUse())
    {
        feedback = mods_[FEEDBACK]->tick();
    }

    // If the sample hasn't been written yet, write it now
    if (readInt_ > 0)
    {
        *write_ = sample + (output * feedback);
        
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

Delay::~Delay()
{
    delete [] buffer_;
}

double AllPassDelay::process(double sample)
{
    iterator read = write_ - readInt_;
    
    // Check if we need to wrap around
    if (read < buffer_)
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
    if (--read < buffer_)
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