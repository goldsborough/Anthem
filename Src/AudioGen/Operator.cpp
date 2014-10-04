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

Operator::Operator(short wt, double amp)
: GenUnit(3,amp)
{
    _mods[AMP]->setHigherBoundary(1);
    _mods[AMP]->setLowerBoundary(0);
    _mods[AMP]->setBaseValue(amp);
    
    _mods[SEMI_OFFSET]->setHigherBoundary(48);
    _mods[SEMI_OFFSET]->setLowerBoundary(-48);
    _mods[SEMI_OFFSET]->setBaseValue(0);
    
    _mods[CENT_OFFSET]->setHigherBoundary(100);
    _mods[CENT_OFFSET]->setLowerBoundary(0);
    _mods[CENT_OFFSET]->setBaseValue(0);
    
    // Set wavetable for all oscillators
    setWavetable(wt);
}

Operator::~Operator()
{
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        delete *itr;
    }
}

void Operator::setWavetable(short wt)
{
    // Store id to add new notes with the same wavetable
    _wavetableID = wt;
    
    // Reset wavetable for all notes
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        (*itr)->setWavetable(wt);
    }
}

short Operator::getWavetableID() const
{
    return _wavetableID;
}

void Operator::setSemitoneOffset(short semitoneOffset)
{
    _mods[SEMI_OFFSET]->setBaseValue(semitoneOffset);
    
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        (*itr)->setSemitoneOffset(semitoneOffset);
    }
    
    _semitoneOffset = semitoneOffset;
}

short Operator::getSemitoneOffset() const
{
    return _semitoneOffset;
}

void Operator::setCentOffset(short centOffset)
{
    _mods[CENT_OFFSET]->setBaseValue(centOffset);
    
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        (*itr)->setCentOffset(centOffset);
    }
    
    _centOffset = centOffset;
}

short Operator::getCentOffset() const
{
    return _centOffset;
}

void Operator::addNote(double frq)
{
    // Initialize and push back a new oscillator with the
    // operator's current wavetable id and the new frequency
    _notes.push_back(new Oscillator(_wavetableID,frq));
}

void Operator::relNote(double frq)
{
    // Iterate over all notes
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        // If that frequency is found
        if ((*itr)->getFreq() == frq)
        {
            // Delete the oscillator
            delete *itr;
            
            // And erase the pointer from the vector
            _notes.erase(itr);
            
            break;
        }
    }
}

void Operator::relNote(unsigned short ind)
{
    // Delete the oscillator
    delete _notes[ind];
    
    // Remove the pointer from the vector
    _notes.erase(_notes.begin() + ind);
}

double Operator::getNoteFreq(unsigned short ind)
{
    return _notes[ind]->getFreq();
}

void Operator::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    GenUnit:setAmp(amp);
    
    // Sets new base value for modulation
    _mods[AMP]->setBaseValue(amp);
}

double Operator::tick()
{
    double val = 0;
    
    // Modulate semitones for all oscillators/notes
    if (_mods[SEMI_OFFSET]->inUse())
    {
        double newSemitoneOffset = _mods[SEMI_OFFSET]->tick();
        
        for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
             itr != end;
             ++itr)
        {
            (*itr)->setSemitoneOffset(newSemitoneOffset);
        }
    }
    
    // Modulate semitones for all oscillators/notes
    if (_mods[CENT_OFFSET]->inUse())
    {
        double newCentOffset = _mods[CENT_OFFSET]->tick();
        
        for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
             itr != end;
             ++itr)
        {
            (*itr)->setCentOffset(newCentOffset);
        }
    }
    
    // Add up all notes
    for (noteVec::iterator itr = _notes.begin(), end = _notes.end();
         itr != end;
         ++itr)
    {
        val += (*itr)->tick();
    }
    
    // Check modulation dock for the amplitude parameter
    if (_mods[AMP]->inUse())
    {
        return val * _mods[AMP]->tick();
    }
    
    return val * _amp;
}