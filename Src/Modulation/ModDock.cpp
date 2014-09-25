//
//  ModUnit.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 17/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "ModDock.h"
#include "Units.h"

#include <stdexcept>

ModDock::ModDock(double masterDepth)
{
    setMasterDepth(masterDepth);
}

void ModDock::setMasterDepth(double depth)
{
    if (depth < 0 || depth > 1)
    { throw std::invalid_argument("Depth value must be between 0 and 1!"); }
    
    _masterDepth = depth;
}

bool ModDock::inUse() const
{
    return (_masterDepth > 0 && ! _mods.empty());
}

double ModDock::modulate(double sample,
                         double minBoundary,
                         double maxBoundary)
{
    // If ModDock is not in use, return original sample immediately
    if (! inUse()) return sample;

    // Apply all modulation
    for (std::vector<ModItem>::const_iterator itr = _mods.begin(), end = _mods.end();
         itr != end;
         ++itr)
    {
        // Get modulated signal
        sample = itr->mod->modulate(sample, itr->depth, minBoundary, maxBoundary);
    }
    
    return sample * _masterDepth;
}

void ModDock::setDepth(index_t index, double depth)
{
    if (index >= _mods.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    _mods[index].depth = depth;
}

void ModDock::attach(ModUnit* mod)
{
    _mods.push_back(ModItem(mod));
}

void ModDock::detach(index_t index)
{
    _mods.erase(_mods.begin() + index);
}