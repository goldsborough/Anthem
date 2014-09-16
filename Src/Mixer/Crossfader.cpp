//
//  Crossfade.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Crossfader.h"
#include "Global.h"
#include "ModDock.h"

#include <stdexcept>
#include <cmath>

CrossfadeUnit::CrossfadeUnit(unsigned short type,
                             bool scalingEnabled,
                             unsigned short offset)
: _scalingEnabled(scalingEnabled)
{
    for (int n = 0; n <= 200; ++n)
    {
        double value = (n - 100) / 100.0;
        
        // Linear table
        _tables[CrossfadeTypes::LINEAR][n].left = (1 - value) / 2.0;
        
        _tables[CrossfadeTypes::LINEAR][n].right = (1 + value) / 2.0;
        
        // Sine table
        _tables[CrossfadeTypes::SINE][n].left = sin((1 -  value) / 2 * Global::pi/2);
        
        _tables[CrossfadeTypes::SINE][n].right = sin((1 +  value) / 2 * Global::pi/2);
        
        // Square root table
        _tables[CrossfadeTypes::SQRT][n].left = sqrt((1 -  value) / 2);
        
        _tables[CrossfadeTypes::SQRT][n].right = sqrt((1 +  value) / 2);
    }
    
    setType(type);
    
    setValue(offset);
}

void CrossfadeUnit::setType(unsigned short type)
{
    if (type > CrossfadeTypes::SQRT)
    { throw std::invalid_argument("Type argument out of range!"); }
    
    _type = type;
}

void CrossfadeUnit::setValue(short value)
{
    if (value < -100 || value > 100)
        throw std::invalid_argument("Crossfade value must be between -100 and 100");
    
    _index = 100 + value;
}

void CrossfadeUnit::setScalingEnabled(bool scalingEnabled)
{ _scalingEnabled = scalingEnabled; }

double CrossfadeUnit::_scale(const double& value) const
{
    // Scale values for sine and sqrt so that the
    // values add up to 1
    if (_type == CrossfadeTypes::SINE ||
        _type == CrossfadeTypes::SQRT)
    { return value * Global::sqrt2; }
    
    // Avoid passing parameter by valuem by passing
    // as reference-to-const but have to do this
    // return here if no scale
    return value;
}

double CrossfadeUnit::left() const
{ return _scale(_tables[_type][_index].left); }

double CrossfadeUnit::right() const
{ return _scale(_tables[_type][_index].right); }

Crossfader::Crossfader(unsigned short fadeType, bool scale,
                       GenUnit* left, GenUnit* right)
: _xfade(0), _left(left), _right(right), _fadeType(123) // random
{
    setFadeType(fadeType,scale);
    
    // Initialize ModDocks
    _initModDocks();
}

Crossfader::~Crossfader()
{ delete _xfade; }

double Crossfader::tick()
{
    // Get left and right ticks and fade them appropriately
    double left = _left->tick() * _xfade->left();
    double right = _right->tick() * _xfade->right();
    
    // Return the combined value 
    return left + right;
}

void Crossfader::_initModDocks()
{ _mods = { new ModDock(2) }; }

void Crossfader::setLeftGenUnit(GenUnit* genUnit)
{ _left = genUnit; }

void Crossfader::setRightGenUnit(GenUnit* genUnit)
{ _right = genUnit; }

void Crossfader::setValue(short value)
{ _xfade->setValue(value); }

void Crossfader::setFadeType(unsigned short fadeType, bool scale)
{
    if (fadeType > SQRT)
    { throw std::invalid_argument("Invalid fade type!"); }
    
    // Make sure we don't unnecessariliy delete and
    // re-allocate an CrossfadeUnit
    if (fadeType != _fadeType)
    {
        delete _xfade;
        
        switch (fadeType)
        {/*
            case LINEAR:
                _xfade = new XFadeLinear;
                break;
                
            case SINE:
                _xfade = new XFadeSine(scale);
                break;
                
            case SQRT:
                _xfade = new XFadeSqrt(scale);
                break;*/
        }
        
        // Store type to check for re-allocation
        _fadeType = fadeType;
    }
}