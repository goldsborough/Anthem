//
//  Envelope.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 09/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Envelope.h"
#include "Global.h"

Envelope::Envelope(unsigned int delay_MS)
                   : EnvSegSeq(5)
{
    _currSegLen = delay_MS * 44.1;
    
    _currSegNum = DEL;
    
    // Found this to be a good and not too long length.
    // For 44100 this is 1102.5 ~ 1102 samples
    _hiddenConnecterLen = Global::samplerate / 40;
    
    _loopStart = NONE;
    _loopEnd   = NONE;

    setSegLevel(Envelope::ATK, 0.8);
    setSegLen(Envelope::ATK, 500);
    
    setSegLevel(Envelope::A, 0.5);
    setSegLen(Envelope::A, 250);
    setSegRate(Envelope::A, 0.1);

    setSegRate(Envelope::REL, 0.1);
    setSegLen(Envelope::REL, 500);
    
    _hiddenConnect.setLen(_hiddenConnecterLen);
}

void Envelope::_changeSeg(int seg)
{
    _currSegNum = seg;
    
    _currSeg = &_segs[_currSegNum];
    
    _currSample = 0;
    
    _currSegLen = _currSeg->getLen();

    if (_currSegNum == REL)
        _currSeg->setStartLevel(_lastTick);
    
}

void Envelope::_resetLoop()
{
    // reset all the loop segments
    for (int i = _loopStart; i <= _loopEnd; i++)
        _segs[i].reset();
    
    // no need for connecting segment if the amplitude
    // is the same e.g. for the constant sustain
    if (_lastTick == _segs[_loopStart].getStartLevel())
        _changeSeg(_loopStart);
    
    else
    {
        _currSegNum = CONNECTOR;
        
        _hiddenConnect.reset();
        
        _currSeg = &_hiddenConnect;
        
        _currSample = 0;
        
        _currSegLen = _hiddenConnecterLen;
    }
}

double Envelope::tick()
{

    if (_currSample++ >= _currSegLen)
    {

        if (_currSegNum == REL)
            return 0;
        

        if (_currSegNum == CONNECTOR)
            _changeSeg(_loopStart);
        
        else if (_currSegNum == _loopEnd &&
                 ( _loopInf || _loopCount++ < _loopMax) &&
                 ! _oneShot)
            
            _resetLoop();
        
        else if (_currSegNum == C && !_oneShot)
            return _lastTick;
        else
        {
            _changeSeg(++_currSegNum);
            
            if (_currSegLen == 0)
                return _lastTick;
        }

    }
    
    if (_currSegNum == DEL)
        return 0;
    
    _lastTick =  _currSeg->tick();
    
    return _lastTick;

}

void Envelope::setLoopStart(int seg)
{
    
    if (seg >= REL || seg <= ATK)
        throw std::invalid_argument("Invalid loop segment, can only loop from Segments A to D!");
    
    _loopStart = seg;

     _hiddenConnect.setEndLevel(_segs[_loopStart].getStartLevel());

    if (_loopStart > _loopEnd)
        _loopEnd = _loopStart;
}

void Envelope::setLoopEnd(int seg)
{
    if (seg >= REL || seg <= ATK)
        throw std::invalid_argument("Invalid loop segment, can only loop from Segments A to D!");
    
    _loopEnd = seg;
    
    _hiddenConnect.setStartLevel(_segs[_loopEnd].getEndLevel());
    
    if (_loopEnd < _loopStart)
        _loopStart = _loopEnd;
}

void Envelope::setSegLevel(int seg, double lv)
{
    if (seg >= REL || seg <= DEL)
        throw std::invalid_argument("Invalid segment for setting the level!");
    
    _segs[seg].setEndLevel(lv);
    _segs[seg + 1].setStartLevel(lv);
    
}