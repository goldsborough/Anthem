/********************************************************************************************//*!
*
*  @file        EnvSeg.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "EnvSeg.hpp"
#include "Global.hpp"
#include "Wavetable.hpp"
#include "ModDock.hpp"

#include <cmath>

EnvSeg::EnvSeg(double startLevel,
               double endLevel,
               len_t len,
               double rate)

: startLevel_(startLevel), endLevel_(endLevel), rate_(rate),
  curr_(0),len_(len), GenUnit(3) // three ModDocks

{
    mods_[RATE].setHigherBoundary(2);
    mods_[RATE].setLowerBoundary(0);
    mods_[RATE].setBaseValue(rate);
    
    mods_[START_LEVEL].setHigherBoundary(1);
    mods_[START_LEVEL].setLowerBoundary(0);
    mods_[START_LEVEL].setBaseValue(startLevel);
    
    mods_[END_LEVEL].setHigherBoundary(1);
    mods_[END_LEVEL].setLowerBoundary(0);
    mods_[END_LEVEL].setBaseValue(endLevel);
    
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
    range_ = endLevel_ - startLevel_;
}

void EnvSeg::calcIncr_()
{
    incr_ = (len_) ? 1.0/len_ : 0;
}

void EnvSeg::update()
{
    // Increment curr_
    curr_ += incr_;
}

double EnvSeg::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // just return the end amplitude
    
    if (curr_ >= 1 || ! len_) return endLevel_;
    
    // Modulate members
    // All together so we only call calcRange_ once
    if (mods_[RATE].inUse()        ||
        mods_[START_LEVEL].inUse() ||
        mods_[END_LEVEL].inUse())
    {
        if (mods_[RATE].inUse())
        {
            rate_ = mods_[RATE].tick();
        }
        
        if (mods_[START_LEVEL].inUse())
        {
            startLevel_ = mods_[START_LEVEL].tick();
        }
        
        if (mods_[END_LEVEL].inUse())
        {
            endLevel_ = mods_[END_LEVEL].tick();
        }
        
        calcRange_();
    }
    
    return range_ * pow(curr_,rate_) + startLevel_;
}

void EnvSeg::setLen(EnvSeg::len_t sampleLen)
{
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
    
    startLevel_ = lv;
    
    mods_[START_LEVEL].setBaseValue(startLevel_);
    
    calcRange_();
}

double EnvSeg::getStartLevel() const
{
    if (mods_[START_LEVEL].inUse())
    {
        return mods_[START_LEVEL].getBaseValue();
    }
    
    else return startLevel_;
}

void EnvSeg::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    endLevel_ = lv;
    
    mods_[END_LEVEL].setBaseValue(endLevel_);
    
    calcRange_();
}

double EnvSeg::getEndLevel() const
{
    if (mods_[END_LEVEL].inUse())
    {
        return mods_[END_LEVEL].getBaseValue();
    }
    
    else return endLevel_;
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
    
    mods_[RATE].setBaseValue(rate_);
}

double EnvSeg::getRate() const
{
    if (mods_[RATE].inUse())
    {
        return mods_[RATE].getBaseValue();
    }
    
    else return rate_;
}

EnvSegSeq::EnvSegSeq(EnvSegSeq::seg_t seqLen)
: segs_(seqLen), currSample_(0), loopCount_(0),
  loopMax_(0), loopInf_(false), currSegNum_(0)
{
    loopStart_ = loopEnd_ = segs_.end();
    
    currSeg_ = segs_.begin();
}

EnvSegSeq::EnvSegSeq(const EnvSegSeq& other)
: segs_(other.segs_), currSample_(other.currSample_),
  currSegNum_(other.currSegNum_), loopCount_(other.loopCount_),
  loopMax_(other.loopMax_), loopInf_(other.loopInf_)
{
    currSeg_ = segs_.begin() + currSegNum_;
    
    std::vector<EnvSeg>::const_iterator itr = other.loopStart_;
    
    loopStart_ = segs_.begin() + std::distance(other.segs_.begin(), itr);
    
    itr = other.loopEnd_;
    
    loopEnd_ = segs_.begin() + std::distance(other.segs_.begin(), itr);
}

EnvSegSeq& EnvSegSeq::operator= (const EnvSegSeq& other)
{
    if (this != &other)
    {
        segs_ = other.segs_;
        
        currSample_ = other.currSample_;
        
        currSegNum_ = other.currSegNum_;
        
        loopCount_ = other.loopCount_;
        
        loopMax_ = other.loopMax_;
        
        loopInf_ = other.loopInf_;
        
        currSeg_ = segs_.begin() + currSegNum_;
        
        std::vector<EnvSeg>::const_iterator itr = other.loopStart_;
        
        loopStart_ = segs_.begin() + std::distance(other.segs_.begin(), itr);
        
        itr = other.loopEnd_;
        
        loopEnd_ = segs_.begin() + std::distance(other.segs_.begin(), itr);
    }
    
    return *this;
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

void EnvSegSeq::setSegBothLevels(seg_t seg, double lv)
{
    setSegStartLevel(seg, lv);
    setSegEndLevel(seg, lv);
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

void EnvSegSeq::changeSeg_(EnvSegSeq::segItr seg)
{
    currSegNum_ = std::distance(segs_.begin(), seg);
    
    currSample_ = 0;
}

void EnvSegSeq::resetLoop_()
{
    // reset all the loop segments
    for (segItr itr = loopStart_; itr != loopEnd_; ++itr)
    { itr->reset(); }
    
    // Set current segment to loop start
    currSeg_ = loopStart_;
    
    // Change currSegNum_ and reset currSample
    changeSeg_(loopStart_);
    
    ++loopCount_;
}

void EnvSegSeq::update()
{
    currSample_++;
    currSeg_->update();
}

double EnvSegSeq::tick()
{
    if (currSample_ >= currSeg_->getLen())
    {
        // Increment segment
        currSeg_++;
        
        // Check if we need to reset the loop
        if (currSeg_ == loopEnd_ && (loopInf_ || loopCount_ < loopMax_))
        { resetLoop_(); }
        
        // If we've reached the end, go back to last segment
        // which will continue to tick its end amplitude value
        else if (currSeg_ == segs_.end()) currSeg_--;
        
        // else change
        else changeSeg_(currSeg_);
    }

    return currSeg_->tick();
}

ModEnvSegSeq::ModEnvSegSeq(seg_t numSegs,
                           seg_t numDocks,
                           double masterAmp)
: EnvSegSeq(numSegs),
  ModUnit(numDocks + 2,masterAmp)
{ }

std::vector<ModDock*>::size_type ModEnvSegSeq::numDocks_Seg(seg_t segNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].numDocks();
}

void ModEnvSegSeq::setModUnitDepth_Seg(seg_t segNum,
                                       index_t dockNum,
                                       index_t modNum,
                                       double depth)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    segs_[segNum].setModUnitDepth(dockNum,modNum, depth);
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
    
    segs_[segNum].attachMod(dockNum, mod);
}

void ModEnvSegSeq::detachMod_Seg(seg_t segNum,
                                 index_t dockNum,
                                 index_t modNum)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }

    segs_[segNum].detachMod(dockNum, modNum);
}

bool ModEnvSegSeq::dockInUse_Seg(seg_t segNum, index_t dockNum) const
{
    return segs_[segNum].dockInUse(dockNum);
}

void ModEnvSegSeq::setSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    segs_[segNum].setSidechain(dockNum, master, slave);
}

void ModEnvSegSeq::unSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    segs_[segNum].unSidechain(dockNum, master, slave);
}

bool ModEnvSegSeq::isSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].isSidechain(dockNum, master, slave);
}

bool ModEnvSegSeq::isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].isMaster(dockNum, index);
}

bool ModEnvSegSeq::isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].isSlave(dockNum, index);
}

unsigned long ModEnvSegSeq::dockSize_Seg(seg_t segNum, index_t dockNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    return segs_[segNum].dockSize(dockNum);
}

void ModEnvSegSeq::setSegModDockBaseValue(seg_t segNum, index_t dockNum, double value)
{
    segs_[segNum].mods_[dockNum].setBaseValue(value);
}

double ModEnvSegSeq::getSegModDockBaseValue(seg_t segNum, index_t dockNum) const
{
    return segs_[segNum].mods_[dockNum].getBaseValue();
}

ModEnvSegSeqFlexible::ModEnvSegSeqFlexible(seg_t numSegs,
                                           seg_t numDocks,
                                           double masterAmp)
: ModEnvSegSeq(numSegs,numDocks,masterAmp)
{ }

void ModEnvSegSeqFlexible::setSegLen(seg_t seg, unsigned long ms)
{
    segs_[seg].setLen(ms * (Global::samplerate/1000.0));
}

unsigned long ModEnvSegSeqFlexible::getSegLen(seg_t seg) const
{
    return segs_[seg].getLen();
}