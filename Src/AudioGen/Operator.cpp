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

Operator::Operator(const short& wt, const double& amp)
{
    _amp = amp;
    
    // Set wavetable for all oscillators
    setWavetable(wt);
    
    // Initialize mod docks
    _initModDocks();
}

void Operator::_initModDocks()
{
    // two for the amplitude, two each for semitone and cent offsetting
    _mods = {new ModDock(4), new ModDock(2), new ModDock(2)};
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

void Operator::setWavetable(const short& wt)
{
    // Store id to add new notes with the same wavetable
    _wavetableId = wt;
    
    // Reset wavetable for all notes
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        (*itr)->setWavetable(wt);
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
    // Initialize and push back a new oscillator with the
    // operator's current wavetable id and the new frequency
    _oscs.push_back(new Oscillator(_wavetableId,frq));
}

void Operator::relNote(const double& frq)
{
    // Iterate over all notes
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        // If that frequency is found
        if ((*itr)->getFreq() == frq)
        {
            // Delete the oscillator
            delete *itr;
            
            // And erase the pointer from the vector
            _oscs.erase(itr);
            
            break;
        }
    }
}

void Operator::relNote(const unsigned short& ind)
{
    // Delete the oscillator
    delete _oscs[ind];
    
    // Remove the pointer from the vector
    _oscs.erase(_oscs.begin() + ind);
}

double Operator::tick()
{
    double val = 0;
    
    // If the ModDock for semitone offsetting has any modulation units (is in use),
    // grab the current value from all the modulation units and set the semitones
    // for all oscillators
    if (_mods[FREQ_SEMI]->inUse())
    {
        short semiOffs = _mods[FREQ_SEMI]->checkAndTick(_semis, -48, 48);
        setSemis(semiOffs);
    }
    
    // Same goes for cent offsetting
    if (_mods[FREQ_CENT]->inUse())
    {
        short centOffs = _mods[FREQ_CENT]->checkAndTick(_cents, 0, 100);
        setCents(centOffs);
    }
    
    // Add up all notes
    for (oscVec::iterator itr = _oscs.begin(), end = _oscs.end();
         itr != end;
         ++itr)
    {
        val += (*itr)->tick();
    }
    
    // Check modulation dock for the amplitude parameter
    if (_mods[AMP]->inUse())
    { return val * _mods[AMP]->checkAndTick(_amp,0,1); }
    
    return val * _amp;
}