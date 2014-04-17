//
//  ModUnit.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 17/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "ModUnit.h"

ModDock::ModDock(index_t modNum, double initVal)
: _mods(modNum), _value(initVal)
{ }

double ModDock::tick()
{
    
}

void ModDock::_checkDpth(index_t index)
{
    ModItem& mod = _mods[index];
    
    if (mod.dpth.left + _value > 1)
        mod.dpth.left = 1 - _value;
    
    else if (mod.dpth.right + _value > 1)
        mod.dpth.right = 1 - _value;
}

void ModDock::_checkAllDpths()
{
    for (int i = 0; i < _mods.size(); i++)
        _checkDpth(i);
}

void ModDock::setValue(double value)
{
    _value = value;
    
    _checkAllDpths();
}

void ModDock::setDepth(index_t index, depth_t dpth)
{
    _mods[index].dpth.setValue(dpth);
    
    _checkDpth(index);
}