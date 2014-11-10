#include "Envelope.h"
#include "Global.h"
#include "ModDock.h"

Envelope::Envelope(bool sustainEnabled)

: ModEnvSegSeqFlexible(7,1), sustainEnabled_(sustainEnabled),
  lastTick_(0)

{
    currSeg_ = segs_.begin() + DEL;
    
    // Set the hidden connector length to a 40th of the
    // samplerate, at 44.1 Khz that's 1102 samples. Not
    // too long to be noticed too much but just enough
    // to prevent transitions between loops from being
    // too abrupt
    segs_[CONNECTOR].setLen(Global::samplerate / 40.0);
    
    // Initial settings
    setSegLevel(ATK, 0.8);
    setSegLen(ATK, 500);
    setSegRate(ATK, 1.2);
    
    setSegLevel(SEG_A, 0.5);
    setSegLen(SEG_A, 500);
    setSegRate(SEG_A, 1.2);
    
    setSegStartLevel(REL, 0.5);
    setSegRate(REL, 0.6);
    setSegLen(REL, 500);

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

void Envelope::changeSeg_(segItr seg)
{
    currSeg_ = seg;
    
    currSample_ = 0;

    currSegNum_ = std::distance(segs_.begin(), seg);
    
    // If we just changed to the release segment
    // set the start level of the segment to the
    // last ticked value, (because note-off could be
    // abrupt)
    if (currSegNum_ == REL)
    {
        currSeg_->setStartLevel(lastTick_);
    }
    
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
    if (loopEnd_->getEndLevel() == loopStart_->getStartLevel())
    {
        changeSeg_(loopStart_);
    }
    
    else
    {
        // Update values
        changeSeg_(segs_.begin() + CONNECTOR);
        
        // Reset the connector segment first
        currSeg_->reset();
    }
}

double Envelope::tick_()
{
    if (currSample_ >= currSeg_->getLen())
    {
        if (currSegNum_ == REL) return 0;
        
        // If this is the end of the connecting segment, change back to the
        // loop start
        else if (currSegNum_ == CONNECTOR)
        {
            changeSeg_(loopStart_);
        }
        
        // If we reached the loop end segment and there are more loops to go
        // through, reset the loop
        else if (currSeg_ == loopEnd_ && (loopInf_ || loopCount_++ < loopMax_))
        { resetLoop_(); }
        
        // if we reached the end of segment c and the sustain is set to infinity
        // return the last tick
        else if (currSegNum_ == SEG_C && sustainEnabled_)
        { return lastTick_; }
        
        else
        {
            // Change to next segment
            changeSeg_(++currSeg_);
            
            // Return last tick if the new segment has no length
            if (! currSeg_->getLen()) return lastTick_;
        }
    }
    
    lastTick_ = currSeg_->tick();
    
    return lastTick_;
}

double Envelope::modulate(double sample, double depth, double)
{
    // Modulate
    if (mods_[AMP].inUse())
    {
        amp_ = mods_[AMP].tick();
    }
    
    return sample * tick_() * depth * amp_;
}

void Envelope::setLoopStart(seg_t seg)
{
    if (seg >= REL)
    { throw std::invalid_argument("Cannot loop release segment!"); }
    
    loopStart_ = segs_.begin() + seg;
    
    segs_[CONNECTOR].setEndLevel(loopStart_->getStartLevel());

    if (loopStart_ > loopEnd_)
    { loopEnd_ = loopStart_; }
}

void Envelope::setLoopEnd(seg_t seg)
{
    if (seg >= REL)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments ATK to SEG_C!"); }
    
    loopEnd_ = segs_.begin() + seg;
    
    segs_[CONNECTOR].setStartLevel(loopEnd_->getEndLevel());
    
    if (loopEnd_ < loopStart_)
    { loopStart_ = loopEnd_; }
}

void Envelope::setSegRate(seg_t seg, double rate)
{
    if (seg < ATK || seg > REL)
    { throw std::invalid_argument("Invalid segment!"); }
    
    segs_[seg].setRate(rate);
    
    setSegModDockBaseValue(seg, EnvSeg::RATE, rate);
}

double Envelope::getSegRate(seg_t seg) const
{
    if (seg < ATK || seg > REL)
    { throw std::invalid_argument("Invalid segment!"); }
    
    if (segs_[seg].dockInUse(EnvSeg::RATE))
    {
        return getSegModDockBaseValue(seg, EnvSeg::RATE);
    }
    
    else return segs_[seg].getRate();
}

void Envelope::setSegLevel(seg_t seg, double lv)
{
    if (seg >= REL)
    { throw std::invalid_argument("Invalid segment for setting the level!"); }
    
    // Delay is always sustain so also set start level
    if (seg == DEL)
    {
        segs_[DEL].setStartLevel(lv);
    }
    
    // Set new linked levels
    EnvSegSeq::setLinkedLevel(seg, lv);
    
    setSegModDockBaseValue(seg, EnvSeg::END_LEVEL, lv);
    setSegModDockBaseValue(seg + 1, EnvSeg::START_LEVEL, lv);
}

double Envelope::getSegLevel(seg_t seg) const
{
    if (segs_[seg].dockInUse(EnvSeg::END_LEVEL))
    {
        return getSegModDockBaseValue(seg, EnvSeg::END_LEVEL);
    }
    
    else return segs_[seg].getEndLevel();
}

void Envelope::noteOff()
{
    // If we aren't already in the release segment
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

void Envelope::setModUnitDepth_Seg(seg_t segNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segs_[segNum].setModUnitDepth(EnvSeg::END_LEVEL, modNum, depth);
        
        if (segNum < segs_.size() - 1)
        {
            segs_[segNum + 1].setModUnitDepth(EnvSeg::START_LEVEL, modNum, depth);
        }
    }
    
    else
    {
        segs_[segNum].setModUnitDepth(dockNum,modNum, depth);
    }
}

double Envelope::getModUnitDepth_Seg(seg_t segNum,
                                     index_t dockNum,
                                     index_t modNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].getModUnitDepth(EnvSeg::END_LEVEL, modNum);
    }
    
    else return segs_[segNum].getModUnitDepth(dockNum, modNum);
}

