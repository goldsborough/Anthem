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
    { return _mods[AMP]->checkAndTick(_amp, 0, 1) * ret; }
    
    return _amp * ret;
}

LFOSeq::LFOSeq(unsigned int seqLength)
: seq(seqLength), _seqLen(seqLength)
{
    seq.setLoopStart(0);
    seq.setLoopEnd(_seqLen - 1);
    
    seq.setLoopInf();
}

void LFOSeq::setRate(double Hz)
{
    if (Hz < 0)
    { throw std::invalid_argument("Rate cannot be greater zero!"); }
    
    // get the period, divide up into _segNum pieces
    double len = (1.0 / Hz) / _seqLen;
    
    // seconds to milliseconds
    len *= 1000;
    
    // Set all segment's lengths
    for (int i = 0; i < _seqLen; i++)
        seq.setSegLen(i, len);
}

LFOUnit::LFOUnit()
{
    // Change derived type for different crossfading
    _xfade = new XFadeLinear(-100);
    
    _env = new EnvSegSeq(2);
    
    _env->setSegLen(0, 100);
    _env->setSegEndLevel(0, 1);
    _env->setSegStartLevel(1, 1);
    
    _env->setOneShot();
}

LFOUnit::~LFOUnit() { delete _env; }

double LFOUnit::tick()
{
    double valA = (_mode) ? _LFOSeqs[0].tick() : _LFOs[0].tick();
    double valB = (_mode) ? _LFOSeqs[1].tick() : _LFOs[1].tick();
    
    valA *= _xfade->left();
    valB *= _xfade->right();
    
    return (valA + valB) * _amp;
}

void LFOUnit::setEnvSegLen(bool envSeg, unsigned short len)
{
    _env->setSegLen(envSeg, len);
}

void LFOUnit::setRate(bool unitNum, double Hz)
{
    if (_mode)
        _LFOSeqs[unitNum].setRate(Hz);
    
    else _LFOs[unitNum].setRate(Hz);
}

void LFOUnit::setEnvLevel(unsigned char point, double lv)
{
    switch (point)
    {
        case 0:
            _env->setSegStartLevel(SegA, lv);
            break;
        
        case 1:
            _env->setSegEndLevel(SegA, lv);
            _env->setSegStartLevel(SegB, lv);
            break;
        
        case 2:
            _env->setSegEndLevel(SegB, lv);
            break;
    }
}

void LFOUnit::setXFade(char value) { _xfade->setValue(value); }

void LFOUnit::setEnvLoopMax(unsigned char loopNum)
{
    _env->setLoopMax(loopNum);
}

