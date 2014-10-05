//
//  Envelope.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 09/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Envelope.h"
#include "Global.h"
#include "ModDock.h"

Envelope::Envelope(unsigned int delayMillis, bool sustainEnabled)

: ModEnvSegSeq(7,1), sustainEnabled_(sustainEnabled)

{
    currSeg_ = segs_.begin() + DEL;
    
    // Set delay length
    currSeg_->setLen(delayMillis * 44.1);
    
    currSegNum_ = DEL;
    
    // Set the hidden connector length to a 40th of the
    // samplerate, at 44.1 Khz that's 1102 samples. Not
    // too long to be noticed too much but just enough
    // to prevent transitions between loops from being
    // too abrupt
    segs_[CONNECTOR].setLen(Global::samplerate / 40);
    
    // Initial settings
    setSegLevel(Envelope::ATK, 0.8);
    setSegLen(Envelope::ATK, 500);
    
    setSegLevel(Envelope::SEG_A, 0.5);
    setSegLen(Envelope::SEG_A, 500);
    setSegRate(Envelope::SEG_A, 0.1);
    
    setSegRate(Envelope::REL, 0.1);
    setSegLen(Envelope::REL, 500);

    _mods[AMP]->setHigherBoundary(1);
    _mods[AMP]->setLowerBoundary(0);
    _mods[AMP]->setBaseValue(1);
}

void Envelope::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    ModUnit::setAmp(amp);
    
    // Sets new base value for modulation
    _mods[AMP]->setBaseValue(amp);
}

void Envelope::changeSeg_(segItr seg)
{
    EnvSegSeq::changeSeg_(seg);
    
    currSegNum_ = seg - segs_.begin();

    // If we just changed to the release segment
    // set the start level of the segment to the
    // last ticked value, (for note-off, could be
    // abrupt)
    if (currSegNum_ == REL)
    {
        currSeg_->setStartLevel(lastTick_);
    }
    
}

void Envelope::setDelay(unsigned int millis)
{
    // Convert samples to milliseconds
    segs_[DEL].setLen(44.1 * millis);
}

unsigned int Envelope::getDelay() const
{
    return segs_[DEL].getLen() / 44.1;
}

void Envelope::resetLoop_()
{
    // Reset all segments
    for(segItr itr = loopStart_, end = loopEnd_ + 1;
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    // no need for connecting segment if the amplitude
    // is the same e.g. for the constant sustain
    if (lastTick_ == loopStart_->getStartLevel())
    {
        changeSeg_(loopStart_);
    }
    
    else
    {
        // Set currSeg_ to connector segment
        changeSeg_(segs_.begin());
        
        currSeg_->reset();
    }
}

double Envelope::tick_()
{

    if (currSample_++ >= currSeg_->getLen())
    {
        // If we're crossing over the release segment, just return 0
        if (currSegNum_ == REL)
        {
            return 0;
        }
        
        // If this is the end of the connecting segment, change back to the
        // loop start
        else if (currSegNum_ == CONNECTOR)
        {
            changeSeg_(loopStart_);
        }
        
        // If we reached the loop end segment and there are more loops to go
        // through, reset the loop
        else if (currSeg_ == loopEnd_ && ( loopInf_ || loopCount_++ < loopMax_))
        {
           resetLoop_();
        }
        
        // if we reached the end of segment c and the sustain is set to infinity
        // return the last tick
        else if (currSegNum_ == SEG_C && sustainEnabled_)
        {
           return lastTick_;
        }
        
        else
        {
            changeSeg_(++currSeg_);
            
            if ( ! currSeg_->getLen() ) return lastTick_;
        }

    }
    
    if (currSegNum_ == DEL) return 0;
    
    lastTick_ = currSeg_->tick();
    
    return lastTick_;
}

double Envelope::modulate(double sample, double depth, double)
{
    double ret = sample * tick_() * depth;
    
    double amp = _amp;
    
    if (_mods[AMP]->inUse())
    {
        amp = _mods[AMP]->tick();
    }
    
    return ret * amp;
}

void Envelope::setLoopStart(seg_t seg)
{
    
    if (seg >= REL || seg <= ATK)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments A to D!"); }
    
    loopStart_ = segs_.begin() + seg;
    
    segs_[CONNECTOR].setEndLevel(loopStart_->getStartLevel());

    if (loopStart_ > loopEnd_)
    { loopEnd_ = loopStart_; }
}

void Envelope::setLoopEnd(seg_t seg)
{
    if (seg >= REL || seg < ATK)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments ATK to SEG_C!"); }
    
    loopEnd_ = segs_.begin() + seg;
    
    segs_[CONNECTOR].setStartLevel(loopEnd_->getEndLevel());
    
    if (loopEnd_ < loopStart_)
    { loopStart_ = loopEnd_; }
}

void Envelope::setSegLevel(seg_t seg, double lv)
{
    if (seg >= REL || seg < ATK)
    { throw std::invalid_argument("Invalid segment for setting the level!"); }
    
    // Set new levels
    EnvSegSeq::setLinkedLevel(seg, lv);
}

void Envelope::noteOff()
{
    if (currSegNum_ != REL)
    {
        // Switch to release segment
        changeSeg_(segs_.begin() + REL);
    }
}

void Envelope::setSustainEnabled(bool sustainEnabled)
{
    sustainEnabled_ = sustainEnabled;
}

bool Envelope::sustainEnabled() const
{
    return sustainEnabled_;
}