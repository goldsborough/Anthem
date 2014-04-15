//
//  GenUnits.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__GenUnits__
#define __Synth__GenUnits__

#include "Wavetable.h"

class GenUnit
{
    
public:
    
    virtual double tick() = 0;
    
    //virtual double * ticks(uint32_t amount) = 0;
    
    virtual void setAmp(const double& amp) { _Amp = amp; }
    
    virtual ~GenUnit() {}
    
protected:
    
    double _Amp = 1.0;
};

class AudioGenUnit : public GenUnit
{
    
public:
    
    virtual void setWT(const Wavetable::Modes& mode)
    {
        _mode = mode;
        _WT = wavetable.getWaveform(_mode);
    }
    
    virtual void setWT(double* wt)
    {
        _mode = Wavetable::USER;
        
        _WT = wt;
    }
    
    virtual ~AudioGenUnit() {}
    
protected:
    
    double * _WT = 0;
    
    Wavetable::Modes _mode;
};

#endif /* defined(__Synth__GenUnits__) */
