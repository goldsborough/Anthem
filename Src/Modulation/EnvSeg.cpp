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
  _segRate(segRate),
  _len(len),
  _lastTick(startAmp)

{
    _calcLevel(_startLevel,_endLevel);
    _calcRate();
}

void EnvSeg::reset()
{
    _sample = 0;
    
    // reset _segCurr
    _calcRate();
    _calcLevel(_startLevel,_endLevel);
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

void EnvSeg::_calcRate()
{
    // Uses iterative exponential
    // calculations instead of
    // calling the pow function
    
    // All courtesy for this algorithm
    // goes to Daniel R. Mitchell
    
    if (_segRate == 1) _type = LIN;
    
    else if (_segRate < 1) _type = EXP;
    
    else
    {
        _type = LOG;
        
        // Rate must still be between 0 and 1, the
        // 1 - 2 range is just for covenience
        _segRate = 2 - _segRate;
    }
    
    // Calculate rate
    
    if (_ads == SUST)
    {
        // Multiplication factor stays 1
        _segIncr = 1;
        
        _offset = _endLevel;
    }
    
    else
    {
        _segCurr = _segRate;
        
        // Deduct sample count from length so that real-time
        // changes consider the passed samples.
        double len = _len - _sample;
        
        double exp = (len > 0) ? 1.0 / len : 0;
        
        double base;
        
        // Attack or Decay, which must be inversed if logarithmic type
        if ((_ads == ATK && _type != LOG) ||
            (_ads == DEC && _type == LOG))
        {
            base = (1 + _segRate) / _segRate;
        }
        
        // Decay or Attack for Logarithmic curves
        else
        {
            _segCurr++;
            
            base = _segRate /( 1 + _segRate);
        }
        
        _segIncr = pow(base,exp);
        
    }
}

void EnvSeg::setLen(EnvSeg::len_t sampleLen)
{
    if (sampleLen > _maxLen)
    {
        throw std::invalid_argument("EnvSeg length cannot exceed max length!");
    }
    
    _len = sampleLen;
    
    _calcRate();
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
    
    // Necessary for setSegRate
    if (!_sample) _lastTick = _startLevel;
    
    _calcLevel(_startLevel,_endLevel);
    _calcRate();
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
    
    _calcLevel(_startLevel,_endLevel);
    _calcRate();
}

double EnvSeg::getEndLevel() const
{
    return _endLevel;
}

void EnvSeg::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
    
    _calcLevel(_startLevel,_endLevel);
}

void EnvSeg::setRate(double rate)
{
    if (rate > 2 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 2"); }
    
    _segRate = rate;
    
    _calcLevel(_lastTick, _endLevel);
    _calcRate();
}

double EnvSeg::getRate() const
{
    return _segRate;
}

double EnvSeg::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // (which shouldn't happen because the
    // envelope should move on), just return
    // the end amplitude. When the _inf (infinite)
    // variable is true, also return the end amplitude
    // (Caller must take care of stopping it)
    
    if (_sample++ >= _len){ return _lastTick; }
    
    // Increment the envelope value
    _segCurr *= _segIncr;
    
    _lastTick = _segCurr;
    
    if (_ads != SUST) _lastTick -= _segRate;
    
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
                           seg_t docksPerSeg,
                           bool modulationStartsAtSeg,
                           double amp)
: EnvSegSeq(numSegs),
  ModUnit(numSegs * docksPerSeg,amp),
  segsPerDock_(docksPerSeg),
  modulationStartsAtSeg_(modulationStartsAtSeg)
{ }

void ModEnvSegSeq::setModUnitDepth(seg_t segNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth)
{
    Unit::setModUnitDepth(getModIndex_(segNum, dockNum), modNum, depth);
}

void ModEnvSegSeq::attachMod(seg_t segNum,
                             index_t dockNum,
                             ModUnit *mod)
{
    Unit::attachMod(getModIndex_(segNum, dockNum), mod);
}

void ModEnvSegSeq::detachMod(seg_t segNum,
                             index_t dockNum,
                             index_t modNum)
{
    Unit::detachMod(getModIndex_(segNum, dockNum), modNum);
}

void ModEnvSegSeq::addDockforSegs_(seg_t numSegs)
{
    // Add numSegs new docks
    for (seg_t i = 0; i < numSegs; ++i)
    {
        _mods.push_back(new ModDock);
    }
    
    segsPerDock_.push_back(numSegs);
}

ModEnvSegSeq::seg_t ModEnvSegSeq::getModIndex_(seg_t seg, seg_t dock) const
{
    // Offset lookup position
    // Since the ModDocks are stored sequentially, we first add
    // the segment amount of each dock before the one we want
    // and then add the segment number requested
    // E.g. there are 3 segments and [0][1] are SEG_RATE (2 segments),
    // then [2][3][4] are SEG_LEVEL (3 segments). Say getModIndex_(1,1)
    // is called, then we first add the SEG_RATE segment number, in this
    // case 2, to the offset and then add the 'seg' parameter for
    // the requested dock, yielding index [2 + 1] = [3]
    
    seg_t offset = 0;
    
    for (seg_t i = 0; i < dock; ++i)
    {
        offset += segsPerDock_[i];
    }
    
    return offset + seg;
}