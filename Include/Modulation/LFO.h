//
//  LFO.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__LFO__
#define __Synth__LFO__

#include "ModUnit.h"
#include "Oscillator.h"
#include "EnvSeg.h"

class EnvSegSeq;
class Oscillator;
class XFadeUnit;

struct LFO : public ModUnit
{
    LFO(const int wave = -1)
    : osc(wave)
    { }
    
    double tick(){ return osc.tick(); }
    
    double scaledTick() { return (osc.tick() + 1) / 2; }
    
    void setRate(double Hz) { osc.setFreq(Hz); }
    
    Oscillator osc;
};

struct LFOSeq : public ModUnit
{
    LFOSeq(unsigned int seqLength = 20);
    
    double tick() { return seq.tick(); };
    
    void setRate(double Hz);
    
    EnvSegSeq seq;
    
private:
    
    const unsigned int _seqLen;
};

class LFOUnit : public ModUnit
{
    
public:
    
    enum Modes { LFO, SEQ };
    
    enum Units { A, B };
    
    enum EnvSegs { SegA, SegB };
    
    enum EnvAmpPoints { BEG, MID, END};
    
    typedef bool unit_t;
    typedef unsigned int subseg_t;
    
    LFOUnit();
    
    ~LFOUnit();
    
    
    double tick();
    
    void setMode(unit_t mode) { _mode = mode; }
    
    
    void setEnvSegLen(unit_t envSeg, unsigned short len);
    
    void setEnvLevel(unsigned char point, double lv);
    
    void setEnvLoopMax(unsigned char loopNum);
    
    
    void setLFOWave(unit_t unitNum, int wave) { _LFOs[unitNum].osc.setWT(wave); }
    
    void setLFOPhaseOffset(unit_t unitNum, short degrees) { _LFOs[unitNum].osc.setPhaseOffset(degrees); };
    
    
    void setLFOSeqSegWave(unit_t unitNum, subseg_t segNum, int wave);
    
    void setLFOSeqSegAmp(unit_t unitNum, subseg_t seg, double amp);
    
    void setLFOSeqLoopStart(unit_t unitNum, subseg_t segNum);
    
    void setLFOSeqLoopEnd(unit_t unitNum, subseg_t segNum);
    
    
    void setXFade(char value);
    
    void setRate(unit_t unitNum, double Hz);
    
private:
    
    
    struct LFOSeq _LFOSeqs[2];
    
    struct LFO _LFOs[2];
    
    unit_t _mode;
    
    EnvSegSeq * _env;
    
    XFadeUnit * _xfade;
};

#endif /* defined(__Synth__LFO__) */
