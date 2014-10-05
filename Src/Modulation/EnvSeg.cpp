//
//  EnvSeg.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "EnvSeg.h"
#include "LFO.h"
#include "Global.h"
#include "Wavetable.h"
#include "ModDock.h"

#include <cmath>

// 60 seconds
const unsigned long EnvSeg::_maxLen = 2646000;

EnvSeg::EnvSeg(double startAmp,
               double endAmp,
               len_t len,
               double segRate)

: _sample(0),
  _startLevel(startAmp),
  _endLevel(endAmp),
  rate_(segRate),
  _len(len),
  _lastTick(startAmp),
  GenUnit(3) // two ModDocks

{
    _mods[SEG_RATE]->setHigherBoundary(2);
    _mods[SEG_RATE]->setLowerBoundary(0);
    _mods[SEG_RATE]->setBaseValue(1);
    
    _mods[SEG_START_LEVEL]->setHigherBoundary(1);
    _mods[SEG_START_LEVEL]->setLowerBoundary(0);
    _mods[SEG_START_LEVEL]->setBaseValue(startAmp);
    
    _mods[SEG_END_LEVEL]->setHigherBoundary(1);
    _mods[SEG_END_LEVEL]->setLowerBoundary(0);
    _mods[SEG_END_LEVEL]->setBaseValue(endAmp);
    
    calcParams_(_startLevel,_endLevel,rate_);
}

void EnvSeg::reset()
{
    _sample = 0;
    
    // reset _segCurr
    calcParams_(_startLevel,_endLevel,rate_);
}

void EnvSeg::_calcLevel(double startLevel, double endLevel)
{
    // The range between start and end
    _diff = endLevel - startLevel;
    
    if (!_diff) _ads = SUST; // no difference means sustain
    
    else if (_diff > 0) // positive difference means attack
    {
        _ads = ATK;
        _offset  = startLevel;
    }
    
    else // negative difference means decay
    {
        _ads = DEC;
        
        // why do I have to do this?
        _diff = -_diff;
        
        _offset = endLevel;
    }
}

void EnvSeg::_calcRate(double rate, double endLevel)
{
    // Uses iterative exponential
    // calculations instead of
    // calling the pow function
    
    // All courtesy for this algorithm
    // goes to Daniel R. Mitchell
    
    if (rate == 1) _type = LIN;
    
    else if (rate < 1) _type = EXP;
    
    else
    {
        _type = LOG;
        
        // Rate must still be between 0 and 1, the
        // 1 - 2 range is just for covenience
        rate = 2 - rate;
    }
    
    // Calculate rate
    
    if (_ads == SUST)
    {
        // Multiplication factor stays 1
        _segIncr = 1;
        
        _offset = endLevel;
    }
    
    else
    {
        _segCurr = rate;
        
        // Deduct sample count from length so that real-time
        // changes consider the passed samples.
        double len = _len - _sample;
        
        double exp = (len > 0) ? 1.0 / len : 0;
        
        double base;
        
        // Attack or Decay, which must be inversed if logarithmic type
        if ((_ads == ATK && _type != LOG) ||
            (_ads == DEC && _type == LOG))
        {
            base = (1 + rate) / rate;
        }
        
        // Decay or Attack for Logarithmic curves
        else
        {
            _segCurr++;
            
            base = rate /( 1 + rate);
        }
        
        _segIncr = pow(base,exp);
        
    }
}

void EnvSeg::calcParams_(double startLevel, double endLevel, double rate)
{
    _calcLevel(startLevel, endLevel);
    _calcRate(rate,endLevel);
}

void EnvSeg::setLen(EnvSeg::len_t sampleLen)
{
    if (sampleLen > _maxLen)
    {
        throw std::invalid_argument("EnvSeg length cannot exceed max length!");
    }
    
    _len = sampleLen;
    
    _calcRate(rate_, _endLevel);
}

EnvSeg::len_t EnvSeg::getLen() const
{
    return _len;
}

void EnvSeg::setStartLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    _startLevel = lv;
    
    // Necessary for setSegRate at the very beginning
    if (!_sample) _lastTick = _startLevel;
    
    _mods[SEG_START_LEVEL]->setBaseValue(_startLevel);
    
    calcParams_(_startLevel,_endLevel,rate_);
}

