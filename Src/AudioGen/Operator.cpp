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
    mods_[AMP].setHigherBoundary(1);
    mods_[AMP].setLowerBoundary(0);
    mods_[AMP].setBaseValue(amp);
    
    mods_[SEMI_OFFSET].setHigherBoundary(48);
    mods_[SEMI_OFFSET].setLowerBoundary(-48);
    mods_[SEMI_OFFSET].setBaseValue(0);
    
    mods_[CENT_OFFSET].setHigherBoundary(100);
    mods_[CENT_OFFSET].setLowerBoundary(0);
    mods_[CENT_OFFSET].setBaseValue(0);
    
    // Set wavetable for all oscillators
    setWavetable(wt);
}

Operator::Operator(const Operator& other)
: GenUnit(other),
  semitoneOffset_(other.semitoneOffset_),
  centOffset_(other.centOffset_),
  wavetableID_(other.wavetableID_),
  notes_(other.notes_.size())
{
    for (noteVec::size_type i = 0; i < notes_.size(); ++i)
    {
        notes_[i].reset(new Oscillator(*other.notes_[i]));
    }
}

Operator& Operator::operator= (const Operator& other)
{
    if (this != &other)
    {
        GenUnit::operator=(other);
        
        semitoneOffset_ = other.semitoneOffset_;
        
        centOffset_ = other.centOffset_;
        
        wavetableID_ = other.wavetableID_;
        
        notes_.resize(other.notes_.size());
        
        for (noteVec::size_type i = 0; i < notes_.size(); ++i)
        {
            *notes_[i] = *other.notes_[i];
        }
    }
    
    return *this;
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
    mods_[SEMI_OFFSET].setBaseValue(semitoneOffset);
    
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
    if (mods_[SEMI_OFFSET].inUse())
    {
        return mods_[SEMI_OFFSET].getBaseValue();
    }
    
    else return semitoneOffset_;
}

void Operator::setCentOffset(short centOffset)
{
    mods_[CENT_OFFSET].setBaseValue(centOffset);
    
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
    if (mods_[CENT_OFFSET].inUse())
    {
        return mods_[CENT_OFFSET].getBaseValue();
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
    mods_[AMP].setBaseValue(amp);
}

double Operator::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    else return amp_;
}

void Operator::increment()
{
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        (*itr)->increment();
    }
}

double Operator::tick()
{
    double val = 0;
    
    // Modulate semitones for all oscillators/notes
    if (mods_[SEMI_OFFSET].inUse())
    {
        double newSemitoneOffset = mods_[SEMI_OFFSET].tick();
        
        for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
             itr != end;
             ++itr)
        {
            (*itr)->setSemitoneOffset(newSemitoneOffset);
        }
    }
    
    // Modulate semitones for all oscillators/notes
    if (mods_[CENT_OFFSET].inUse())
    {
        double newCentOffset = mods_[CENT_OFFSET].tick();
        
        for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
             itr != end;
             ++itr)
        {
            (*itr)->setCentOffset(newCentOffset);
        }
    }
    
    // Check modulation dock for the amplitude parameter
    if (mods_[AMP].inUse())
    {
        amp_ = mods_[AMP].tick();
    }
    
    // Add up all notes
    for (noteVec::iterator itr = notes_.begin(), end = notes_.end();
         itr != end;
         ++itr)
    {
        val += (*itr)->tick();
    }
    
    return val * amp_;
}