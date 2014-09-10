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

EnvSeg::EnvSeg(double startAmp, double endAmp, unsigned long len,
               double segR, int modW, double modDpth,
               unsigned char modR)
{
    _sample = 0;
    
    _startLevel = startAmp;
    _endLevel = endAmp;
    
    _segRate = segR;
    
    _len = len;
    
    _modDepth = modDpth;
    _modRate = modR;
    
    _calcLevel();
    _calcRate();
    
    _lfo = new LFO;
    
    // initalize operator to waveform wanted
    setModWave(modW);
    
    _calcModRate();
}

EnvSeg::~EnvSeg() { delete _lfo; }

void EnvSeg::_calcModRate()
{
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    
    // Then multiply by the modulation rate,
    // so the non-base frequency of the
    // modulation wave
    
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
    
    // set ads
    
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
    
    // set type
    
    if (_segRate == 1) _type = LIN;
    else if (_segRate < 1) _type = EXP;
    else
    {
        _type = LOG;
        
        // rate must still be between 0 and 1, the
        // 1 - 2 range is just for covenience
        
        // Just as 0 is the most exponential, 2 should
        // be the most logarithmic, so subtracting one from
        // _segRate is not enough, since 2 - 1 would be 1, which
        // is practically linear. So this value must then be subtracted
        // from by one, to give the inverse. 1.2 - 1 = 0.2 -> 1 - 0.2 = 0.8
        
        _segRate = 1 - (_segRate - 1);
    }
    
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
        
        if (_len > 0)
            exp = 1.0 / _len;
        
        else exp = 0;
        
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

void EnvSeg::setLen(unsigned long sampleLen)
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

void EnvSeg::setStartLevel(double lv)
{
    if (lv > 1 || lv < 0)
        throw std::invalid_argument("Level must be between 0 and 1");
    
    _startLevel = lv;
    
    _calcLevel();
    _calcRate();
}

void EnvSeg::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
        throw std::invalid_argument("Level must be between 0 and 1");
    
    _endLevel = lv;
    
    _calcLevel();
    _calcRate();
}

void EnvSeg::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
}

void EnvSeg::setRate(double rate)
{
    if (rate > 2 || rate < 0)
        throw std::invalid_argument("Rate must be between 0 and 2");
    
    _segRate = rate;
    
    _calcRate();
}

void EnvSeg::setModWave(int modW)
{
    _modWave = modW;
    
    // Do some phase offsetting so that the part with
    // full amplitude is at the beginning and end
    
    if (_modWave == WavetableDB::SINE || _modWave == WavetableDB::TRIANGLE)
        _lfo->osc.setPhaseOffset(90);
    
    else if (_modWave == WavetableDB::SMOOTH_SQUARE)
        _lfo->osc.setPhaseOffset(215);
    
    _lfo->osc.setWavetable(_modWave);
}

void EnvSeg::setModDepth(double dpth)
{
    // Go von deepar
    
    if (dpth > 1 || dpth < 0)
        throw std::invalid_argument("Modulation depth must be betwee 0 and 1");
    
    _modDepth = dpth;
}

void EnvSeg::setModRate(unsigned char rate)
{
    // data type used takes care of range
    // 0 - 255
    
    _modRate = rate;
    
    _calcModRate();
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
    
    _segCurr *= _segIncr;
    
    double ret = _segCurr;
    
    if (_ads != SUST) ret -= _segRate;
    
    if (_type == LOG) ret = 1 - ret;
    
    ret = (ret * _diff) + _offset;
    
    // Apply modulation oscillator
    if (_modWave != WavetableDB::NONE)
    {
        // modulated value
        double modValue = ret + _lfo->tick();
        
        // difference between modulated and unmodulated
        double diff = modValue - ret;
        
        // control how much change is applied
        ret += diff * _modDepth;
    }
    
    return ret;
}

EnvSegSeq::EnvSegSeq(unsigned int seqLen)
: _segs(seqLen), _seqLen(seqLen)
{
    _currSample = 0;
    
    _currSegLen = 0;
    
    _currSegNum = 0;
    
    _currSeg = (seqLen > 0) ? &_segs[0] : 0;
    
    _loopStart = -1;
    _loopEnd   = -1;
    
    _loopCount = 0;
    
    _loopMax = 0;
    
    _loopInf = false;
    
    _oneShot = false;
    
    _lastTick = 0;
    
}

void EnvSegSeq::setSegLen(subseg_t seg, unsigned int ms)
{
    double inSamples = ms * 44.1;
    
    _segs[seg].setLen(inSamples);
    
    if (seg == _currSegNum)
        _currSegLen = inSamples;
}

void EnvSegSeq::_changeSeg(subseg_t seg)
{
    _currSeg = &_segs[_currSegNum];
    
    _currSample = 0;
    
    _currSegLen = _currSeg->getLen();
}

void EnvSegSeq::_resetLoop()
{
    // reset all the loop segments
    for (subseg_t i = _loopEnd; i > _loopStart; i--)
        _segs[i].reset();
}

double EnvSegSeq::tick()
{
    
    if (_currSample++ >= _currSegLen)
    {
        // Check if we need to reset the loop
        if (_currSegNum == _loopEnd &&
                 ( _loopInf || _loopCount++ < _loopMax) &&
                 ! _oneShot)
            
            _resetLoop();
        
        // send out the last good value if no more segments left
        else if (_currSegNum + 1 >= _segs.size())
            return _lastTick;
        
        // else increment segment
        else
        {
            _changeSeg(++_currSegNum);
            
            if (_currSegLen == 0)
                return _lastTick;
        }
        
    }

    _lastTick =  _currSeg->tick();
    
    return _lastTick;
    
}

void EnvSegSeq::setLoopMax(unsigned char n)
{
    if (n > 64)
        _loopInf = true;
    else
    {
        _loopInf = false;
        _loopMax = n;
    }
};

void EnvSegSeq::setLoopStart(subseg_t seg)
{
    _loopStart = seg;
    
    if (_loopStart > _loopEnd)
        _loopEnd = _loopStart;
}

void EnvSegSeq::setLoopEnd(subseg_t seg)
{
    _loopEnd = seg;
    
    if (_loopEnd < _loopStart)
        _loopStart = _loopEnd;
}