//
//  GenUnits.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "GenUnits.h"
#include "ModDock.h"
#include "Wavetable.h"
#include <stdexcept>

void GenUnit::setDockMasterDepth(docknum_t dockNum, double lvl)
{ _mods[dockNum]->setMasterDepth(lvl); }

void GenUnit::setDepth(docknum_t dockNum,
                       index_t modNum,
                       double dpth)
{
    
    if (dpth > 1 || dpth < 0)
    { throw std::invalid_argument("Depth must be between 0 and 1!");}
    
    _mods[dockNum]->setDepth(modNum, dpth);
}

void GenUnit::attachMod(docknum_t dockNum,
                        index_t modNum,
                        ModUnit* mod)
{
    _mods[dockNum]->attach(modNum, mod);
}

void GenUnit::detachMod(docknum_t dockNum,
                        index_t modNum)
{
    _mods[dockNum]->detach(modNum);
}

GenUnit::~GenUnit()
{
    for (std::vector<ModDock*>::iterator itr = _mods.begin(), end = _mods.end();
         itr != end;
         ++itr)
    {
        delete *itr;
    }
}

void AudioGenUnit::setWT(const int mode)
{
    _mode = mode;
    _WT = wavetableDB.getWaveform(_mode);
}