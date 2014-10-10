#include "EnvSeg.h"
#include "Global.h"
#include "Wavetable.h"
#include "ModDock.h"

#include <cmath>

// 60 seconds
const unsigned long EnvSeg::maxLen_ = 2646000;

EnvSeg::EnvSeg(double startAmp,
               double endAmp,
               len_t len,
               double rate)

: startAmp_(startAmp), endAmp_(endAmp), rate_(rate),
  curr_(0),len_(len), GenUnit(3) // three ModDocks

{
    _mods[RATE]->setHigherBoundary(2);
    _mods[RATE]->setLowerBoundary(0);
    _mods[RATE]->setBaseValue(rate);
    
    _mods[START_LEVEL]->setHigherBoundary(1);
    _mods[START_LEVEL]->setLowerBoundary(0);
    _mods[START_LEVEL]->setBaseValue(startAmp);
    
    _mods[END_LEVEL]->setHigherBoundary(1);
    _mods[END_LEVEL]->setLowerBoundary(0);
    _mods[END_LEVEL]->setBaseValue(endAmp);
    
    calcRange_();
    calcIncr_();
}

void EnvSeg::reset()
{
    curr_ = 0;
}

void EnvSeg::calcRange_()
{
    // The range between start and end
    range_ = endAmp_ - startAmp_;
}

void EnvSeg::calcIncr_()
{
    incr_ = (len_) ? 1.0/len_ : 0;
}

double EnvSeg::calc_()
{
    // Calculate value
    double ret = range_ * pow(curr_,rate_) + startAmp_;
    
    // Increment curr_
    curr_ += incr_;
    
    return ret;
}

double EnvSeg::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // (which shouldn't happen because the
    // envelope should move on), just return
    // the end amplitude.
    
    if (curr_ >= 1) return endAmp_;
    
    // Modulate members
    if (_mods[RATE]->inUse()        ||
        _mods[START_LEVEL]->inUse() ||
        _mods[END_LEVEL]->inUse())
    {
        if (_mods[RATE]->inUse())
        {
            rate_ = _mods[RATE]->tick();
        }
        
        if (_mods[START_LEVEL]->inUse())
        {
            startAmp_ = _mods[START_LEVEL]->tick();
        }
        
        if (_mods[END_LEVEL]->inUse())
        {
            endAmp_ = _mods[END_LEVEL]->tick();
        }
        
        calcRange_();
    }
    
    return calc_();
}

void EnvSeg::setLen(EnvSeg::len_t sampleLen)
{
    if (sampleLen > maxLen_)
    { throw std::invalid_argument("EnvSeg length cannot exceed max length!"); }
    
    len_ = sampleLen;
    
    calcIncr_();
}

EnvSeg::len_t EnvSeg::getLen() const
{
    return len_;
}

void EnvSeg::setStartLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    startAmp_ = lv;
    
    _mods[START_LEVEL]->setBaseValue(startAmp_);
    
    calcRange_();
}

double EnvSeg::getStartLevel() const
{
    if (_mods[START_LEVEL]->inUse())
    {
        return _mods[START_LEVEL]->getBaseValue();
    }
    
    else return startAmp_;
}

void EnvSeg::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    endAmp_ = lv;
    
    _mods[END_LEVEL]->setBaseValue(endAmp_);
    
    calcRange_();
}

double EnvSeg::getEndLevel() const
{
    if (_mods[END_LEVEL]->inUse())
    {
        return _mods[END_LEVEL]->getBaseValue();
    }
    
    else return endAmp_;
}

void EnvSeg::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
}

