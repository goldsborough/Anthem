//
//  Envelope.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 09/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Envelope.h"
#include "Global.h"
#include "ModDock.h"

Envelope::Envelope(unsigned int delayMillis, bool sustainEnabled)

: ModEnvSegSeq(7,1), _sustainEnabled(sustainEnabled)

{
    _currSeg = _segs.begin() + DEL;
    
    // Set delay length
    _currSeg->setLen(delayMillis * 44.1);
    
    _currSegNum = DEL;
    
    // Set the hidden connector length to a 40th of the
    // samplerate, at 44.1 Khz that's 1102 samples. Not
    // too long to be noticed too much but just enough
    // to prevent transitions between loops from being
    // too abrupt
    _segs[CONNECTOR].setLen(Global::samplerate / 40);
    
    // Initial settings
    setSegLevel(Envelope::ATK, 0.8);
    setSegLen(Envelope::ATK, 500);
    
    setSegLevel(Envelope::SEG_A, 0.5);
    setSegLen(Envelope::SEG_A, 250);
    setSegRate(Envelope::SEG_A, 0.1);
    
    setSegRate(Envelope::REL, 0.1);
    setSegLen(Envelope::REL, 500);

    _mods[AMP]->setHigherBoundary(1);
    _mods[AMP]->setLowerBoundary(0);
    _mods[AMP]->setBaseValue(1);
}

void Envelope::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    ModUnit::setAmp(amp);
    
    // Sets new base value for modulation
    _mods[AMP]->setBaseValue(amp);
}

void Envelope::_changeSeg(segItr seg)
{
    EnvSegSeq::_changeSeg(seg);
    
    _currSegNum = seg - _segs.begin();

    // If we just changed to the release segment
    // set the start level of the segment to the
    // last ticked value, (for note-off, could be
    // abrupt)
    if (_currSegNum == REL)
    {
        _currSeg->setStartLevel(_lastTick);
    }
    
}

void Envelope::setDelay(unsigned int millis)
{
    // Convert samples to milliseconds
    _segs[DEL].setLen(44.1 * millis);
}

unsigned int Envelope::getDelay() const
{
    return _segs[DEL].getLen() / 44.1;
}

void Envelope::_resetLoop()
{
    // Reset all segments
    for(segItr itr = _loopStart, end = _loopEnd + 1;
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    // no need for connecting segment if the amplitude
    // is the same e.g. for the constant sustain
    if (_lastTick == _loopStart->getStartLevel())
    {
        _changeSeg(_loopStart);
    }
    
    else
    {
        // Set _currSeg to connector segment
        _changeSeg(_segs.begin());
        
        _currSeg->reset();
    }
}

double Envelope::_tick()
{

    if (_currSample++ >= _currSeg->getLen())
    {
        // If we're crossing over the release segment, just return 0
        if (_currSegNum == REL)
        {
            return 0;
        }
        
        // If this is the end of the connecting segment, change back to the
        // loop start
        else if (_currSegNum == CONNECTOR)
        {
            _changeSeg(_loopStart);
        }
        
        // If we reached the loop end segment and there are more loops to go
        // through, reset the loop
        else if (_currSeg == _loopEnd && ( _loopInf || _loopCount++ < _loopMax))
        {
           _resetLoop();
        }
        
        // if we reached the end of segment c and the sustain is set to infinity
        // return the last tick
        else if (_currSegNum == SEG_C && _sustainEnabled)
        {
           return _lastTick;
        }
        
        else
        {
            _changeSeg(++_currSeg);
            
            if ( ! _currSeg->getLen() ) return _lastTick;
        }

    }
    
    if (_currSegNum == DEL) return 0;
    
    _lastTick = _currSeg->tick();
    
    return _lastTick;
}

double Envelope::modulate(double sample, double depth, double)
{
    double ret = sample * _tick() * depth;
    
    double amp = _amp;
    
    if (_mods[AMP]->inUse())
    {
        amp = _mods[AMP]->tick();
    }
    
    return ret * amp;
}

void Envelope::setLoopStart(seg_t seg)
{
    
    if (seg >= REL || seg <= ATK)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments A to D!"); }
    
    _loopStart = _segs.begin() + seg;
    
    _segs[CONNECTOR].setEndLevel(_loopStart->getStartLevel());

    if (_loopStart > _loopEnd)
    { _loopEnd = _loopStart; }
}

void Envelope::setLoopEnd(seg_t seg)
{
    if (seg >= REL || seg < ATK)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments ATK to SEG_C!"); }
    
    _loopEnd = _segs.begin() + seg;
    
    _segs[CONNECTOR].setStartLevel(_loopEnd->getEndLevel());
    
    if (_loopEnd < _loopStart)
    { _loopStart = _loopEnd; }
}

void Envelope::setSegLevel(seg_t seg, double lv)
{
    if (seg >= REL || seg < ATK)
    { throw std::invalid_argument("Invalid segment for setting the level!"); }
    
    // Set new levels
    EnvSegSeq::setLinkedLevel(seg, lv);
}

void Envelope::noteOff()
{
    if (_currSegNum != REL)
    {
        // Switch to release segment
        _changeSeg(_segs.begin() + REL);
    }
}

void Envelope::setSustainEnabled(bool sustainEnabled)
{
    _sustainEnabled = sustainEnabled;
}

bool Envelope::sustainEnabled() const
{
    return _sustainEnabled;
}