//
//  Operator.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Operator.h"
#include "Wavetable.h"
#include "Oscillator.h"
#include "ModDock.h"
#include "Util.h"

Operator::Operator(const int& wt, const double& amp)
{
    _amp = amp;
    
    // Set wavetable for all oscillators
    setWavetable(wt);
    
    _mods = {new ModDock(4), new ModDock(2), new ModDock(2) };
}

Operator::~Operator()
{
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        delete *itr;
    }
}

void Operator::setWavetable(const unsigned short& wt)
{
    // Store id to add new notes with the same wavetable
    _wavetableId = wt;
    
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        (*itr)->setWT(wt);
    }
}

void Operator::setSemis(const double& semis)
{
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        (*itr)->setSemis(semis);
    }
}

void Operator::setCents(const double& cents)
{
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        (*itr)->setCents(cents);
    }
}

void Operator::addNote(const double& frq)
{
    _oscs.push_back(new Oscillator(_wavetableId,frq));
}

void Operator::relNote(const unsigned short& ind)
{
    delete _oscs[ind];
    
    _oscs.erase(_oscs.begin() + ind);
}

double Operator::tick()
{
    double val = 0.0;
    
    if (_mods[FREQ_SEMI]->inUse())
    {
        short semiOffs = _mods[FREQ_SEMI]->checkAndTick(_semis, -48, 48);
        setSemis(semiOffs);
    }
    
    if (_mods[FREQ_CENT]->inUse())
    {
        short centOffs = _mods[FREQ_CENT]->checkAndTick(_cents, 0, 100);
        setCents(centOffs);
    }
    
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        val += (*itr)->tick();
    }
    
    if (_mods[AMP]->inUse())
    { return val * _mods[AMP]->checkAndTick(_amp,0,1); }
    
    else return val * _amp;
}