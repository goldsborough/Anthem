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
        SEG_RATE,
        MOD_RATE,
        MOD_DEPTH,
        AMP = 15
    };

    enum Segment
    {
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
    
    void setModUnitDepth(seg_t segNum,
                         index_t dockNum,
                         index_t modNum,
                         double depth);

    
    void attachMod(seg_t segNum,
                   index_t dockNum,
                   ModUnit* mod);
    
    void detachMod(seg_t segNum,
                   index_t dockNum,
                   index_t modNum);
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      seg The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *********************************************************************************/
    
    void setSegRate(seg_t seg, double rate);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the modulation depth of a segment's mod wave.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       dpth The new depth value, between 0 and 1.
    *
    *********************************************************************************/
    
    void setSegModDepth(seg_t seg, double dpth);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets the modulation rate of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       rate The new rate, not that this is cycles per segment and not per second.
    *
    ********************************************************************************************/
    
    void setSegModRate(seg_t seg, unsigned short rate);
    
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
    enum { CONNECTOR, DEL = 1 };
    
    /*! Changes the current segment in the sequence */
    void _changeSeg(segItr itr);
    
    double _tick();
    
    void _resetLoop();
    
    bool _sustainInf;
    
    seg_t _currSegNum;
    
    double _currSegRate;
    
    double _currSegModRate;
    
    double _currSegModDepth;
};

#endif /* defined(__Anthem__Envelope__) */
