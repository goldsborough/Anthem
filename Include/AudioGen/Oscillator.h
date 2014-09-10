//
//  Oscillator.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Oscillator__
#define __Anthem__Oscillator__

#include "Units.h"

class Oscillator : public GenUnit
{
    
public:
    
    Oscillator(const unsigned short& wt = -1, const double& frq = 1,
               const double& amp = 1, const short& phaseOffset = 0);
    
    double tick();
    
    void setFreq(const double& Hz);
    
    void setSemis(const short& semis, bool permanent = false);
    
    void setCents(const short& cents, bool permanent = false);
    
    void setPhaseOffset(short degrees);
    
    void setWavetable(const unsigned short& wt);
    
    void reset() { _ind = _phaseOffset; }
    
private:
    
    double _freq;
    
    double _ind; // current WT index
    double _indIncr;
    
    double _phaseOffset;
    
    Wavetable _wt;
};

#endif /* defined(__Anthem__Oscillator__) */
