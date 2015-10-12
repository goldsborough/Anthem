/********************************************************************************************//*!
*
*  @file        Macro.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Macro.hpp"
#include "ModDock.hpp"

#include <stdexcept>

Macro::Macro(double value)
: ModUnit(1), _value(value)
{
    _mods[VALUE].setHigherBoundary(1);
    _mods[VALUE].setLowerBoundary(-1);
    _mods[VALUE].setBaseValue(value);
}

void Macro::setValue(double value)
{
    if (value > 1 || value < -1)
    { throw std::invalid_argument("Value must be between -1 and 1!"); }
    
    _value = value;
    
    _mods[VALUE].setBaseValue(_value);
}

double Macro::getValue() const
{
    if (_mods[VALUE].inUse())
    {
        return _mods[VALUE].getBaseValue();
    }
    
    else return _value;
}

double Macro::modulate(double sample, double depth, double maximum)
{
    if (_mods[VALUE].inUse())
    {
        _value = _mods[VALUE].tick();
    }
    
    return sample + (maximum * _value * depth);
}