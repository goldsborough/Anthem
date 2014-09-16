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

LFO::LFO(short wt, double rate, double phaseOffset)
: _osc(new Oscillator())
{
    setWavetable(wt);
    setRate(rate);
    setPhaseOffset(phaseOffset);
    
    // Initialize ModDocks
    _initModDocks();
}

LFO::~LFO()
{ delete _osc; }

void LFO::_initModDocks()
{
    _mods = {new ModDock(2), new ModDock(2), new ModDock(2) };
}

void LFO::setWavetable(short wt)
{ _osc->setWavetable(wt); }

void LFO::setPhaseOffset(double degrees)
{
    _phaseOffset = degrees;
    
    _osc->setPhaseOffset(degrees);
}

void LFO::setRate(double Hz)
{
    _rate = Hz;
    
    _osc->setFreq(Hz);
}

double LFO::tick()
{
    // Set all of these modulations to the oscillator directly so that the internal
    // base values aren't changed (e.g. _rate, in this case, must stay the same, as
    // it's the base value for modulation)
    if (_mods[RATE]->inUse())
    { _osc->setFreq(_mods[RATE]->checkAndTick(_rate, 0, Global::nyquistLimit)); }
    
    if (_mods[PHASE]->inUse())
    { _osc->setPhaseOffset(_mods[PHASE]->checkAndTick(_phaseOffset, 0, 360)); }
    
    double ret = _osc->tick();
    
    // Return ret * modulated value if ModDock in use
    if (_mods[AMP]->inUse())
    { return ret * _mods[AMP]->checkAndTick(_amp, 0, 1); }
    
    // Else unmodulated value
    return ret * _amp;
}

LFOSeq::LFOSeq(unsigned int seqLength)
: EnvSegSeq(seqLength), _seqLen(seqLength)
{
    EnvSegSeq::setLoopStart(0);
    EnvSegSeq::setLoopEnd(_seqLen - 1);
    
    EnvSegSeq::setLoopInf();
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0)
    { throw std::invalid_argument("Rate cannot be less than zero!"); }
    
    // get the period, divide up into _segNum pieces
    double len = (1.0 / Hz) / _seqLen;
    
    // seconds to milliseconds
    len *= 1000;
    
    // Set all segment's lengths
    for (int i = 0; i < _seqLen; i++)
    { }//EnvSegSeq::setSegLen(i, len); }
}

LFOUnit::LFOUnit(unsigned short mode)
{ setMode(mode); }

void LFOUnit::setMode(unsigned short mode)
{
    if (mode)
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

LFOUnit::Env::Env()
: EnvSegSeq(2)
{
    setSegLen(0, 100);
    setSegEndLevel(0, 1);
    setSegStartLevel(1, 1);
    
    setOneShot();
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

double LFOUnit::tick()
{
    return fader.tick() * _amp;//* env.tick() * _amp;
}
