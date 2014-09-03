//
//  Oscillator.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Oscillator__
#define __Anthem__Oscillator__

#include "GenUnits.h"

class Oscillator : public AudioGenUnit
{
    
public:
    
    Oscillator(const int mode = -1, double frq = 1,
               double amp = 1, short phaseOffset = 0);
    
    double tick();
    
    void setFreq(double Hz);
    
    void setSemis(short semis, bool permanent = false);
    
    void setCents(short cents, bool permanent = false);
    
    void setPhaseOffset(short degrees);
    
    void reset() { _ind = _phaseOffset; }
    
private:
    
    double _freq;
    
    double _ind; // current WT index
    double _indIncr;
    
    double _phaseOffset;
};

#endif /* defined(__Anthem__Oscillator__) */
