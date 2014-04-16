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

class EnvSegSeq;
class Oscillator;
class XFadeUnit;

class LFOModeBase : public GenUnit
{
    
public:
    
    enum Units { A, B };
    
    typedef bool unit_t;
    typedef unsigned int subseg_t;
    
    LFOModeBase();
    
    virtual double first() = 0;
    
    virtual double second() = 0;
    
    virtual void setRate(unit_t unitNum, double Hz) = 0;
    
    virtual void setXFade(double value);
    
    virtual ~LFOModeBase();
    
protected:
    
    XFadeUnit * _xfade;
};

class LFOSeq : public LFOModeBase
{
    
public:
    
    LFOSeq();
    
    ~LFOSeq();
    
    // with crossfading
    double tick();
    
    // without crossfading, seq 0 value
    double first();
    
    // without crossfading, seq 1 value
    double second();
    
    void setWave(unit_t unitNum, subseg_t segNum, Wavetable::Modes wave);
    
    void setLevel(unit_t unitNum, subseg_t segNum, double lv);
    
    void setRate(unit_t subSeg, double Hz);
    
private:
    
    EnvSegSeq * _seqs[2];
    
    subseg_t _seqMaxCount;
    
};

class LFOWave : public LFOModeBase
{

public:
    
    enum Oscs { OscA, OscB };
    
    LFOWave();
    
    ~LFOWave();
    
    double tick();
    
    double first();
    
    double second();
    
    void setWave(unit_t unitNum, Wavetable::Modes wave);
    
    void setPhaseOffset(unit_t unitNum, unsigned short degrees);
    
    void setRate(unit_t unitNum, double Hz);
    
private:
    
    Oscillator * _oscs[2];

};

class LFO : public GenUnit
{
    
public:
    LFO();
    
    ~LFO();
    
    enum EnvSegs { SegA, SegB };
    
    enum EnvAmpPoints { A, B, C};
    
    void setMode(bool mode);
    
    void setEnvSegLen(bool envSeg, unsigned short len);
    
    void setEnvLevel(unsigned char point, double lv);
    
    void setEnvLoopMax(unsigned char loopNum);
    
    void setXFade(bool mode, double value);
    
    double tick();
    
    double first();
    
    double second();
    
    void setRate(bool unitNum, double Hz);
    
private:
    
    LFOSeq _lfoSeq;
    
    LFOWave _lfoWave;
    
    LFOModeBase& _currMode;
    
    EnvSegSeq * _env;
    
    XFadeUnit * _xfade;
};

#endif /* defined(__Synth__LFO__) */
