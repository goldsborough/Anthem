//
//  ModUnit.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 17/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "ModDock.h"
#include "GenUnits.h"
#include <stdexcept>

ModDock::ModDock(index_t dockSize)
: _mods(new ModItem [dockSize]), _dockSize(dockSize),
  _usedDocks(0), _masterDpth(1)
{ }

double ModDock::tick()
{
    double dp = 0.0;
    
    // if not in use, no need to tick
    if (_usedDocks == 0)
        return dp;
    
    // gather all ticks from all modulation sources and
    // add them together
    for (index_t i = 0; i < _dockSize; i++)
        if (_mods[i].mod)
            dp += _mods[i].mod->tick() * _mods[i].dpth;
    
    // get average modulation value and multiply
    // by master depth
    return (dp / _usedDocks) * _masterDpth;
}

double ModDock::checkAndTick(const double val,
                             const double base,
                             const double max)
{
    // get the offset value
    double t = val + (max * tick());
    
    // do boundary checking
    if (t > max) t = max;
    
    else if (t < base) t = base;
    
    return t;
}

void ModDock::setDepth(index_t index, depth_t dpth)
{
    if (index < 0 || index >= _dockSize || ! _mods[index].mod)
        throw std::invalid_argument("Invalid modDock index given!");
    
    else if (dpth < 0 || dpth > 1)
        throw std::invalid_argument("Invalid depth, not between 0 and 1");
    
    _mods[index].dpth = dpth;
}

void ModDock::attach(index_t index, ModUnit* mod)
{
    if (index < 0 || index >= _dockSize)
        throw std::invalid_argument("Invalid modDock index given!");
    
    if (! mod) _usedDocks--;
    
    else _usedDocks++;
    
    _mods[index].mod = mod;
}