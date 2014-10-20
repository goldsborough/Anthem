
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
    _mods[FREQ]->setHigherBoundary(Global::nyquistLimit);
    _mods[FREQ]->setHigherBoundary(0);
    _mods[FREQ]->setBaseValue(rate);
    
    _mods[PHASE]->setHigherBoundary(360);
    _mods[PHASE]->setLowerBoundary(0);
    _mods[PHASE]->setBaseValue(phaseOffset);
    
    _mods[AMP]->setHigherBoundary(1);
    _mods[AMP]->setLowerBoundary(0);
    _mods[AMP]->setBaseValue(amp);
}

void LFO::setFreq(double Hz)
{
    Oscillator::setFreq(Hz);
    
    if (_mods[FREQ]->inUse())
    {
        _mods[FREQ]->setBaseValue(Hz);
    }
}

double LFO::getFreq() const
{
    if (_mods[FREQ]->inUse())
    {
        return _mods[FREQ]->getBaseValue();
    }
    
    else return freq_;
}

void LFO::setPhaseOffset(double degrees)
{
    Oscillator::setPhaseOffset(degrees);
    
    if (_mods[PHASE]->inUse())
    {
        _mods[PHASE]->setBaseValue(degrees);
    }
}

double LFO::getPhaseOffset() const
{
    if (_mods[PHASE]->inUse())
    {
        return _mods[PHASE]->getBaseValue();
    }
    
    else return Oscillator::getPhaseOffset();
}

void LFO::setAmp(double amp)
{
    // For boundary checking
    Oscillator::setAmp(amp);
    
    if (_mods[AMP]->inUse())
    {
        _mods[AMP]->setBaseValue(amp);
    }
}

double LFO::getAmp() const
{
    if (_mods[AMP]->inUse())
    {
        return _mods[AMP]->getBaseValue();
    }
    
    else return amp_;
}

double LFO::modulate(double sample, double depth, double maximum)
{
    // Modulate rate/frequency
    if (_mods[FREQ]->inUse())
    {
        Oscillator::setFreq(_mods[FREQ]->tick());
    }
    
    // Modulate phase offset
    if (_mods[PHASE]->inUse())
    {
        Oscillator::setPhaseOffset(_mods[PHASE]->tick());
    }
    
    // Get amplitude modulation
    if (_mods[AMP]->inUse())
    {
        amp_ = _mods[AMP]->tick();
    }

    // Actual modulation by LFO
    sample += (maximum * Oscillator::tick() * depth);

    return sample * amp_;
}

LFOSeq::LFOSeq(unsigned short seqLength, double rate)
: ModEnvSegSeq(seqLength), lfos_(seqLength)
{
    setLoopStart(0);
    setLoopEnd(seqLength - 1);
    
    setLoopInf();
    
    setRate(rate);
    
    // Init segs
    for (int i = 0; i < seqLength; ++i)
    {
        // Attach lfos to segment
        segs_[i].attachMod(EnvSeg::START_LEVEL, &lfos_[i]);
        segs_[i].attachMod(EnvSeg::END_LEVEL, &lfos_[i]);
        
        setSegBothLevels(i, 1);
        
        setScaledModFreq_(i, 1);
    }
}

void LFOSeq::setScaledModFreq_(seg_t seg, double rate)
{
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    
    // Since the rate is the cycles per segment
    // and not cycles per second, we get the
    // "period" of the segment and multiply that
    // by the rate, giving the mod wave's frequency.
    
    if (segs_[seg].getLen() > 0)
    {
        double freq = (Global::samplerate / static_cast<double>(segs_[seg].getLen())) * rate;
        
        // For some reason this is necessary
        //freq /= 2;
        
        lfos_[seg].setFreq(freq);
    }
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0)
    { throw std::invalid_argument("Rate cannot be less than zero!"); }
    
    rate_ = Hz;
    
    // get the period, divide up into _segNum pieces
    double len = (1.0 / rate_) / segs_.size();
    
    // seconds to milliseconds
    len *= 1000;
    
    // Set all segment's lengths
    for (int i = 0; i < segs_.size(); i++)
    {
        EnvSegSeq::setSegLen(i, len);
    }
}

double LFOSeq::getRate() const
{
    return rate_;
}

void LFOSeq::setSegLen(seg_t seg, unsigned long ms)
{
    double oldUnscaledFreq = lfos_[seg].getFreq() / (Global::samplerate/segs_[seg].getLen());
    
    segs_[seg].setLen(ms * 44.1);
    
    setScaledModFreq_(seg, oldUnscaledFreq);
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
    
    lfos_[seg].setWavetable(wt);
}

short LFOSeq::getModWavetableID(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].getWavetableID();
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
    
    setScaledModFreq_(seg, freq);
}

double LFOSeq::getModFreq(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].getFreq();
}

void LFOSeq::setModPhaseOffset(seg_t seg, double degrees)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    lfos_[seg].setPhaseOffset(degrees);
}

double LFOSeq::getModPhaseOffset(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return lfos_[seg].getPhaseOffset();
}

void LFOSeq::addSegment()
{
    ModEnvSegSeq::addSegment();
    lfos_.push_back(LFO());
}

void LFOSeq::removeLastSegment()
{
    if (segs_.empty())
    { throw std::runtime_error("Cannot remove segment from LFOSeq if already empty!"); }
    
    ModEnvSegSeq::removeLastSegment();
    lfos_.pop_back();
}

void LFOSeq::removeSegment(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    segs_.erase(segs_.begin() + seg);
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
    else lfos_[segNum].setModUnitDepth(dockNum,modNum, depth);
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
    else return lfos_[segNum].getModUnitDepth(dockNum, modNum);
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
    
    else lfos_[segNum].attachMod(dockNum, mod);
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
    
    else lfos_[segNum].detachMod(dockNum, modNum);
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
    
    else lfos_[segNum].setSidechain(dockNum, master, slave);
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
    
    else lfos_[segNum].unSidechain(dockNum, master, slave);
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
    
    else return lfos_[segNum].isSidechain(dockNum, master, slave);
}

bool LFOSeq::isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segs_[segNum].isMaster(EnvSeg::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].isMaster(dockNum, index);
}

bool LFOSeq::isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segs_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segs_[segNum].isSlave(EnvSeg::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].isSlave(dockNum, index);
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
    
    else return lfos_[segNum].dockSize(dockNum);
}

void LFOSeq::increment()
{
    ModEnvSegSeq::increment();
    
    for (std::vector<LFO>::iterator itr = lfos_.begin(), end = lfos_.end();
         itr != end;
         ++itr)
    {
        itr->increment();
    }
}

double LFOSeq::modulate(double sample, double depth, double)
{
    return sample * tick() * depth * _amp;
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
    setSegLen(0, 500);
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
    return fader.modulate(sample, depth, maximum) * env.tick() * _amp;
}
