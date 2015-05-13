/********************************************************************************************//*!
*
*  @file        Envelope.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Envelope.hpp"
#include "Global.hpp"
#include "ModDock.hpp"

Envelope::Envelope(bool sustainEnabled)
: ModEnvelopeSegmentSequenceFlexible(7,1),
  sustainEnabled_(sustainEnabled),
  lastTick_(0)
{
    currSegment_ = segments_.begin() + Segments::DELAY;
    
    // Set the hidden connector length to a 40th of the
    // samplerate, at 44.1 Khz that's 1102 samples. Not
    // too long to be noticed too much but just enough
    // to prevent transitions between loops from being
    // too abrupt
    segments_[CONNECTOR].setLength(Global::samplerate / 40.0);
    
    // Initial settings
    /*
    setSegmentLevel(Segments::ATTACK, 0.8);
    setSegmentLength(Segments::ATTACK, 500);
    setSegmentRate(Segments::ATTACK, 1.2);
    
    setSegmentLevel(Segments::A, 0.5);
    setSegmentLength(Segments::A, 500);
    setSegmentRate(Segments::A, 1.2);
    
    setSegmentStartLevel(Segments::RELEASE, 0.5);
    setSegmentRate(Segments::RELEASE, 0.6);
    setSegmentLength(Segments::RELEASE, 500);
    */

    mods_[AMP].setHigherBoundary(1);
    mods_[AMP].setLowerBoundary(0);
    mods_[AMP].setBaseValue(1);
}

void Envelope::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    ModUnit::setAmp(amp);
    
    // Sets new base value for modulation
    mods_[AMP].setBaseValue(amp);
}

double Envelope::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    else return amp_;
}

void Envelope::changeSegment_(segmentItr segment)
{
    currSegment_ = segment;
    
    currSample_ = 0;

    currSegmentNum_ = std::distance(segments_.begin(), segment);
    
    // If we just changed to the release segmentment
    // set the start level of the segmentment to the
    // last ticked value, (because note-off could be
    // abrupt)
    if (currSegmentNum_ == Segments::RELEASE)
    {
        currSegment_->setStartLevel(lastTick_);
    }
    
}

void Envelope::resetLoop_()
{
    // Reset all segmentments
    for(segmentItr itr = loopStart_, end = loopEnd_ + 1;
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    // no need for connecting segmentment if the amplitude
    // is the same e.g. for the constant sustain
    if (loopEnd_->getEndLevel() == loopStart_->getStartLevel())
    {
        changeSegment_(loopStart_);
    }
    
    else
    {
        // Update values
        changeSegment_(segments_.begin() + CONNECTOR);
        
        // Reset the connector segmentment first
        currSegment_->reset();
    }
}

double Envelope::tick_()
{
    if (currSample_ >= currSegment_->getLength())
    {
        if (currSegmentNum_ == Segments::RELEASE) return 0;
        
        // If this is the end of the connecting segmentment,
        // change back to the loop start
        else if (currSegmentNum_ == Hidden::CONNECTOR)
        {
            changeSegment_(loopStart_);
        }
        
        // If we reached the loop end segmentment and there are more loops to go
        // through, reset the loop
        else if (currSegment_ == loopEnd_ && (loopInf_ || loopCount_++ < loopMax_))
        {
            resetLoop_();
        }
        
        // if we reached the end of segmentment c and the sustain is set to infinity
        // return the last tick
        else if (currSegmentNum_ == Segments::C && sustainEnabled_)
        {
            return lastTick_;
        }
        
        else
        {
            // Change to next segmentment
            changeSegment_(++currSegment_);
            
            // Return last tick if the new segmentment has no length
            if (! currSegment_->getLength()) return lastTick_;
        }
    }
    
    lastTick_ = currSegment_->tick();
    
    return lastTick_;
}

double Envelope::modulate(double sample, double depth, double maximum)
{
    // Modulate
    if (mods_[AMP].inUse())
    {
        amp_ = mods_[AMP].tick();
    }
    
    return sample + (maximum * tick_() * depth * amp_);
}

void Envelope::setLoopStart(segment_t segment)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Cannot loop release segmentment!"); }
    
    loopStart_ = segments_.begin() + segment;
    
    segments_[CONNECTOR].setEndLevel(loopStart_->getStartLevel());

    if (loopStart_ > loopEnd_)
    { loopEnd_ = loopStart_; }
}

void Envelope::setLoopEnd(segment_t segment)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Invalid loop segmentment, can only loop from\
                                  Segments Segments::ATTACK to Segments::C!"); }
    
    loopEnd_ = segments_.begin() + segment;
    
    segments_[CONNECTOR].setStartLevel(loopEnd_->getEndLevel());
    
    if (loopEnd_ < loopStart_)
    { loopStart_ = loopEnd_; }
}

