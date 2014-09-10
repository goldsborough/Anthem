//
//  ModUnit.h
//  Anthem
//
//  Created by Peter Goldsborough on 17/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__ModUnit__
#define __Anthem__ModUnit__

class GenUnit;

class ModDock
{
    
public:
    
    typedef unsigned short index_t;
    typedef double depth_t;
    
    ModDock(index_t dockSize);

    ~ModDock() { delete [] _mods; }
    
    double tick();
    
    double checkAndTick(const double val,
                        const double base,
                        const double max);
    
    void setMasterDepth(depth_t dpth)
    { _masterDpth = dpth; }
    
    bool inUse()
    { return (_usedDocks > 0 && _masterDpth > 0); }
    
    void setDepth(index_t index, depth_t dpth);
    
    void attach(index_t index, GenUnit* mod);
    
    void detach(index_t index)
    { attach(index, 0); }
    
private:
    
    struct ModItem
    {
        GenUnit* mod = 0;
        depth_t dpth = 1;
    };

    ModItem *  _mods;
    
    index_t _dockSize;
    index_t _usedDocks;
    
    depth_t _masterDpth;
};

#endif /* defined(__Anthem__ModUnit__) */
