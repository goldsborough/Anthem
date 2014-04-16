//
//  LFO.cpp
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "LFO.h"
#include "EnvSeg.h"
#include "Crossfader.h"
#include "Oscillator.h"

LFOModeBase::LFOModeBase() { _xfade = 0; }

LFOModeBase::~LFOModeBase() {  delete _xfade; }

void LFOModeBase::setXFade(double value) { _xfade->setValue(value); }

LFOSeq::LFOSeq()
{
    _seqMaxCount = 16;
    
    _xfade = new XFadeLinear;
    
    _seqs[0] = new EnvSegSeq(_seqMaxCount);
    _seqs[1] = new EnvSegSeq(_seqMaxCount);
}

LFOSeq::~LFOSeq()
{
    delete _seqs[0];
    delete _seqs[1];
}

double LFOSeq::first() { return _seqs[0]->tick(); }

double LFOSeq::second() { return _seqs[1]->tick(); }

// with crossfading
double LFOSeq::tick()
{
    double val_A = _seqs[0]->tick() * _xfade->left();
    double val_B = _seqs[1]->tick() * _xfade->right();
    
    return val_A + val_B;
}

void LFOSeq::setWave(bool unitNum, unsigned int segNum, Wavetable::Modes wave)
{
    if (segNum > _seqMaxCount)
        throw std::out_of_range("Segment number out of range!");
    
    _seqs[unitNum]->setSegModWave(segNum, wave);
}

void LFOSeq::setLevel(bool unitNum, unsigned int segNum, double lv)
{
    if (segNum > _seqMaxCount)
        throw std::out_of_range("Segment number out of range!");
    
    _seqs[unitNum]->setSegBothLevels(segNum, lv);
}

void LFOSeq::setRate(bool unitNum, double Hz)
{
    // get the period, divide up into _segNum pieces
    double len = (1.0 / Hz) / _seqMaxCount;
    
    // seconds to milliseconds
    len *= 1000;
    
    // Set all segment's lengths
    for (int i = 0; i < _seqMaxCount; i++)
        _seqs[unitNum]->setSegLen(i, len);
}

LFOWave::LFOWave()
{
    _oscs[0] = new Oscillator(Wavetable::SINE,2);
    _oscs[1] = new Oscillator(Wavetable::SINE,2);
    
    _xfade = new XFadeLinear;
}

LFOWave::~LFOWave()
{
    delete _oscs[0];
    delete _oscs[1];
}

double LFOWave::tick()
{
    double val_A = _oscs[0]->tick() * _xfade->left();
    double val_B = _oscs[1]->tick() * _xfade->right();
    
    return val_A + val_B;
}

double LFOWave::first() { return _oscs[0]->tick(); }

double LFOWave::second() { return _oscs[1]->tick(); }

void LFOWave::setWave(bool oscNum, Wavetable::Modes wave)
{
    _oscs[oscNum]->setWT(wave);
}

void LFOWave::setPhaseOffset(bool oscNum, unsigned short degrees)
{
    _oscs[oscNum]->setPhaseOffset(degrees);
}

void LFOWave::setRate(unit_t unitNum, double hertz)
{
    _oscs[unitNum]->setFreq(hertz);
}

LFO::LFO()
: _currMode(_lfoWave)
{
    // Change derived type for different crossfading
    _xfade = new XFadeLinear;
    
    _env = new EnvSegSeq(2);
    
    _env->setSegLen(0, 100);
    _env->setSegEndLevel(0, 1);
    _env->setSegStartLevel(1, 1);
    
    _env->setOneShot();
}

LFO::~LFO()
{
    delete _env;
}

double LFO::tick()
{
    double valA = _currMode.first() * _xfade->left();
    double valB = _currMode.second() * _xfade->right();
    
    return (valA + valB) * _Amp;
}

void LFO::setMode(bool mode)
{
     if (mode)
         _currMode = _lfoSeq;
    
     else _currMode = _lfoWave;
}

void LFO::setEnvSegLen(bool envSeg, unsigned short len)
{
    _env->setSegLen(envSeg, len);
}

void LFO::setRate(bool unitNum, double Hz)
{
    _currMode.setRate(unitNum, Hz);
}

void LFO::setEnvLevel(unsigned char point, double lv)
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

void LFO::setEnvLoopMax(unsigned char loopNum)
{
    _env->setLoopMax(loopNum);
}