
//
//  LFO.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "LFO.h"
#include "EnvSeg.h"
#include "Crossfader.h"
#include "Oscillator.h"
#include "ModDock.h"
#include "Global.h"

#include <stdexcept>

LFO::LFO(short wt, double rate, double amp, double phaseOffset)
: Oscillator(wt,rate,amp,phaseOffset), ModUnit(3,amp)
{
    mods_[FREQ]->setHigherBoundary(100);
    mods_[FREQ]->setLowerBoundary(0);
    mods_[FREQ]->setBaseValue(rate);
    
    mods_[PHASE]->setHigherBoundary(360);
    mods_[PHASE]->setLowerBoundary(0);
    mods_[PHASE]->setBaseValue(phaseOffset);
    
    mods_[AMP]->setHigherBoundary(1);
    mods_[AMP]->setLowerBoundary(0);
    mods_[AMP]->setBaseValue(amp);
}

void LFO::setFreq(double Hz)
{
    Oscillator::setFreq(Hz);
    
    if (mods_[FREQ]->inUse())
    {
        mods_[FREQ]->setBaseValue(Hz);
    }
}

double LFO::getFreq() const
{
    if (mods_[FREQ]->inUse())
    {
        return mods_[FREQ]->getBaseValue();
    }
    
    else return freq_;
}

void LFO::setPhaseOffset(double degrees)
{
    Oscillator::setPhaseOffset(degrees);
    
    if (mods_[PHASE]->inUse())
    {
        mods_[PHASE]->setBaseValue(degrees);
    }
}

double LFO::getPhaseOffset() const
{
    if (mods_[PHASE]->inUse())
    {
        return mods_[PHASE]->getBaseValue();
    }
    
    else return Oscillator::getPhaseOffset();
}

void LFO::setAmp(double amp)
{
    // For boundary checking
    Oscillator::setAmp(amp);
    
    if (mods_[AMP]->inUse())
    {
        mods_[AMP]->setBaseValue(amp);
    }
}

double LFO::getAmp() const
{
    if (mods_[AMP]->inUse())
    {
        return mods_[AMP]->getBaseValue();
    }
    
    else return amp_;
}

double LFO::modulate(double sample, double depth, double maximum)
{
    // Modulate rate/frequency
    if (mods_[FREQ]->inUse())
    {
        Oscillator::setFreq(mods_[FREQ]->tick());
    }
    
    // Modulate phase offset
    if (mods_[PHASE]->inUse())
    {
        Oscillator::setPhaseOffset(mods_[PHASE]->tick());
    }
    
    // Get amplitude modulation
    if (mods_[AMP]->inUse())
    {
        amp_ = mods_[AMP]->tick();
    }

    // Actual modulation by LFO
    sample += (maximum * Oscillator::tick() * depth);

    return sample * amp_;
}

LFOSeq::LFOSeq(unsigned short seqLength, double rate)
: ModEnvSegSeq(seqLength,1), lfos_(seqLength)
{
    setLoopStart(0);
    setLoopEnd(seqLength - 1);
    
    setLoopInf();
    
    setRate(rate);
    
    // Init segs
    for (int i = 0; i < seqLength; ++i)
    {
        // Attach lfos to segment
        segs_[i].attachMod(EnvSeg::START_LEVEL, &lfos_[i].lfo);
        segs_[i].attachMod(EnvSeg::END_LEVEL, &lfos_[i].lfo);
        
        setSegBothLevels(i, 1);
        
        setScaledModFreq_(i);
    }
    
    mods_[RATE]->setHigherBoundary(10);
    mods_[RATE]->setLowerBoundary(0);
    mods_[RATE]->setBaseValue(rate);
}

double LFOSeq::getScaledModFreqValue(double freq) const
{
    // Since the rate is the cycles per segment
    // and not cycles per second, we get the
    // "period" of the segment and multiply that
    // by the rate, giving the mod wave's frequency.
    
    if (! segLen_) return 0;
    
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    double temp = Global::samplerate / static_cast<double>(segLen_);
    
    // Multiply by wanted frequency
    return freq * temp;
}

void LFOSeq::setScaledModFreq_(seg_t seg)
{
    // Set scaled frequency to frequency of lfo
    lfos_[seg].lfo.setFreq(getScaledModFreqValue(lfos_[seg].freq));
}

void LFOSeq::resizeSegsFromRate_(double rate)
{
    // get the period, divide up into _segNum pieces
    segLen_ = (Global::samplerate / rate) / segs_.size();
    
    // Set all segments' lengths
    for (int i = 0; i < segs_.size(); i++)
    {
        segs_[i].setLen(segLen_);
        
        // Scale frequency of mods according to length
        setScaledModFreq_(i);
    }
}

unsigned long LFOSeq::getSegLen() const
{
    return segLen_;
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0 || Hz > 10)
    { throw std::invalid_argument("Rate cannot be less than zero or greater 10!"); }
    
    rate_ = Hz;
    
    if (mods_[RATE]->inUse())
    {
        mods_[RATE]->setBaseValue(rate_);
    }
    
    resizeSegsFromRate_(rate_);
}