double EnvSeg::getStartLevel() const
{
    return _startLevel;
}

void EnvSeg::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    _endLevel = lv;
    
    _mods[SEG_END_LEVEL]->setBaseValue(_endLevel);
    
    calcParams_(_startLevel,_endLevel,rate_);
}

double EnvSeg::getEndLevel() const
{
    return _endLevel;
}

void EnvSeg::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
}

void EnvSeg::setRate(double rate)
{
    if (rate > 2 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 2"); }
    
    rate_ = rate;
    
    _mods[SEG_RATE]->setBaseValue(rate_);
    
    // Calculate level from last tick rather than from start amplitude
    // so that the rate is adjusted properly
    calcParams_(_startLevel,_lastTick,rate_);
}

double EnvSeg::getRate() const
{
    return rate_;
}

double EnvSeg::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // (which shouldn't happen because the
    // envelope should move on), just return
    // the end amplitude.
    
    if (_sample++ >= _len) return _lastTick;
    
    // Modulate members
    if (_mods[SEG_RATE]->inUse()        ||
        _mods[SEG_START_LEVEL]->inUse() ||
        _mods[SEG_END_LEVEL]->inUse())
    {
        double rate = rate_,
               start = _lastTick,
               end = _endLevel;
        
        if (_mods[SEG_RATE]->inUse())
        {
            rate = _mods[SEG_RATE]->inUse();
        }
        
        if (_mods[SEG_START_LEVEL]->inUse())
        {
            start = _mods[SEG_START_LEVEL]->tick();
        }
        
        if (_mods[SEG_END_LEVEL]->inUse())
        {
            end = _mods[SEG_END_LEVEL]->tick();
        }
        
        calcParams_(start,end,rate);
    }
    
    // Increment the envelope value
    _segCurr *= _segIncr;
    
    _lastTick = _segCurr;
    
    if (_ads != SUST) _lastTick -= rate_;
    
    if (_type == LOG) _lastTick = 1 - _lastTick;
    
    _lastTick = (_lastTick * _diff) + _offset;
    
    return _lastTick;
}

EnvSegSeq::EnvSegSeq(EnvSegSeq::seg_t seqLen)
: _segs(seqLen), _currSample(0), _loopCount(0),
  _loopMax(0), _loopInf(false), _lastTick(0)
{
    _loopStart = _loopEnd = _segs.end();
    
    _currSeg = _segs.begin();
}

void EnvSegSeq::setSegRate(seg_t seg, double rate)
{
    _segs[seg].setRate(rate);
}

double EnvSegSeq::getSegRate(seg_t seg) const
{
    return _segs[seg].getRate();
}


void EnvSegSeq::setSegStartLevel(seg_t seg, double lv)
{
    _segs[seg].setStartLevel(lv);
}

double EnvSegSeq::getSegStartLevel(seg_t seg) const
{
    return _segs[seg].getStartLevel();
}


void EnvSegSeq::setSegEndLevel(seg_t seg, double lv)
{
    _segs[seg].setEndLevel(lv);
}

double EnvSegSeq::getSegEndLevel(seg_t seg) const
{
    return _segs[seg].getEndLevel();
}


void EnvSegSeq::setSegLen(seg_t seg, unsigned long ms)
{
    _segs[seg].setLen(ms * 44.1);
}

unsigned long EnvSegSeq::getSegLen(seg_t seg) const
{
    return _segs[seg].getLen();
}


void EnvSegSeq::setSegBothLevels(seg_t seg, double lv)
{
    setSegStartLevel(seg, lv), setSegEndLevel(seg, lv);
}


void EnvSegSeq::setLinkedLevel(seg_t seg, double lv)
{
    _segs[seg].setEndLevel(lv);
    
    // Set starting level of adjacent segment (unless
    // this is the last segment)
    if (seg < _segs.size() - 1)
    {
        _segs[seg + 1].setStartLevel(lv);
    }
}


void EnvSegSeq::setLoopStart(EnvSegSeq::seg_t seg)
{
    _loopStart = _segs.begin() + seg;
}

EnvSegSeq::seg_t EnvSegSeq::getLoopStart() const
{
    return _loopStart - _segs.begin();
}


void EnvSegSeq::setLoopEnd(EnvSegSeq::seg_t seg)
{
    // Plus one because the end points to one after
    // the loop end segment
    _loopEnd = _segs.begin() + seg + 1;
}

