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

ModDock::ModDock()
: _masterDepth(1)
{ }

ModDock::ModDock(double lowerBoundary,
                 double higherBoundary,
                 double baseValue,
                 double masterDepth)
: _lowerBoundary(lowerBoundary), _higherBoundary(higherBoundary),
  _baseValue(baseValue)
{
    setMasterDepth(masterDepth);
}

void ModDock::setBaseValue(double baseValue)
{
    _baseValue = baseValue;
}

double ModDock::getBaseValue() const
{
    return _baseValue;
}

void ModDock::setLowerBoundary(double lowerBoundary)
{
    _lowerBoundary = lowerBoundary;
}

double ModDock::getLowerBoundary() const
{
    return _lowerBoundary;
}

void ModDock::setHigherBoundary(double higherBoundary)
{
    _higherBoundary = higherBoundary;
}

double ModDock::getHigherBoundary() const
{
    return _higherBoundary;
}

void ModDock::setMasterDepth(double depth)
{
    if (depth < 0 || depth > 1)
    { throw std::invalid_argument("Depth value must be between 0 and 1!"); }
    
    _masterDepth = depth;
}

double ModDock::getMasterDepth() const
{
    return _masterDepth;
}

bool ModDock::inUse() const
{
    return (_masterDepth > 0 && ! _mods.empty());
}

double ModDock::tick()
{
    return modulate(_baseValue);
}

double ModDock::modulate(double sample)
{
    // If ModDock is not in use, return original sample immediately
    if (! inUse()) return sample;

    double temp = sample;
    
    // Apply all modulation
    for (std::vector<ModItem>::const_iterator itr = _mods.begin(), end = _mods.end();
         itr != end;
         ++itr)
    {
        // Get modulated signal
        temp = itr->mod->modulate(temp, itr->depth, _higherBoundary);
    }
    
    // Apply master depth like dry/wet
    sample = (sample * (1 - _masterDepth)) + (temp * _masterDepth);
    
    if (sample > _higherBoundary) { sample = _higherBoundary; }
    
    else if (sample < _lowerBoundary) { sample = _lowerBoundary; }
    
    return sample;
}

void ModDock::setDepth(index_t index, double depth)
{
    if (index >= _mods.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    if (depth < 0 || depth > 1)
    { throw std::invalid_argument("Invalid depth value, must be between 0 and 1!"); }
    
    _mods[index].depth = depth;
}

double ModDock::getDepth(index_t index) const
{
    if (index >= _mods.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    return _mods[index].depth;
}

void ModDock::attach(ModUnit* mod)
{
    _mods.push_back(ModItem(mod));
}

void ModDock::detach(index_t index)
{
    _mods.erase(_mods.begin() + index);
}

unsigned long ModDock::size() const
{
    return _mods.size();
}