double LFOSeq::getRate() const
{
    if (mods_[RATE]->inUse())
    {
        return mods_[RATE]->getBaseValue();
    }
    
    else return rate_;
}

void LFOSeq::setSegRate(seg_t seg, double rate)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    segs_[seg].setRate(rate);
}

void LFOSeq::setModWavetable(seg_t seg, short wt)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    lfos_[seg].lfo.setWavetable(wt);
}

short LFOSeq::getModWavetableID(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].lfo.getWavetableID();
}

void LFOSeq::setModDepth(seg_t seg, double depth)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    segs_[seg].setModUnitDepth(EnvSeg::START_LEVEL, 0, depth);
    segs_[seg].setModUnitDepth(EnvSeg::END_LEVEL, 0, depth);
}

double LFOSeq::getModDepth(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return segs_[seg].getModUnitDepth(EnvSeg::END_LEVEL, 0);
    return segs_[seg].getModUnitDepth(EnvSeg::END_LEVEL, 0);
}

void LFOSeq::setModFreq(seg_t seg, double freq)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    lfos_[seg].freq = freq;
    
    setScaledModFreq_(seg);
}

double LFOSeq::getModFreq(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].freq;
}

void LFOSeq::setModPhaseOffset(seg_t seg, double degrees)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    lfos_[seg].lfo.setPhaseOffset(degrees);
}

double LFOSeq::getModPhaseOffset(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].lfo.getPhaseOffset();
}

void LFOSeq::insertSegment(seg_t pos, const EnvSeg& seg)
{
    if (pos > segs_.size())
    { throw std::invalid_argument("Invalid insertion position!"); }
    
    // pushing back a segment is faster and simpler than insertion
    else if (pos == segs_.size() && segs_.size() + 1 < segs_.capacity())
    { addSegment(); }
    
    // Store distances because the iterators will be invalidated
    seg_t start = std::distance(segs_.begin(), loopStart_);
    seg_t end = std::distance(segs_.begin(), loopEnd_);
    
    // Positions that are greater than the insertion point have to
    // increment together with the segment they refer to
    if (end > pos) ++end;
    
    if (start > pos) ++start;
    
    // Note the greater or equal
    if (currSegNum_ >= pos) ++currSegNum_;
    
    segs_.insert(segs_.begin() + pos, seg);
    
    // Set iterators to valid items now
    loopEnd_ = segs_.begin() + end;
    loopStart_ = segs_.begin() + start;
    
    currSeg_ = segs_.begin() + currSegNum_;
    
    resizeSegsFromRate_(rate_);
    
    lfos_.insert(lfos_.begin() + pos, LFOSeq_LFO());
}

void LFOSeq::insertSegment(seg_t pos)
{
    insertSegment(pos, EnvSeg());
}

void LFOSeq::addSegment()
{
    // If there will be a re-allocation we need to update
    // iterators whicc is done in insertSegment()
    if (segs_.size() + 1 == segs_.capacity())
    { insertSegment(segs_.size() - 1); }
    
    else
    {
        segs_.push_back(EnvSeg());
        
        resizeSegsFromRate_(rate_);
        
        lfos_.push_back(LFOSeq_LFO());
    }
}

void LFOSeq::removeLastSegment()
{
    removeSegment(segs_.size() - 1);
}

void LFOSeq::removeSegment(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    // Store distances because the iterators will be invalidated
    seg_t start = std::distance(segs_.begin(), loopStart_);
    seg_t end = std::distance(segs_.begin(), loopEnd_);
    
    if (end > seg) --end;
    
    if (start > seg) --start;
    
    if (currSegNum_ > seg) --currSegNum_;
    
    segs_.erase(segs_.begin() + seg);
    
    // Set iterators to valid items now
    loopEnd_ = segs_.begin() + end;
    loopStart_ = segs_.begin() + start;
    
    currSeg_ = segs_.begin() + currSegNum_;
    
    if (seg == currSegNum_)
    {
        // Change segment to new current one
        changeSeg_(currSeg_);
    }
    
    resizeSegsFromRate_(rate_);
    
    lfos_.erase(lfos_.begin() + seg);
}

void LFOSeq::setModUnitDepth_Seg(seg_t segNum,
                                index_t dockNum,
                                index_t modNum,
                                double depth)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // The depth ModDock is not part of the LFO but of the segment
        // itself because the depth is the depth of the LFO in the
        // EnvSeg's Moddock. The depth is modulated by having two
        // other ModUnits sidechain the internal LFO, so the LFO's
        // depth ModDock is actually just an extension of the EnvSeg's
        // ModDock. modNum + 1 because the LFO is at index 0
        segs_[segNum].setModUnitDepth(EnvSeg::END_LEVEL, modNum + 1, depth);
    }
    
    // All other parameters belong to the lfo (phase and freq)
    else lfos_[segNum].lfo.setModUnitDepth(dockNum,modNum, depth);
}

