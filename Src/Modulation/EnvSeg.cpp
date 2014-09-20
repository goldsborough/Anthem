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
#include <cmath>

#include <sstream>

// 60 seconds
const unsigned long EnvSeg::_maxLen = 2646000;

EnvSeg::EnvSeg(double startAmp,
               double endAmp,
               EnvSeg::len_t len,
               double segR, int modW,
               double modDpth,
               unsigned char modR)
: _sample(0), _startLevel(startAmp), _endLevel(endAmp),
  _segRate(segR), _len(len), _modDepth(modDpth), _modRate(modR),
  _lfo(new LFO)
{
    _calcLevel();
    _calcRate();
    
    // initalize operator to waveform wanted
    setModWave(modW);
    
    _calcModRate();
}

EnvSeg::~EnvSeg() { delete _lfo; }

void EnvSeg::reset()
{
    _sample = 0;
    
    // reset _segCurr
    _calcRate();
}

void EnvSeg::_calcModRate()
{
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    
    // Since the rate is the cycles per segment
    // and not cycles per second, we get the
    // "period" of the segment and multiply that
    // by the rate, giving the mod wave's frequency.
    
    if (_len > 0)
    {
        double freq = (Global::samplerate / (double) _len) * _modRate;
        
        _lfo->setRate(freq);
    }
}

void EnvSeg::_calcLevel()
{
    // The range between start and end
    _diff = _endLevel - _startLevel;
    
    // set ADS
    
    if (_diff == 0) _ads = SUST;
    
    else if (_diff > 0)
    {
        _ads = ATK;
        _offset  = _startLevel;
    }
    
    else
    {
        _ads = DEC;
        
        // why do I have to do this ?
        // damn it, it solves the problem
        // who cares
        _diff = -_diff;
        
        _offset = _endLevel;
    }
}

void EnvSeg::_calcRate()
{
    
    // Set type
    
    if (_segRate == 1) _type = LIN;
    
    else if (_segRate < 1) _type = EXP;
    
    else
    {
        _type = LOG;
        
        // rate must still be between 0 and 1, the
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
        double base;
        double exp;
        
        _segCurr = _segRate;
        
        exp = (_len > 0) ? 1.0 / _len : 0;
        
        // Uses iterative exponential
        // calculations instead of
        // calling the pow function
        
        // All courtesy for this algorithm
        // goes to Daniel R. Mitchell, it is
        // truly great.
        
        // Basierend auf Pseudocode (vgl. Mitchell, 2008)
        
        // Attack
        // Or Decay, which must be inversed
        // if logarithmic type
        if ((_ads == ATK && _type != LOG) || (_ads == DEC && _type == LOG))
        {
            base = (1 + _segRate) / _segRate;
        }
        
        // Decay
        // Or Attack for Logarithmic curves
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
        // convert the _maxLen to string since
        // it might change...
        std::string error = "Length must be between 0 and ";
        
        std::stringstream ss;
        
        ss << _maxLen;
        
        error += ss.str();
        
        throw std::invalid_argument(error);
    }
    
    _len = sampleLen;
    
    _calcRate();
    _calcModRate();
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
    
    _calcLevel();
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
    
    _calcLevel();
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
}

void EnvSeg::setRate(double rate)
{
    if (rate > 2 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 2"); }
    
    _segRate = rate;
    
    _calcRate();
}

double EnvSeg::getRate() const
{
    return _segRate;
}

void EnvSeg::setModWave(int modW)
{
    _modWave = modW;
    
    // Do some phase offsetting so that the part with
    // full amplitude is at the beginning and end
    
    if (_modWave == WavetableDB::SINE || _modWave == WavetableDB::TRIANGLE)
    { _lfo->setPhaseOffset(90); }
    
    else if (_modWave == WavetableDB::SMOOTH_SQUARE)
    { _lfo->setPhaseOffset(215); }
    
    _lfo->setWavetable(_modWave);
}

void EnvSeg::setModDepth(double dpth)
{
    _lfo->setAmp(dpth);
}

double EnvSeg::getModDepth() const
{
    return _lfo->getAmp();
}

void EnvSeg::setModRate(unsigned short rate)
{
    _modRate = rate;
    
    _calcModRate();
}

double EnvSeg::getModRate() const
{
    return _lfo->getRate();
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
    
    if (_sample++ >= _len)
    { return _endLevel; }
    
    // Increment the envelope value
    _segCurr *= _segIncr;
    
    double ret = _segCurr;
    
    if (_ads != SUST) ret -= _segRate;
    
    if (_type == LOG) ret = 1 - ret;
    
    ret = (ret * _diff) + _offset;
    
    // Apply modulation oscillator
    if (_modWave != WavetableDB::NONE)
    {
        ret = _lfo->modulate(ret, 0, 1);
    }
    
    return ret;
}

EnvSegSeq::EnvSegSeq(EnvSegSeq::subseg_t seqLen)
: _segs(seqLen), _currSample(0), _loopCount(0),
  _loopMax(0), _loopInf(false), _lastTick(0)
{
    _loopStart = _loopEnd = _segs.end();
    
    _currSeg = _segs.begin();
}

void EnvSegSeq::setSegRate(subseg_t seg, double rate)
{
    _segs[seg].setRate(rate);
}

double EnvSegSeq::getSegRate(subseg_t seg) const
{
    return _segs[seg].getRate();
}


void EnvSegSeq::setSegStartLevel(subseg_t seg, double lv)
{
    _segs[seg].setStartLevel(lv);
}

double EnvSegSeq::getSegStartLevel(subseg_t seg) const
{
    return _segs[seg].getStartLevel();
}


void EnvSegSeq::setSegEndLevel(subseg_t seg, double lv)
{
    _segs[seg].setEndLevel(lv);
}

double EnvSegSeq::getSegEndLevel(subseg_t seg) const
{
    return _segs[seg].getEndLevel();
}


void EnvSegSeq::setSegLen(subseg_t seg, unsigned long ms)
{
    _segs[seg].setLen(ms * 44.1);
}

unsigned long EnvSegSeq::getSegLen(subseg_t seg) const
{
    return _segs[seg].getLen();
}


void EnvSegSeq::setSegBothLevels(subseg_t seg, double lv)
{
    setSegStartLevel(seg, lv), setSegEndLevel(seg, lv);
}


void EnvSegSeq::setSegModWave(subseg_t seg, int wavetableId)
{
    _segs[seg].setModWave(wavetableId);
}


void EnvSegSeq::setSegModDepth(subseg_t seg, double dpth)
{
    _segs[seg].setModDepth(dpth);
}


void EnvSegSeq::setSegModRate(subseg_t seg, unsigned short rate)
{
    _segs[seg].setModRate(rate);
}

double EnvSegSeq::getSegModRate(subseg_t seg) const
{
    return _segs[seg].getModRate();
}


void EnvSegSeq::setLoopStart(EnvSegSeq::subseg_t seg)
{
    _loopStart = _segs.begin() + seg;
}

EnvSegSeq::subseg_t EnvSegSeq::getLoopStart() const
{
    return _loopStart - _segs.begin();
}


void EnvSegSeq::setLoopEnd(EnvSegSeq::subseg_t seg)
{
    // Plus one because the end points to one after
    // the loop end segment
    _loopEnd = _segs.begin() + seg + 1;
}

EnvSegSeq::subseg_t EnvSegSeq::getLoopEnd() const
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