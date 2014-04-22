//
//  GenUnits.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__GenUnits__
#define __Synth__GenUnits__

class GenUnit
{
    
public:
    
    virtual double tick() = 0;
    
    virtual void setAmp(const double& amp) { _amp = amp; }
    
    virtual ~GenUnit() {}
    
protected:
    
    double _amp = 1.0;
};

class AudioGenUnit : public GenUnit
{
    
public:
    
    virtual void setWT(const int mode);
    
    virtual ~AudioGenUnit() {}
    
protected:
    
    double * _WT = 0;
    
    int _mode;
};

#endif /* defined(__Synth__GenUnits__) */
