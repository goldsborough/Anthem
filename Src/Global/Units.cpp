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

void Unit::setDockMasterDepth(docknum_t dockNum, double depth)
{
    if (depth > 1 || depth < 0)
    { throw std::invalid_argument("Depth value must be between 0 and 1!");}
    
    _mods[dockNum]->setMasterDepth(depth);
}

void Unit::setDepth(docknum_t dockNum,
                       index_t modNum,
                       double depth)
{
    
    if (depth > 1 || depth < 0)
    { throw std::invalid_argument("Depth value must be between 0 and 1!");}
    
    _mods[dockNum]->setDepth(modNum, depth);
}

void Unit::attachMod(docknum_t dockNum,
                        index_t modNum,
                        GenUnit* mod)
{
    _mods[dockNum]->attach(modNum, mod);
}

void Unit::detachMod(docknum_t dockNum,
                        index_t modNum)
{
    _mods[dockNum]->detach(modNum);
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

void EffectUnit::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet level must be between 0 and 1!"); }
    
    _dw = dw;
}

double EffectUnit::_dryWet(double originalSample, double processedSample)
{
    return (originalSample * (1 - _dw)) + (processedSample * _dw);
}

double EffectUnit::_dryWet(double originalSample, double processedSample, double dryWet)
{
    return (originalSample * (1 - dryWet)) + (processedSample * dryWet);
}

void GenUnit::setAmp(const double& amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    _amp = amp;
}