EnvSegSeq::seg_t EnvSegSeq::getLoopEnd() const
{
    return _loopEnd - _segs.begin();
}


void EnvSegSeq::setLoopMax(unsigned short n)
{
    if (n > 64) _loopInf = true;
    else
    {
        _loopInf = false;
        _loopMax = n;
    }
}

unsigned short EnvSegSeq::getLoopMax() const
{
    return _loopMax;
}


void EnvSegSeq::setLoopInf(bool state)
{
    _loopInf = state;
}

bool EnvSegSeq::loopInf() const
{
    return _loopInf;
}


std::vector<EnvSeg>::size_type EnvSegSeq::size() const
{
    return _segs.size();
}


void EnvSegSeq::addSegment()
{
    _segs.push_back(EnvSeg());
}

void EnvSegSeq::removeSegment()
{
    if (_segs.empty())
    { throw std::runtime_error("Cannot remove segment from LFOSeq if already empty!"); }
    
    _segs.erase(_segs.end() - 1);
}

void EnvSegSeq::removeSegment(seg_t seg)
{
    if (seg >= _segs.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    _segs.erase(_segs.begin() + seg);
}

void EnvSegSeq::_changeSeg(EnvSegSeq::segItr seg)
{
    _currSeg = seg;
    
    _currSample = 0;
}

void EnvSegSeq::_resetLoop()
{
    // reset all the loop segments
    for (segItr itr = _loopStart; itr != _loopEnd; ++itr)
    { itr->reset(); }
    
    _changeSeg(_loopStart);
    
    ++_loopCount;
}

double EnvSegSeq::tick()
{
    if (_currSample++ >= _currSeg->getLen())
    {
        if (_currSeg != _segs.end()) _currSeg++;
        
        // Check if we need to reset the loop
        if (_currSeg == _loopEnd && (_loopInf || _loopCount < _loopMax))
        { _resetLoop(); }
        
        // send out the last good value if no more segments left
        else if (_currSeg == _segs.end()) return _lastTick;
        
        // else increment segment
        else
        {
            _changeSeg(_currSeg);
            
            // Return last tick if the new segment has no length
            if (! _currSeg->getLen()) return _lastTick;
        }
    }

    _lastTick = _currSeg->tick();
    
    return _lastTick;
    
}

ModEnvSegSeq::ModEnvSegSeq(seg_t numSegs,
                           seg_t dockNum,
                           double amp)
: EnvSegSeq(numSegs),
  ModUnit(dockNum + 2,amp)
{ }

void ModEnvSegSeq::setModUnitDepth(seg_t segNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth)
{
    if (dockNum == SEG_LEVEL)
    {
        _segs[segNum].setModUnitDepth(EnvSeg::SEG_END_LEVEL, modNum, depth);
        
        if (segNum < _segs.size() - 1)
        {
            _segs[segNum + 1].setModUnitDepth(EnvSeg::SEG_START_LEVEL, modNum, depth);
        }
    }
    
    else
    {
        _segs[segNum].setModUnitDepth(dockNum,modNum, depth);
    }
}

void ModEnvSegSeq::attachMod(seg_t segNum,
                             index_t dockNum,
                             ModUnit *mod)
{
    // Set linked modulator if SEG_LEVEL
    if (dockNum == SEG_LEVEL)
    {
        _segs[segNum].attachMod(EnvSeg::SEG_END_LEVEL, mod);
        
        // If last segment chosen, no next segment to set start level for
        if (segNum < _segs.size() - 1)
        {
            _segs[segNum + 1].attachMod(EnvSeg::SEG_START_LEVEL, mod);
        }
    }
    
    else
    {
        _segs[segNum].attachMod(dockNum, mod);
    }
}

void ModEnvSegSeq::detachMod(seg_t segNum,
                             index_t dockNum,
                             index_t modNum)
{
    if (dockNum == SEG_LEVEL)
    {
        _segs[segNum].detachMod(dockNum, modNum);
        
        // If last segment chosen, no next segment to set start level for
        if (segNum < _segs.size() - 1)
        {
            _segs[segNum + 1].detachMod(dockNum, modNum);
        }
    }
    
    else
    {
        _segs[segNum].detachMod(dockNum, modNum);
    }
}