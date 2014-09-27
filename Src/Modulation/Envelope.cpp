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

Envelope::Envelope(unsigned int delayMillis,
                   bool sustainInf)
: EnvSegSeq(7), ModUnit(1,16), _sustainInf(sustainInf),
  _currSegRate(1), _currSegModRate(0), _currSegModDepth(1)
{
    _currSeg = _segs.begin() + DEL;
    
    // Set delay length
    _currSeg->setLen(delayMillis * 44.1);
    
    _currSegNum = DEL;

    // Initial settings
    setSegLevel(Envelope::ATK, 0.8);
    setSegLen(Envelope::ATK, 500);
    
    setSegLevel(Envelope::SEG_A, 0.5);
    setSegLen(Envelope::SEG_A, 250);
    setSegRate(Envelope::SEG_A, 0.1);

    setSegRate(Envelope::REL, 0.1);
    setSegLen(Envelope::REL, 500);
    
    _segs[CONNECTOR].setLen(Global::samplerate / 40);
}

void Envelope::_changeSeg(segItr seg)
{
    _currSeg = seg;
    
    _currSegNum = seg - _segs.begin();
    
    _currSample = 0;

    // If we just changed to the release segment
    // set the start level of the segment to the
    // last ticked value
    if ((seg - _segs.begin()) == REL)
    { _currSeg->setStartLevel(_lastTick); }
    
}

void Envelope::setModUnitDepth(seg_t segNum,
                           index_t dockNum,
                           index_t modNum,
                           double depth)
{
    // Advance docknum for appropriate segment
    Unit::setModUnitDepth(((segNum-2) * 3) + dockNum, modNum, depth);
}


void Envelope::attachMod(seg_t segNum,
                         index_t dockNum,
                         ModUnit* mod)
{
    // Advance docknum for appropriate segment
    // -2 because ATK = 2
    Unit::attachMod(((segNum-2) * 3) + dockNum, mod);
}

void Envelope::detachMod(seg_t segNum,
                         index_t dockNum,
                         index_t modNum)
{
    // Advance docknum for appropriate segment
    Unit::detachMod(((segNum-2) * 3) + dockNum, modNum);
}

void Envelope::setDelay(unsigned int millis)
{
    _segs[DEL].setLen(44.1 * millis);
}

void Envelope::_resetLoop()
{
    // Reset all segments
    for(segItr itr = _segs.begin(), end = _segs.end();
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
        EnvSegSeq::_changeSeg(_segs.begin());
        
        _currSeg->reset();
    }
}

double Envelope::_tick()
{

    if (_currSample++ >= _currSeg->getLen())
    {
        // If we're crossing over the release segment, just return 0
        if (_currSegNum == REL) return 0;
        
        // If this is the end of the connectin segment, change back to the
        // loop start
        else if (_currSegNum == CONNECTOR) _changeSeg(_loopStart);
        
        // If we reached the loop end segment and there are more loops to go
        // through, reset the loop
        else if (_currSeg == _loopEnd && ( _loopInf || _loopCount++ < _loopMax))
        {
           _resetLoop();
        }
        
        // if we reached the end of segment c and the sustain is set to infinity
        // return the last tick
        else if (_currSegNum == SEG_C && _sustainInf) return _lastTick;
        
        else
        {
            _changeSeg(++_currSeg);
            
            if (_currSeg->getLen() == 0) return _lastTick;
        }

    }
    
    if (_currSegNum == DEL) return 0;
    
    _lastTick = _currSeg->tick();
    
    return _lastTick;
}

double Envelope::modulate(double sample, double depth, double, double)
{
    if (_currSegNum == SEG_A)
    {
        
    }
    
    if (_currSegNum > DEL)
    {
        seg_t seg = (_currSegNum - 2) * 3;

        if (_mods[seg+SEG_RATE]->inUse())
        {
            _currSeg->setRate(_mods[seg+SEG_RATE]->modulate(_currSegRate, 0, 2));
        }
        
        if (_mods[seg+MOD_RATE]->inUse())
        {
            _currSeg->setModRate(_mods[seg+MOD_RATE]->modulate(_currSegModRate, 0, 100));
        }
        
        if (_mods[seg+MOD_DEPTH]->inUse())
        {
            _currSeg->setModDepth(_mods[seg+MOD_DEPTH]->modulate(_currSegModDepth, 0, 1));
        }
    }
    
    double ret = sample * _tick() * depth;
    
    if (_mods[AMP]->inUse())
    {
        return ret * _mods[AMP]->modulate(_amp, 0, 1);
    }
    
    return ret * _amp;
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
    if (seg >= REL || seg <= ATK)
    { throw std::invalid_argument("Invalid loop segment, can only loop from Segments A to D!"); }
    
    _loopEnd = _segs.begin() + seg;
    
    _segs[CONNECTOR].setStartLevel(_loopEnd->getEndLevel());
    
    if (_loopEnd < _loopStart)
    { _loopStart = _loopEnd; }
}

void Envelope::setSegLevel(seg_t seg, double lv)
{
    if (seg >= REL || seg <= DEL)
        throw std::invalid_argument("Invalid segment for setting the level!");
    
    _segs[seg].setEndLevel(lv);
    _segs[seg + 1].setStartLevel(lv);
    
}

void Envelope::setSegRate(seg_t seg, double rate)
{
    // Set _currSegRate to new base value for modulation
    if (seg == _currSegNum)
    { _currSegRate = rate; }
    
    EnvSegSeq::setSegRate(seg, rate);
}

void Envelope::setSegModDepth(seg_t seg, double dpth)
{
    if (seg == _currSegNum)
    { _currSegModDepth = dpth; }
    
    EnvSegSeq::setSegModDepth(seg, dpth);
}

void Envelope::setSegModRate(seg_t seg, unsigned short rate)
{
    if (seg == _currSegNum)
    { _currSegModRate = rate; }
    
    EnvSegSeq::setSegModRate(seg, rate);
}

void Envelope::noteOff()
{
    // Switch to release segment
    _changeSeg(_segs.begin() + REL);
}

void Envelope::setSustainInf(bool sustainInf)
{
    _sustainInf = sustainInf;
}

bool Envelope::sustainInf() const
{
    return _sustainInf;
}