void Envelope::attachMod_Seg(seg_t segNum,
                             index_t dockNum,
                             ModUnit *mod)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    // Set linked modulator if SEG_LEVEL
    if (dockNum == SEG_LEVEL)
    {
        segs_[segNum].attachMod(EnvSeg::END_LEVEL, mod);
        
        // If last segment chosen, no next segment to set start level for
        if (segNum < segs_.size() - 1)
        {
            segs_[segNum + 1].attachMod(EnvSeg::START_LEVEL, mod);
        }
    }
    
    else
    {
        segs_[segNum].attachMod(dockNum, mod);
    }
}

void Envelope::detachMod_Seg(seg_t segNum,
                             index_t dockNum,
                             index_t modNum)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segs_[segNum].detachMod(dockNum, modNum);
        
        // If last segment chosen, no next segment to set start level for
        if (segNum < segs_.size() - 1)
        {
            segs_[segNum + 1].detachMod(dockNum, modNum);
        }
    }
    
    else
    {
        segs_[segNum].detachMod(dockNum, modNum);
    }
}

void Envelope::setSidechain_Seg(seg_t segNum,
                                index_t dockNum,
                                index_t master,
                                index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segs_[segNum].setSidechain(EnvSeg::END_LEVEL, master, slave);
        
        if (segNum < segs_.size() - 1)
        {
            segs_[segNum + 1].setSidechain(EnvSeg::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        segs_[segNum].setSidechain(dockNum, master, slave);
    }
}

void Envelope::unSidechain_Seg(seg_t segNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        segs_[segNum].unSidechain(EnvSeg::END_LEVEL, master, slave);
        
        if (segNum < segs_.size() - 1)
        {
            segs_[segNum + 1].unSidechain(EnvSeg::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        segs_[segNum].unSidechain(dockNum, master, slave);
    }
}

bool Envelope::isSidechain_Seg(seg_t segNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        // Only need this segment here, segNum + 1 is the same for START_LEVEL
        return segs_[segNum].isSidechain(EnvSeg::END_LEVEL, master, slave);
    }
    
    else return segs_[segNum].isSidechain(dockNum, master, slave);
}

bool Envelope::isMaster_Seg(seg_t segNum,
                            index_t dockNum,
                            index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].isMaster(EnvSeg::END_LEVEL, index);
    }
    
    else return segs_[segNum].isMaster(dockNum, index);
}

bool Envelope::isSlave_Seg(seg_t segNum,
                           index_t dockNum,
                           index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].isSlave(EnvSeg::END_LEVEL, index);
    }
    
    else return segs_[segNum].isSlave(dockNum, index);
}

unsigned long Envelope::dockSize_Seg(seg_t segNum, index_t dockNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].dockSize(EnvSeg::END_LEVEL);
    }
    
    else return segs_[segNum].dockSize(dockNum);
}