void Envelope::setSegmentRate(segment_t segment, double rate)
{
    if (segment < Segments::ATTACK || segment > Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment!"); }
    
    segments_[segment].setRate(rate);
    
    setSegmentModDockBaseValue(segment, EnvelopeSegment::RATE, rate);
}

double Envelope::getSegmentRate(segment_t segment) const
{
    if (segment < Segments::ATTACK || segment > Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment!"); }
    
    if (segments_[segment].dockInUse(EnvelopeSegment::RATE))
    {
        return getSegmentModDockBaseValue(segment, EnvelopeSegment::RATE);
    }
    
    else return segments_[segment].getRate();
}

void Envelope::setSegmentLevel(segment_t segment, double lv)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment for setting the level!"); }
    
    // Delay is always sustain so also set start level
    if (segment == Segments::DELAY)
    {
        segments_[Segments::DELAY].setStartLevel(lv);
    }
    
    // Set new linked levels
    EnvelopeSegmentSequence::setLinkedLevel(segment, lv);
    
    setSegmentModDockBaseValue(segment, EnvelopeSegment::END_LEVEL, lv);
    setSegmentModDockBaseValue(segment + 1, EnvelopeSegment::START_LEVEL, lv);
}

double Envelope::getSegmentLevel(segment_t segment) const
{
    if (segments_[segment].dockInUse(EnvelopeSegment::END_LEVEL))
    {
        return getSegmentModDockBaseValue(segment, EnvelopeSegment::END_LEVEL);
    }
    
    else return segments_[segment].getEndLevel();
}

void Envelope::noteOff()
{
    // If we aren't already in the release segmentment
    if (currSegmentNum_ != Segments::RELEASE)
    {
        // Switch to release segmentment
        changeSegment_(segments_.begin() + Segments::RELEASE);
    }
}

void Envelope::setSustainEnabled(bool sustainEnabled)
{
    sustainEnabled_ = sustainEnabled;
}

bool Envelope::sustainIsEnabled() const
{
    return sustainEnabled_;
}

void Envelope::setModUnitDepth_Segment(segment_t segmentNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segments_[segmentNum].setModUnitDepth(EnvelopeSegment::END_LEVEL, modNum, depth);
        
        if (segmentNum < segments_.size() - 1)
        {
            segments_[segmentNum + 1].setModUnitDepth(EnvelopeSegment::START_LEVEL, modNum, depth);
        }
    }
    
    else
    {
        segments_[segmentNum].setModUnitDepth(dockNum,modNum, depth);
    }
}

double Envelope::getModUnitDepth_Segment(segment_t segmentNum,
                                     index_t dockNum,
                                     index_t modNum) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segments_[segmentNum].getModUnitDepth(EnvelopeSegment::END_LEVEL, modNum);
    }
    
    else return segments_[segmentNum].getModUnitDepth(dockNum, modNum);
}

void Envelope::attachMod_Segment(segment_t segmentNum,
                             index_t dockNum,
                             ModUnit *mod)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    // Set linked modulator if SEG_LEVEL
    if (dockNum == SEG_LEVEL)
    {
        segments_[segmentNum].attachMod(EnvelopeSegment::END_LEVEL, mod);
        
        // If last segmentment chosen, no next segmentment to set start level for
        if (segmentNum < segments_.size() - 1)
        {
            segments_[segmentNum + 1].attachMod(EnvelopeSegment::START_LEVEL, mod);
        }
    }
    
    else
    {
        segments_[segmentNum].attachMod(dockNum, mod);
    }
}

void Envelope::detachMod_Segment(segment_t segmentNum,
                             index_t dockNum,
                             index_t modNum)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segments_[segmentNum].detachMod(dockNum, modNum);
        
        // If last segmentment chosen, no next segmentment to set start level for
        if (segmentNum < segments_.size() - 1)
        {
            segments_[segmentNum + 1].detachMod(dockNum, modNum);
        }
    }
    
    else
    {
        segments_[segmentNum].detachMod(dockNum, modNum);
    }
}

void Envelope::setSidechain_Segment(segment_t segmentNum,
                                index_t dockNum,
                                index_t master,
                                index_t slave)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segments_[segmentNum].setSidechain(EnvelopeSegment::END_LEVEL, master, slave);
        
        if (segmentNum < segments_.size() - 1)
        {
            segments_[segmentNum + 1].setSidechain(EnvelopeSegment::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        segments_[segmentNum].setSidechain(dockNum, master, slave);
    }
}

void Envelope::unSidechain_Segment(segment_t segmentNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segments_[segmentNum].unSidechain(EnvelopeSegment::END_LEVEL, master, slave);
        
        if (segmentNum < segments_.size() - 1)
        {
            segments_[segmentNum + 1].unSidechain(EnvelopeSegment::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        segments_[segmentNum].unSidechain(dockNum, master, slave);
    }
}

bool Envelope::isSidechain_Segment(segment_t segmentNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        // Only need this segmentment here, segmentNum + 1 is the same for START_LEVEL
        return segments_[segmentNum].isSidechain(EnvelopeSegment::END_LEVEL, master, slave);
    }
    
    else return segments_[segmentNum].isSidechain(dockNum, master, slave);
}

bool Envelope::isMaster_Segment(segment_t segmentNum,
                            index_t dockNum,
                            index_t index) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segments_[segmentNum].isMaster(EnvelopeSegment::END_LEVEL, index);
    }
    
    else return segments_[segmentNum].isMaster(dockNum, index);
}

bool Envelope::isSlave_Segment(segment_t segmentNum,
                           index_t dockNum,
                           index_t index) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segments_[segmentNum].isSlave(EnvelopeSegment::END_LEVEL, index);
    }
    
    else return segments_[segmentNum].isSlave(dockNum, index);
}

unsigned long Envelope::dockSize_Segment(segment_t segmentNum, index_t dockNum) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segments_[segmentNum].dockSize(EnvelopeSegment::END_LEVEL);
    }
    
    else return segments_[segmentNum].dockSize(dockNum);
}