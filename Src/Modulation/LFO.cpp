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
: _osc(new Oscillator(wt,rate,amp,phaseOffset)), ModUnit(3,amp)
{
    _mods[RATE]->setHigherBoundary(Global::nyquistLimit);
    _mods[RATE]->setHigherBoundary(0);
    _mods[RATE]->setBaseValue(rate);
    
    _mods[PHASE]->setHigherBoundary(360);
    _mods[PHASE]->setLowerBoundary(0);
    _mods[PHASE]->setBaseValue(phaseOffset);
    
    _mods[AMP]->setHigherBoundary(1);
    _mods[AMP]->setLowerBoundary(0);
    _mods[AMP]->setBaseValue(amp);
}

LFO::~LFO()
{ delete _osc; }

void LFO::setWavetable(short wt)
{
    _osc->setWavetable(wt);
}

void LFO::setPhaseOffset(double degrees)
{
    _mods[PHASE]->setBaseValue(degrees);
    
    _osc->setPhaseOffset(degrees);
    
    _phaseOffset = degrees;
}

double LFO::getPhaseOffset() const
{
    return _phaseOffset;
}

void LFO::setRate(double Hz)
{
    _mods[RATE]->setBaseValue(Hz);
    
    _osc->setFreq(Hz);
    
    _rate = Hz;
}

double LFO::getRate() const
{
    return _rate;
}

void LFO::setAmp(double amp)
{
    _osc->setAmp(amp);
    
    _mods[AMP]->setBaseValue(amp);
}

double LFO::getAmp() const
{
    return _osc->getAmp();
}

double LFO::modulate(double sample, double depth, double maximum)
{
    // Modulate rate/frequency
    if (_mods[RATE]->inUse())
    { _osc->setFreq(_mods[RATE]->tick()); }
    
    // Modulate phase offset
    if (_mods[PHASE]->inUse())
    { _osc->setPhaseOffset(_mods[PHASE]->tick()); }
    
    double tick = _osc->tick();
    
    // Get amplitude modulation
    if (_mods[AMP]->inUse())
    {
        tick *= _mods[AMP]->tick();
    }
    
    // Actual modulation by LFO
    sample += (maximum * tick * depth);
    
    // Note that the LFO's amplitude is taken care of by the oscillator
    // (LFO::setAmp() controls the oscillator's amplitude)
    return sample;
}

LFOSeq::LFOSeq(unsigned short seqLength, double rate)
: EnvSegSeq(seqLength),_lfos(seqLength)
{
    EnvSegSeq::setLoopStart(0);
    EnvSegSeq::setLoopEnd(seqLength - 1);
    
    EnvSegSeq::setLoopInf();
    
    for (int i = 0; i < seqLength; ++i)
    {
        EnvSegSeq::setSegLen(i, 100);
        EnvSegSeq::setSegBothLevels(i, 1);
    }
    
    EnvSegSeq::setSegStartLevel(0, 0);
    EnvSegSeq::setSegEndLevel(seqLength - 1, 0);
    
    setRate(rate);
}

void LFOSeq::_setScaledModRate(seg_t seg, double rate)
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
        
        _lfos[seg].setRate(freq);
    }
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0)
    { throw std::invalid_argument("Rate cannot be less than zero!"); }
    
    _rate = Hz;
    
    // get the period, divide up into _segNum pieces
    double len = (1.0 / _rate) / segs_.size();
    
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
    return _rate;
}

void LFOSeq::setSegRate(seg_t seg, double rate)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    if (segs_.begin() + seg == currSeg_)
    { _currSegRate = rate; }
    
    segs_[seg].setRate(rate);
}

void LFOSeq::setModDepth(seg_t seg, double depth)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    if (segs_.begin() + seg == currSeg_)
    { _currSegModDepth = depth; }
    
    _lfos[seg].setAmp(depth);
}

double LFOSeq::getModDepth(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return _lfos[seg].getAmp();
}

void LFOSeq::setModRate(seg_t seg, double rate)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    if (segs_.begin() + seg == currSeg_)
    { _currSegModRate = rate; }
    
    _lfos[seg].setRate(rate);
}

double LFOSeq::getModRate(seg_t seg) const
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return _lfos[seg].getRate();
}

void LFOSeq::setModPhaseOffset(seg_t seg, double degrees)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    if (segs_.begin() + seg == currSeg_)
    { _currSegPhaseOffset = degrees; }
    
    _lfos[seg].setPhaseOffset(degrees);
}

double LFOSeq::getModPhaseOffset(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Segment out of range for LFOSeq!"); }
    
    return _lfos[seg].getPhaseOffset();
}

void LFOSeq::addSegment()
{
    if (segs_.size() == 10)
    { throw std::runtime_error("Segment count already 10!"); }
    
    segs_.push_back(EnvSeg());
    _lfos.push_back(LFO(-1));
}

void LFOSeq::removeSegment()
{
    if (segs_.empty())
    { throw std::runtime_error("Cannot remove segment from LFOSeq if already empty!"); }
    
    segs_.erase(segs_.end() - 1);
    _lfos.erase(_lfos.end() - 1);
}

void LFOSeq::removeSegment(seg_t seg)
{
    if (seg >= segs_.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    segs_.erase(segs_.begin() + seg);
    _lfos.erase(_lfos.begin() + seg);
}

double LFOSeq::modulate(double sample, double depth, double)
{
    return sample * EnvSegSeq::tick() * depth * _amp;
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
