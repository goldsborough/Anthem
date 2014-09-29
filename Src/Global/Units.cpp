//
//  GenUnits.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Units.h"
#include "ModDock.h"
#include "Wavetable.h"

#include <stdexcept>

Unit::Unit(index_t numDocks)
: _mods(numDocks)
{
    for (index_t i = 0; i < numDocks; ++i)
    {
        _mods[i] = new ModDock;
    }
}

Unit::~Unit()
{
    for (std::vector<ModDock*>::iterator itr = _mods.begin(), end = _mods.end();
         itr != end;
         ++itr)
    {
        delete *itr;
    }
}

std::vector<ModDock*>::size_type Unit::numDocks() const
{
    return _mods.size();
}

void Unit::setDockMasterDepth(index_t dockNum, double depth)
{
    if (depth > 1 || depth < 0)
    { throw std::invalid_argument("Master depth value must be between 0 and 1!"); }
    
    _mods[dockNum]->setMasterDepth(depth);
}

double Unit::getDockMasterDepth(index_t dockNum) const
{
    return _mods[dockNum]->getMasterDepth();
}

void Unit::setModUnitDepth(index_t dockNum,
                           index_t modNum,
                           double depth)
{
    if (depth > 1 || depth < 0)
    { throw std::invalid_argument("Depth value must be between 0 and 1!");}
    
    _mods[dockNum]->setDepth(modNum, depth);
}

double Unit::getModUnitDepth(index_t dockNum, index_t modNum) const
{
    return _mods[dockNum]->getDepth(modNum);
}

void Unit::attachMod(index_t dockNum,
                     ModUnit* mod)
{
    _mods[dockNum]->attach(mod);
}

void Unit::detachMod(index_t dockNum,
                     index_t modNum)
{
    _mods[dockNum]->detach(modNum);
}

void EffectUnit::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet level must be between 0 and 1!"); }
    
    _dw = dw;
}

double EffectUnit::getDryWet() const
{
    return _dw;
}

double EffectUnit::_dryWet(double originalSample, double processedSample)
{
    return (originalSample * (1 - _dw)) + (processedSample * _dw);
}

double EffectUnit::_dryWet(double originalSample, double processedSample, double dryWet)
{
    return (originalSample * (1 - dryWet)) + (processedSample * dryWet);
}

GenUnit::GenUnit(double amp, unsigned short dockNum)
: Unit(dockNum)
{
    setAmp(amp); 
}

void GenUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    _amp = amp;
}

double GenUnit::getAmp() const
{
    return _amp;
}

ModUnit::ModUnit(double amp , unsigned short dockNum)
: Unit(dockNum)
{
    setAmp(amp);
}

void ModUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    _amp = amp;
}

double ModUnit::getAmp() const
{
    return _amp;
}