//
//  GenUnits.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__GenUnits__
#define __Synth__GenUnits__

#include <vector>

class ModDock;
class ModUnit;

class GenUnit
{
    
public:
    
    typedef const unsigned char docknum_t;
    typedef const unsigned short index_t;
    
    virtual double tick() = 0;
    
    virtual void setAmp(const double& amp) { _amp = amp; }
    
    virtual void setDockMasterDepth(docknum_t dockNum, double lvl);
    
    virtual void setDepth(docknum_t dockNum,
                          index_t modNum,
                          double dpth);
    
    virtual void attachMod(docknum_t dockNum,
                           index_t modNum,
                           ModUnit* mod);
    
    virtual void detachMod(docknum_t dockNum,
                           index_t modNum);
    
    virtual ~GenUnit();
    
protected:
    
    std::vector<ModDock*> _mods;
    
    double _amp = 1.0;
};

class AudioGenUnit : public GenUnit
{
    
public:
    
    virtual void setWT(const int mode);
    
    virtual ~AudioGenUnit() { }
    
protected:
    
    double * _WT = 0;
    
    int _mode;
};

class ModUnit : public GenUnit
{ public: virtual ~ModUnit() { } };

#endif /* defined(__Synth__GenUnits__) */
