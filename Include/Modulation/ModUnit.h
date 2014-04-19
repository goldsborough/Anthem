//
//  ModUnit.h
//  Vibe
//
//  Created by Peter Goldsborough on 17/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__ModUnit__
#define __Vibe__ModUnit__

#include "GenUnits.h"
#include <vector>
#include <map>

class ModUnit : public GenUnit
{ public: virtual ~ModUnit() { } };
/*
class ModDock
{
    
public:
    
    typedef unsigned short index_t;
    typedef double depth_t;
    
    ModDock(index_t modNum, double initVal = 0);
    
    double tick();
    
    void setValue(double value);
    
    void setDepth(index_t index, depth_t dpth);
    
    void addMod(const ModUnit& mod) { _mods.push_back(ModItem(mod)); }
    
    void remMod(index_t index) { _mods.erase(_mods.begin() + index); }
    
private:
    
    struct Depth
    {
        void setValue(double value)
        { left = value, right = value; }
        
        double left  = 0;
        double right = 0;
    };
    
    struct ModItem
    {
        ModItem(const ModUnit& m) : mod(m) {}
 
        ModItem(const ModItem& other)
        : mod(other.mod)
        {
            if (&other != this)
                dpth = other.dpth;
        }
        
        void operator= (const ModItem& other)
        {
            if (&other != this)
            {
                mod = other.mod;
                dpth = other.dpth;
            }
        }
 
        const ModUnit& mod;
        Depth dpth;
        
        
    };
    
    typedef std::vector<ModItem> modVec;
    
    void _checkDpth(index_t index);
    
    void _checkAllDpths();
    
    // can't use map because I need indexing
    modVec _mods;
    
    double _value;
};
*/
#endif /* defined(__Vibe__ModUnit__) */
