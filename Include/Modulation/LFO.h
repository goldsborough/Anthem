//
//  LFO.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__LFO__
#define __Synth__LFO__

#include "GenUnits.h"
// Vector comes from Wavetable.h

class EnvSegSeq;
//class Oscillator;
class XFadeUnit;

class LFOSeq : public GenUnit
{
    enum Units { A, B };
    
    LFOSeq();
    
    ~LFOSeq();
    
    double tick();
    
    void setWave(int unitNum, int segNum, Wavetable::Modes wave);
    
    void setLevel(int unitNum, int segNum, double lv);
    
    void setRate(int unitNum, double hertz);
    
    void setXFade(double xfade);
    
private:
    
    EnvSegSeq * _seqs[2];
    
    XFadeUnit * _xfade;
    
    int _seqLen;
    
};

class LFOWave : public GenUnit
{

    
    
};

class LFO : public GenUnit
{
private:
    
    //LFOSeq _lfoSeq;
    
    //LFOWave _lfoWave;
};

#endif /* defined(__Synth__LFO__) */