void EnvSeg::setRate(double rate)
{
    if (rate > 10 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 10"); }
    
    rate_ = rate;
    
    _mods[RATE]->setBaseValue(rate_);
}

double EnvSeg::getRate() const
{
    if (_mods[RATE]->inUse())
    {
        return _mods[RATE]->getBaseValue();
    }
    
    else return rate_;
}

EnvSegSeq::EnvSegSeq(EnvSegSeq::seg_t seqLen)
: segs_(seqLen), currSample_(0), loopCount_(0),
  loopMax_(0), loopInf_(false), lastTick_(0)
{
    loopStart_ = loopEnd_ = segs_.end();
    
    currSeg_ = segs_.begin();
}

void EnvSegSeq::setSegRate(seg_t seg, double rate)
{
    segs_[seg].setRate(rate);
}

double EnvSegSeq::getSegRate(seg_t seg) const
{
    return segs_[seg].getRate();
}


void EnvSegSeq::setSegStartLevel(seg_t seg, double lv)
{
    segs_[seg].setStartLevel(lv);
}

double EnvSegSeq::getSegStartLevel(seg_t seg) const
{
    return segs_[seg].getStartLevel();
}


void EnvSegSeq::setSegEndLevel(seg_t seg, double lv)
{
    segs_[seg].setEndLevel(lv);
}

double EnvSegSeq::getSegEndLevel(seg_t seg) const
{
    return segs_[seg].getEndLevel();
}


void EnvSegSeq::setSegLen(seg_t seg, unsigned long ms)
{
    segs_[seg].setLen(ms * 44.1);
}

unsigned long EnvSegSeq::getSegLen(seg_t seg) const
{
    return segs_[seg].getLen();
}


void EnvSegSeq::setSegBothLevels(seg_t seg, double lv)
{
    setSegStartLevel(seg, lv), setSegEndLevel(seg, lv);
}


void EnvSegSeq::setLinkedLevel(seg_t seg, double lv)
{
    segs_[seg].setEndLevel(lv);
    
    // Set starting level of adjacent segment (unless
    // this is the last segment)
    if (seg < segs_.size() - 1)
    {
        segs_[seg + 1].setStartLevel(lv);
    }
}


void EnvSegSeq::setLoopStart(EnvSegSeq::seg_t seg)
{
    loopStart_ = segs_.begin() + seg;
}

EnvSegSeq::seg_t EnvSegSeq::getLoopStart() const
{
    return loopStart_ - segs_.begin();
}


void EnvSegSeq::setLoopEnd(EnvSegSeq::seg_t seg)
{
    // Plus one because the end points to one after
    // the loop end segment
    loopEnd_ = segs_.begin() + seg + 1;
}

EnvSegSeq::seg_t EnvSegSeq::getLoopEnd() const
{
    return loopEnd_ - segs_.begin();
}


void EnvSegSeq::setLoopMax(unsigned short n)
{
    if (n > 64) loopInf_ = true;
    else
    {
        loopInf_ = false;
        loopMax_ = n;
    }
}

unsigned short EnvSegSeq::getLoopMax() const
{
    return loopMax_;
}


void EnvSegSeq::setLoopInf(bool state)
{
    loopInf_ = state;
}

bool EnvSegSeq::loopInf() const
{
    return loopInf_;
}


std::vector<EnvSeg>::size_type EnvSegSeq::size() const
{
    return segs_.size();
}


void EnvSegSeq::addSegment()
{
    segs_.push_back(EnvSeg());
}

void EnvSegSeq::removeSegment()
{
    if (segs_.empty())
    { throw std::runtime_error("Cannot remove segment from LFOSeq if already empty!"); }
    
    segs_.erase(segs_.end() - 1);
}

void EnvSegSeq::removeSegment(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    segs_.erase(segs_.begin() + seg);
}

void EnvSegSeq::changeSeg_(EnvSegSeq::segItr seg)
{
    currSeg_ = seg;
    
    currSample_ = 0;
}

void EnvSegSeq::resetLoop_()
{
    // reset all the loop segments
    for (segItr itr = loopStart_; itr != loopEnd_; ++itr)
    { itr->reset(); }
    
    changeSeg_(loopStart_);
    
    ++loopCount_;
}

double EnvSegSeq::tick()
{
    if (currSample_++ >= currSeg_->getLen())
    {
        if (currSeg_ != segs_.end()) currSeg_++;
        
        // Check if we need to reset the loop
        if (currSeg_ == loopEnd_ && (loopInf_ || loopCount_ < loopMax_))
        { resetLoop_(); }
        
        // send out the last good value if no more segments left
        else if (currSeg_ == segs_.end()) return lastTick_;
        
        // else increment segment
        else
        {
            changeSeg_(currSeg_);
            
            // Return last tick if the new segment has no length
            if (! currSeg_->getLen()) return lastTick_;
        }
    }

    lastTick_ = currSeg_->tick();
    
    return lastTick_;
    
}

ModEnvSegSeq::ModEnvSegSeq(seg_t numSegs,
                           seg_t dockNum,
                           double amp)
: EnvSegSeq(numSegs),
  ModUnit(dockNum + 2,amp)
{ }

std::vector<ModDock*>::size_type ModEnvSegSeq::numDocks_Seg(seg_t segNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].numDocks();
}

void ModEnvSegSeq::setDockMasterDepth_Seg(seg_t segNum, index_t dockNum, double depth)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    segs_[segNum].setDockMasterDepth(dockNum, depth);
}

double ModEnvSegSeq::getDockMasterDepth_Seg(seg_t segNum, index_t dockNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].getDockMasterDepth(dockNum);
}

void ModEnvSegSeq::setModUnitDepth_Seg(seg_t segNum,
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

double ModEnvSegSeq::getModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].getModUnitDepth(dockNum, modNum);
}

void ModEnvSegSeq::attachMod_Seg(seg_t segNum,
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

void ModEnvSegSeq::detachMod_Seg(seg_t segNum,
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

void ModEnvSegSeq::setSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
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

void ModEnvSegSeq::unSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
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

bool ModEnvSegSeq::isSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave) const
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

bool ModEnvSegSeq::isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].isMaster(EnvSeg::END_LEVEL, index);
    }
    
    else return segs_[segNum].isMaster(dockNum, index);
}

bool ModEnvSegSeq::isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return segs_[segNum].isSlave(EnvSeg::END_LEVEL, index);
    }
    
    else return segs_[segNum].isSlave(dockNum, index);
}