double LFOSeq::getModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        
        return segs_[segNum].getModUnitDepth(EnvSeg::END_LEVEL, modNum + 1);
    }
    
    // All other parameters belong to the lfo (phase + freq)
    else return lfos_[segNum].lfo.getModUnitDepth(dockNum, modNum);
}

void LFOSeq::attachMod_Seg(seg_t segNum,
                                 index_t dockNum,
                                 ModUnit *mod)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Attach the depth ModUnit to the EnvSeg's dock
        // because they sidechain the internal LFO's depth
        segs_[segNum].attachMod(EnvSeg::END_LEVEL, mod);
        
        // Make the latest addition side chain the internal LFO's depth
        segs_[segNum].setSidechain(EnvSeg::END_LEVEL, segs_[segNum].dockSize(EnvSeg::END_LEVEL) - 1, 0);
    }
    
    else lfos_[segNum].lfo.attachMod(dockNum, mod);
}

void LFOSeq::detachMod_Seg(seg_t segNum,
                           index_t dockNum,
                           index_t modNum)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        segs_[segNum].detachMod(EnvSeg::END_LEVEL, modNum);
    }
    
    else lfos_[segNum].lfo.detachMod(dockNum, modNum);
}

void LFOSeq::setSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master from the internal LFO
        segs_[segNum].unSidechain(EnvSeg::END_LEVEL, master + 1, 0);
        
        // Make it sidechain another ModUnit. + 1 because internal LFO is at
        // index 0
        segs_[segNum].setSidechain(EnvSeg::END_LEVEL, master + 1, slave + 1);
    }
    
    else lfos_[segNum].lfo.setSidechain(dockNum, master, slave);
}

void LFOSeq::unSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master and slave
        segs_[segNum].unSidechain(EnvSeg::END_LEVEL, master + 1, slave + 1);
        
        // Make the master sidechain the internal LFO again
        segs_[segNum].setSidechain(EnvSeg::END_LEVEL, master + 1, 0);
    }
    
    else lfos_[segNum].lfo.unSidechain(dockNum, master, slave);
}

bool LFOSeq::isSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // + 1 because internal LFO is at index 0
        return segs_[segNum].isSidechain(EnvSeg::END_LEVEL, master + 1, slave + 1);
    }
    
    else return lfos_[segNum].lfo.isSidechain(dockNum, master, slave);
}

bool LFOSeq::isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segs_[segNum].isMaster(EnvSeg::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].lfo.isMaster(dockNum, index);
}

bool LFOSeq::isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segs_[segNum].isSlave(EnvSeg::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].lfo.isSlave(dockNum, index);
}

unsigned long LFOSeq::dockSize_Seg(seg_t segNum, index_t dockNum) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // - 1 because internal LFO is part of it
        return segs_[segNum].dockSize(EnvSeg::END_LEVEL) - 1;
    }
    
    else return lfos_[segNum].lfo.dockSize(dockNum);
}

void LFOSeq::increment()
{
    ModEnvSegSeq::increment();
    
    for (std::vector<LFOSeq_LFO>::iterator itr = lfos_.begin(), end = lfos_.end();
         itr != end;
         ++itr)
    {
        itr->lfo.increment();
    }
}

double LFOSeq::modulate(double sample, double depth, double)
{
    if (mods_[RATE]->inUse())
    {
        resizeSegsFromRate_(mods_[RATE]->tick());
    }
    
    return sample * tick() * depth * amp_;
}

LFOUnit::LFOUnit(unsigned short mode)
{
    setMode(mode);
}

void LFOUnit::setMode(bool mode)
{
    _mode = mode;
    
    if (_mode)
    {
        fader.setLeftUnit(&lfoSeqs[0]);
        
        fader.setRightUnit(&lfoSeqs[1]);
    }
    
    else
    {
        fader.setLeftUnit(&lfos[0]);
        
        fader.setRightUnit(&lfos[1]);
    }
}

bool LFOUnit::getMode() const
{
    return _mode;
}

LFOUnit::Env::Env()
: EnvSegSeq(2)
{
    //setSegLen(0, 500);
    setEnvLevel(MID, 1);
}

void LFOUnit::Env::setEnvLevel(short point, double lvl)
{
    switch (point)
    {
        case BEG:
            setSegStartLevel(SEG_A, lvl);
            break;
            
        case MID:
            setSegEndLevel(SEG_A, lvl);
            setSegStartLevel(SEG_B, lvl);
            break;
            
        case END:
            setSegEndLevel(SEG_B, lvl);
            break;
            
        default:
            break;
    }
}

double LFOUnit::modulate(double sample, double depth, double maximum)
{
    // Tick the crossfaded value from the lfos and multiply by the envelope
    // value and the total amplitude value
    return fader.modulate(sample, depth, maximum) * env.tick() * amp_;
}
