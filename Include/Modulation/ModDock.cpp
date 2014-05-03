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

#include <iostream>

ModDock::ModDock(index_t dockSize)
: _mods(new ModItem [dockSize]), _dockSize(dockSize),
  _usedDocks(0), _masterDpth(1)
{ }

double ModDock::tick()
{
    double dp = 0.0;
    
    if (_usedDocks == 0)
        return dp;
    
    for (index_t i = 0; i < _dockSize; i++)
        if (_mods[i].mod)
            dp += _mods[i].mod->tick() * _mods[i].dpth;
    
    return dp / _usedDocks * _masterDpth;
}

double ModDock::checkAndTick(const double val,
                             const double base,
                             const double max)
{
    double t = val + (max * tick());
    
    if (t > max) t = max;
    
    else if (t < base) t = base;
    
    return t;
}

void ModDock::setDepth(index_t index, depth_t dpth)
{
    if (index < 0 || index >= _dockSize || ! _mods[index].mod)
        throw std::out_of_range("Invalid modDock index given!");
    
    else if (dpth < 0 || dpth > 1)
        throw std::out_of_range("Invalid depth, not between 0 and 1");
    
    _mods[index].dpth = dpth;
}

void ModDock::attach(index_t index, ModUnit* mod)
{
    if (index < 0 || index >= _dockSize)
        throw std::out_of_range("Invalid modDock index given!");
    
    if (! mod) _usedDocks--;
    
    else _usedDocks++;
    
    _mods[index].mod = mod;
}