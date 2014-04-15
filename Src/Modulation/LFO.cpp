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

LFOSeq::LFOSeq()
{
    _xfade = new XFadeLinear;
    
    _seqLen = 16;
    
    _seqs[A] = new EnvSegSeq(_seqLen);
    _seqs[B] = new EnvSegSeq(_seqLen);
}

LFOSeq::~LFOSeq()
{
    delete _xfade;
    delete _seqs[A];
    delete _seqs[B];
}

double LFOSeq::tick()
{
    double tickA = _seqs[A]->tick() * _xfade->left();
    
    double tickB = _seqs[B]->tick() * _xfade->right();
    
    return tickA + tickB;
}

void LFOSeq::setXFade(double xfade)
{
    _xfade->setValue(xfade);
}

void LFOSeq::setWave(int unitNum, int segNum, Wavetable::Modes wave)
{
    _seqs[unitNum]->setSegModWave(segNum, wave);
}

void LFOSeq::setLevel(int unitNum, int segNum, double lv)
{
    _seqs[unitNum]->setSegBothLevels(segNum, lv);
}

void LFOSeq::setRate(int segNum, double hertz)
{
    // get the period, divide up into _segNum pieces
    double len = (1.0 / hertz) / _seqLen;
    
    // seconds to milliseconds
    len *= 1000;
    
    // The rate determines the length of each segment
    EnvSegSeq& seq = *_seqs[segNum];
    
    // Set all segment's lengths
    for (int i = 0; i < _seqLen; i++)
    {
        seq.setSegLen(i, len);
    }
}