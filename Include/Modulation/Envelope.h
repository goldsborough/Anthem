//
//  Envelope.h
//  Anthem
//
//  Created by Peter Goldsborough on 09/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Envelope__
#define __Anthem__Envelope__

#include "EnvSeg.h"

class Envelope : public EnvSegSeq
{
public:
    
    /*
     INSERT DESCRIPTION PLZ
     
     
           /\         /\
          /  \_______/  \
     ____/               \
     DLY ATK A    B   C REL
     
     
     */
    
    enum Segment
    {
        ATK,
        A,
        B,
        C,
        REL
    };
    
    Envelope(unsigned int delay_MS = 0);
    
    double tick();
    
    // in ms
    void setDelay(unsigned int millis)
    {
        if (_currSegNum == DEL)
            _currSegLen = 44.1 * millis;
    }
    
    void setSegLevel(int seg, double lv);
    
    void setLoopStart(int seg);
    void setLoopEnd(int seg);
    
    void noteOff() { _changeSeg(REL); };
    
private:
    
    enum HiddenSegment
    {
        NONE = -3,
        CONNECTOR = -2,
        DEL = -1,
    };
    
    void _changeSeg(int seg);
    
    void _resetLoop();
    
    unsigned int _hiddenConnecterLen;
    
    // Connects the end and starting points of the loop
    // to avoid harsh transitions
    EnvSeg _hiddenConnect;
    
};

#endif /* defined(__Anthem__Envelope__) */
