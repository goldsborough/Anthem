//
//  Oscillator.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Oscillator__
#define __Synth__Oscillator__

#include "GenUnits.h"

class Oscillator : public AudioGenUnit
{
    
public:
    
    
    Oscillator(const Wavetable::Modes& mode = Wavetable::NONE, double frq = 1,
               double amp = 1, short phaseOffset = 0);
    
    double tick();
    
    void setFreq(double frq);
    
    void setFreqOffset(double offs) { setFreq(_freq + offs); }
    
    void setPhaseOffset(int16_t degrees);
    
    void reset() { _ind = _phaseOffset; }
    
private:
    
    double _freq;
    
    double _ind; // current WT index
    double _ind_incr;
    
    double _phaseOffset;
};

#endif /* defined(__Synth__Oscillator__) */
