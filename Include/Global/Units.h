//
//  Units.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__GenUnits__
#define __Anthem__GenUnits__

#include <vector>
#include "Wavetable.h"

class ModDock;
class GenUnit;

class Unit
{
    
public:
    
    typedef unsigned char docknum_t;
    typedef unsigned short index_t;
    
    virtual void setAmp(const double& amp) { _amp = amp; }
    
    virtual void setDockMasterDepth(docknum_t dockNum, double lvl);
    
    virtual void setDepth(docknum_t dockNum,
                          index_t modNum,
                          double dpth);
    
    virtual void attachMod(docknum_t dockNum,
                           index_t modNum,
                           GenUnit* mod);
    
    virtual void detachMod(docknum_t dockNum,
                           index_t modNum);
    
    virtual ~Unit() { }
    
protected:
    
    std::vector<ModDock*> _mods;
    
    double _amp = 1.0;
};

class EffectUnit : public Unit
{
public:
    
    EffectUnit(const double& dryWet = 1)
    : _dw(dryWet)
    { }
    
    virtual ~EffectUnit() { }
    
    /************************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The new sample.
    *
    *************************************************************************************************/
    
    virtual double process(double sample) = 0;
    
    virtual void setDryWet(const double& dw);
    
protected:
    
    double _dryWet(const double& originalSample, const double& processedSample);
    
    double _dw;
};

class GenUnit : public Unit
{
    
public:
    
    virtual double tick() = 0;
    
    virtual ~GenUnit() { }
};

class AudioGenUnit : public GenUnit
{
public:
    
    virtual void setWT(const int mode);
    
    virtual ~AudioGenUnit() { }
    
protected:
    
    Wavetable _WT;
    
    int _mode;
};

#endif /* defined(__Anthem__GenUnits__) */
