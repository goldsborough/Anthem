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

void Operator::setWavetable(short wt)
{
    // Store id to add new notes with the same wavetable
    wavetableID_ = wt;
    
    // Reset wavetable for all notes
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        (*itr)->setWavetable(wt);
    }
}

short Operator::getWavetableID() const
{
    return wavetableID_;
}

void Operator::setSemitoneOffset(short semitoneOffset)
{
    _mods[SEMI_OFFSET]->setBaseValue(semitoneOffset);
    
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        (*itr)->setSemitoneOffset(semitoneOffset);
    }
    
    semitoneOffset_ = semitoneOffset;
}

short Operator::getSemitoneOffset() const
{
    if (_mods[SEMI_OFFSET]->inUse())
    {
        return _mods[SEMI_OFFSET]->getBaseValue();
    }
    
    else return semitoneOffset_;
}

void Operator::setCentOffset(short centOffset)
{
    _mods[CENT_OFFSET]->setBaseValue(centOffset);
    
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        (*itr)->setCentOffset(centOffset);
    }
    
    centOffset_ = centOffset;
}

short Operator::getCentOffset() const
{
    if (_mods[CENT_OFFSET]->inUse())
    {
        return _mods[CENT_OFFSET]->getBaseValue();
    }
    
    else return centOffset_;
}

void Operator::addNote(double frq)
{
    // Initialize and push back a new oscillator in a unique_ptr with the
    // operator's current wavetable id and the new frequency
    notes_.push_back(std::unique_ptr<Oscillator>(new Oscillator(wavetableID_,frq)));
}

void Operator::relNote(double frq)
{
    // Iterate over all notes
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        // If that frequency is found
        if ((*itr)->getFreq() == frq)
        {
            // And erase the pointer from the vector
            notes_.erase(itr);
            
            break;
        }
    }
}

void Operator::relNote(unsigned short ind)
{
    // Remove the pointer from the vector
    notes_.erase(notes_.begin() + ind);
}

double Operator::getNoteFreq(unsigned short ind)
{
    return notes_[ind]->getFreq();
}

void Operator::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    GenUnit::setAmp(amp);
    
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
        
        for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
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
        
        for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
             itr != end;
             ++itr)
        {
            (*itr)->setCentOffset(newCentOffset);
        }
    }
    
    // Add up all notes
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        val += (*itr)->tick();
    }
    
    double amp = _amp;
    
    // Check modulation dock for the amplitude parameter
    if (_mods[AMP]->inUse())
    {
        amp = _mods[AMP]->tick();
    }
    
    return val * amp;
}