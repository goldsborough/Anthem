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

class Envelope : public EnvSegSeq, public ModUnit
{
public:

    typedef unsigned short seg_t;
    
    enum Docks
    {
        DELAY,
        
    };

    enum Segment
    {
        DEL = 1,
        ATK = 2,
        SEG_A = 3,
        SEG_B = 4,
        SEG_C = 5,
        REL = 6
    };
    
    Envelope(unsigned int delayMillis = 0,
             bool sustainInf = true);
    
    double modulate(double sample,
                    double depth,
                    double,
                    double);
    
    void setDelay(unsigned int millis);
    
    void setSegLevel(seg_t seg, double lv);
    
    void setLoopStart(seg_t seg);
    void setLoopEnd(seg_t seg);
    
    void noteOff();
    
    void setSustainInf(bool sustainInf);
    
    bool sustainInf() const;
    
private:
    
    /*! Connects the end and starting poseg_ts of the loop
        to avoid harsh transitions */
    enum { CONNECTOR };
    
    /*! Changes the current segment in the sequence */
    void _changeSeg(segItr itr);
    
    double _tick();
    
    void _resetLoop();
    
    bool _sustainInf;
    
    seg_t _currSegNum;
};

#endif /* defined(__Anthem__Envelope__) */
