/********************************************************************************************//*!
*
*  @file        EnvelopeSegment.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "EnvelopeSegment.hpp"
#include "Global.hpp"
#include "Wavetable.hpp"
#include "ModDock.hpp"

#include <cmath>

EnvelopeSegment::EnvelopeSegment(double startLevel,
               double endLevel,
               length_t len,
               double rate)

: startLevel_(startLevel),
  endLevel_(endLevel),
  rate_(rate),
  curr_(0),
  len_(len),
  GenUnit(3) // three ModDocks

{
    mods_[RATE].setHigherBoundary(10);
    mods_[RATE].setLowerBoundary(0);
    mods_[RATE].setBaseValue(rate);
    
    mods_[START_LEVEL].setHigherBoundary(1);
    mods_[START_LEVEL].setLowerBoundary(0);
    mods_[START_LEVEL].setBaseValue(startLevel);
    
    mods_[END_LEVEL].setHigherBoundary(1);
    mods_[END_LEVEL].setLowerBoundary(0);
    mods_[END_LEVEL].setBaseValue(endLevel);
    
    calculateRange_();
    calculateIncr_();
}

void EnvelopeSegment::reset()
{
    curr_ = 0;
}

void EnvelopeSegment::calculateRange_()
{
    // The range between start and end
    range_ = endLevel_ - startLevel_;
}

void EnvelopeSegment::calculateIncr_()
{
    incr_ = (len_) ? 1.0/len_ : 0;
}

void EnvelopeSegment::update()
{
    // Increment curr_
    curr_ += incr_;
}

double EnvelopeSegment::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // just return the end amplitude
    
    if (curr_ >= 1 || ! len_) return endLevel_;
    
    // Modulate members
    // All together so we only call calculateRange_ once
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
        
        calculateRange_();
    }
    
    return range_ * std::pow(curr_, rate_) + startLevel_;
}

void EnvelopeSegment::setLength(EnvelopeSegment::length_t sampleLength)
{
    len_ = sampleLength;
    
    calculateIncr_();
}

EnvelopeSegment::length_t EnvelopeSegment::getLength() const
{
    return len_;
}

void EnvelopeSegment::setStartLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    startLevel_ = lv;
    
    mods_[START_LEVEL].setBaseValue(startLevel_);
    
    calculateRange_();
}

double EnvelopeSegment::getStartLevel() const
{
    if (mods_[START_LEVEL].inUse())
    {
        return mods_[START_LEVEL].getBaseValue();
    }
    
    else return startLevel_;
}

void EnvelopeSegment::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    endLevel_ = lv;
    
    mods_[END_LEVEL].setBaseValue(endLevel_);
    
    calculateRange_();
}

double EnvelopeSegment::getEndLevel() const
{
    if (mods_[END_LEVEL].inUse())
    {
        return mods_[END_LEVEL].getBaseValue();
    }
    
    else return endLevel_;
}

void EnvelopeSegment::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
}

void EnvelopeSegment::setRate(double rate)
{
    if (rate > 10 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 10"); }
    
    rate_ = rate;
    
    mods_[RATE].setBaseValue(rate_);
}

double EnvelopeSegment::getRate() const
{
    if (mods_[RATE].inUse())
    {
        return mods_[RATE].getBaseValue();
    }
    
    else return rate_;
}

EnvelopeSegmentSequence::EnvelopeSegmentSequence(EnvelopeSegmentSequence::segment_t seqLength)
: segments_(seqLength), currSample_(0), loopCount_(0),
  loopMax_(0), loopInf_(false), currSegmentNum_(0)
{
    loopStart_ = loopEnd_ = segments_.end();
    
    currSegment_ = segments_.begin();
}

EnvelopeSegmentSequence::EnvelopeSegmentSequence(const EnvelopeSegmentSequence& other)
: segments_(other.segments_), currSample_(other.currSample_),
  currSegmentNum_(other.currSegmentNum_), loopCount_(other.loopCount_),
  loopMax_(other.loopMax_), loopInf_(other.loopInf_)
{
    currSegment_ = segments_.begin() + currSegmentNum_;
    
    std::vector<EnvelopeSegment>::const_iterator itr = other.loopStart_;
    
    loopStart_ = segments_.begin() + std::distance(other.segments_.begin(), itr);
    
    itr = other.loopEnd_;
    
    loopEnd_ = segments_.begin() + std::distance(other.segments_.begin(), itr);
}

EnvelopeSegmentSequence& EnvelopeSegmentSequence::operator= (const EnvelopeSegmentSequence& other)
{
    if (this != &other)
    {
        segments_ = other.segments_;
        
        currSample_ = other.currSample_;
        
        currSegmentNum_ = other.currSegmentNum_;
        
        loopCount_ = other.loopCount_;
        
        loopMax_ = other.loopMax_;
        
        loopInf_ = other.loopInf_;
        
        currSegment_ = segments_.begin() + currSegmentNum_;
        
        std::vector<EnvelopeSegment>::const_iterator itr = other.loopStart_;
        
        loopStart_ = segments_.begin() + std::distance(other.segments_.begin(), itr);
        
        itr = other.loopEnd_;
        
        loopEnd_ = segments_.begin() + std::distance(other.segments_.begin(), itr);
    }
    
    return *this;
}

void EnvelopeSegmentSequence::reset()
{
    for(segmentItr itr = segments_.begin(), end = segments_.end();
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    changeSegment_(segments_.begin());
}

void EnvelopeSegmentSequence::setSegmentRate(segment_t segment, double rate)
{
    segments_[segment].setRate(rate);
}

double EnvelopeSegmentSequence::getSegmentRate(segment_t segment) const
{
    return segments_[segment].getRate();
}


void EnvelopeSegmentSequence::setSegmentStartLevel(segment_t segment, double lv)
{
    segments_[segment].setStartLevel(lv);
}

double EnvelopeSegmentSequence::getSegmentStartLevel(segment_t segment) const
{
    return segments_[segment].getStartLevel();
}


void EnvelopeSegmentSequence::setSegmentEndLevel(segment_t segment, double lv)
{
    segments_[segment].setEndLevel(lv);
}

double EnvelopeSegmentSequence::getSegmentEndLevel(segment_t segment) const
{
    return segments_[segment].getEndLevel();
}

void EnvelopeSegmentSequence::setSegmentBothLevels(segment_t segment, double lv)
{
    setSegmentStartLevel(segment, lv);
    setSegmentEndLevel(segment, lv);
}


void EnvelopeSegmentSequence::setLinkedLevel(segment_t segment, double lv)
{
    segments_[segment].setEndLevel(lv);
    
    // Set starting level of adjacent segment (unless
    // this is the last segment)
    if (segment < segments_.size() - 1)
    {
        segments_[segment + 1].setStartLevel(lv);
    }
}


void EnvelopeSegmentSequence::setLoopStart(EnvelopeSegmentSequence::segment_t segment)
{
    loopStart_ = segments_.begin() + segment;
}

EnvelopeSegmentSequence::segment_t EnvelopeSegmentSequence::getLoopStart() const
{
    return loopStart_ - segments_.begin();
}


void EnvelopeSegmentSequence::setLoopEnd(EnvelopeSegmentSequence::segment_t segment)
{
    // Plus one because the end points to one after
    // the loop end segment
    loopEnd_ = segments_.begin() + segment + 1;
}

EnvelopeSegmentSequence::segment_t EnvelopeSegmentSequence::getLoopEnd() const
{
    return loopEnd_ - segments_.begin();
}


void EnvelopeSegmentSequence::setLoopMax(unsigned short n)
{
    if (n > 64) loopInf_ = true;
    else
    {
        loopInf_ = false;
        loopMax_ = n;
    }
}

unsigned short EnvelopeSegmentSequence::getLoopMax() const
{
    return loopMax_;
}


void EnvelopeSegmentSequence::setLoopInf(bool state)
{
    loopInf_ = state;
}

bool EnvelopeSegmentSequence::loopInf() const
{
    return loopInf_;
}


std::vector<EnvelopeSegment>::size_type EnvelopeSegmentSequence::size() const
{
    return segments_.size();
}

void EnvelopeSegmentSequence::changeSegment_(EnvelopeSegmentSequence::segmentItr segment)
{
    currSegmentNum_ = std::distance(segments_.begin(), segment);
    
    currSample_ = 0;
}

void EnvelopeSegmentSequence::resetLoop_()
{
    // reset all the loop segments
    for (segmentItr itr = loopStart_; itr != loopEnd_; ++itr)
    { itr->reset(); }
    
    // Set current segment to loop start
    currSegment_ = loopStart_;
    
    // Change currSegmentNum_ and reset currSample
    changeSegment_(loopStart_);
    
    ++loopCount_;
}

void EnvelopeSegmentSequence::update()
{
    currSample_++;
    currSegment_->update();
}

double EnvelopeSegmentSequence::tick()
{
    if (currSample_ >= currSegment_->getLength())
    {
        // Increment segment
        currSegment_++;
        
        // Check if we need to reset the loop
        if (currSegment_ == loopEnd_ && (loopInf_ || loopCount_ < loopMax_))
        { resetLoop_(); }
        
        // If we've reached the end, go back to last segment
        // which will continue to tick its end amplitude value
        else if (currSegment_ == segments_.end()) currSegment_--;
        
        // else change
        else changeSegment_(currSegment_);
    }

    return currSegment_->tick();
}

ModEnvelopeSegmentSequence::ModEnvelopeSegmentSequence(segment_t numSegments,
                           segment_t numDocks,
                           double masterAmp)
: EnvelopeSegmentSequence(numSegments),
  ModUnit(numDocks + 2,masterAmp)
{ }

std::vector<ModDock*>::size_type ModEnvelopeSegmentSequence::numDocks_Segment(segment_t segmentNum) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].numDocks();
}

void ModEnvelopeSegmentSequence::setModUnitDepth_Segment(segment_t segmentNum,
                                       index_t dockNum,
                                       index_t modNum,
                                       double depth)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    segments_[segmentNum].setModUnitDepth(dockNum,modNum, depth);
}

double ModEnvelopeSegmentSequence::getModUnitDepth_Segment(segment_t segmentNum, index_t dockNum, index_t modNum) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].getModUnitDepth(dockNum, modNum);
}

void ModEnvelopeSegmentSequence::attachMod_Segment(segment_t segmentNum,
                                 index_t dockNum,
                                 ModUnit *mod)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    segments_[segmentNum].attachMod(dockNum, mod);
}

void ModEnvelopeSegmentSequence::detachMod_Segment(segment_t segmentNum,
                                 index_t dockNum,
                                 index_t modNum)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }

    segments_[segmentNum].detachMod(dockNum, modNum);
}

bool ModEnvelopeSegmentSequence::dockInUse_Segment(segment_t segmentNum, index_t dockNum) const
{
    return segments_[segmentNum].dockInUse(dockNum);
}

void ModEnvelopeSegmentSequence::setSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    segments_[segmentNum].setSidechain(dockNum, master, slave);
}

void ModEnvelopeSegmentSequence::unSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave)
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    segments_[segmentNum].unSidechain(dockNum, master, slave);
}

bool ModEnvelopeSegmentSequence::isSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].isSidechain(dockNum, master, slave);
}

bool ModEnvelopeSegmentSequence::isMaster_Segment(segment_t segmentNum, index_t dockNum, index_t index) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].isMaster(dockNum, index);
}

bool ModEnvelopeSegmentSequence::isSlave_Segment(segment_t segmentNum, index_t dockNum, index_t index) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].isSlave(dockNum, index);
}

unsigned long ModEnvelopeSegmentSequence::dockSize_Segment(segment_t segmentNum, index_t dockNum) const
{
    if (segmentNum >= segments_.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return segments_[segmentNum].dockSize(dockNum);
}

void ModEnvelopeSegmentSequence::setSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum, double value)
{
    segments_[segmentNum].mods_[dockNum].setBaseValue(value);
}

double ModEnvelopeSegmentSequence::getSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum) const
{
    return segments_[segmentNum].mods_[dockNum].getBaseValue();
}

ModEnvelopeSegmentSequenceFlexible::ModEnvelopeSegmentSequenceFlexible(segment_t numSegments,
                                           segment_t numDocks,
                                           double masterAmp)
: ModEnvelopeSegmentSequence(numSegments,numDocks,masterAmp)
{ }

void ModEnvelopeSegmentSequenceFlexible::setSegmentLength(segment_t segment, unsigned long ms)
{
    segments_[segment].setLength(ms * (Global::samplerate/1000.0));
}

unsigned long ModEnvelopeSegmentSequenceFlexible::getSegmentLength(segment_t segment) const
{
    return segments_[segment].getLength();
}