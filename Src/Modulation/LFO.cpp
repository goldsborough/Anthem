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
: _osc(new Oscillator(wt,rate,amp,phaseOffset)), ModUnit(1,3)
{ }

LFO::~LFO()
{ delete _osc; }

void LFO::setWavetable(short wt)
{
    _osc->setWavetable(wt);
}

void LFO::setPhaseOffset(double degrees)
{
    _phaseOffset = degrees;
    
    _osc->setPhaseOffset(degrees);
}

double LFO::getPhaseOffset() const
{
    return _phaseOffset;
}

void LFO::setRate(double Hz)
{
    _rate = Hz;
    
    _osc->setFreq(Hz);
}

double LFO::getRate() const
{
    return _rate;
}

void LFO::setAmp(double amp)
{
    _osc->setAmp(amp);
}

double LFO::getAmp() const
{
    return _osc->getAmp();
}

double LFO::modulate(double sample, double depth, double minBoundary, double maxBoundary)
{
    // Set all of these modulations to the oscillator directly so that the internal
    // base values aren't changed (e.g. _rate, in this case, must stay the same, as
    // it's the base value for modulation)
    if (_mods[RATE]->inUse())
    { _osc->setFreq(_mods[RATE]->modulate(_rate, 0, Global::nyquistLimit)); }
    
    if (_mods[PHASE]->inUse())
    { _osc->setPhaseOffset(_mods[PHASE]->modulate(_phaseOffset, 0, 360)); }
    
    double tick = _osc->tick();
    
    if (tick > 0.3)
    {
        
    }
    
    if (_mods[AMP]->inUse())
    { tick *= _mods[AMP]->modulate(_osc->getAmp(), 0, 1); }
    
    // Modulate
    sample += (maxBoundary * tick * depth);
    
    // Boundary checking
    if (sample > maxBoundary) return maxBoundary;
    
    else if (sample < minBoundary) return minBoundary;
    
    return sample;
}

LFOSeq::LFOSeq(unsigned short seqLength, double rate)
: EnvSegSeq(seqLength)
{
    EnvSegSeq::setLoopStart(0);
    EnvSegSeq::setLoopEnd(seqLength - 1);
    
    EnvSegSeq::setLoopInf();
    
    EnvSegSeq::setSegEndLevel(0, 1);
    
    EnvSegSeq::setSegLen(0, 1000);
    
    for (int i = 1; i < seqLength; ++i)
    {
        EnvSegSeq::setSegLen(i, 100);
        EnvSegSeq::setSegBothLevels(i, 1);
    }
    
    setRate(rate);
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0)
    { throw std::invalid_argument("Rate cannot be less than zero!"); }
    
    _rate = Hz;
    
    // get the period, divide up into _segNum pieces
    double len = (1.0 / _rate) / _segs.size();
    
    // seconds to milliseconds
    len *= 1000;
    
    // Set all segment's lengths
    for (int i = 0; i < _segs.size(); i++)
    {
        EnvSegSeq::setSegLen(i, len);
    }
}

double LFOSeq::modulate(double sample, double depth, double, double)
{
    return sample * tick() * depth;
}

double LFOSeq::getRate() const
{
    return _rate;
}

void LFOSeq::addSegment()
{
    if (_segs.size() == 10)
    { throw std::runtime_error("Segment count already 10!"); }
    
    _segs.push_back(EnvSeg());
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
    setSegLen(0, 200);
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

double LFOUnit::modulate(double sample, double depth, double minBoundary, double maxBoundary)
{
    // Tick the crossfaded value from the lfos and multiply by the envelope
    // value and the total amplitude value
    return fader.modulate(sample, depth, minBoundary, maxBoundary);// * env.tick() * _amp;
}
