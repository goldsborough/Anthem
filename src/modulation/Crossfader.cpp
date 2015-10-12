/********************************************************************************************//*!
*
*  @file        Crossfader.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Crossfader.hpp"
#include "Global.hpp"
#include "ModDock.hpp"
#include "Pantable.hpp"

#include <stdexcept>
#include <cmath>

CrossfadeUnit::CrossfadeUnit(unsigned short type,
                             bool scalingEnabled,
                             unsigned short offset)
{
    setType(type);
 
    enableScaling(scalingEnabled);
    
    setValue(offset);
}

CrossfadeUnit::CrossfadeUnit(const CrossfadeUnit& other)
: _type(other._type), _scalingEnabled(other._scalingEnabled),
  _index(other._index), _table(other._table)
{ }

CrossfadeUnit& CrossfadeUnit::operator=(const CrossfadeUnit &other)
{
    if (this != &other)
    {
        _type = other._type;
        
        _scalingEnabled = other._scalingEnabled;
        
        _index = other._index;
        
        _table = other._table;
    }
    
    return *this;
}

CrossfadeUnit::~CrossfadeUnit() = default;

void CrossfadeUnit::setType(unsigned short type)
{
    if (type > PantableDatabase::SQRT)
    {
        throw std::invalid_argument("Invalid type! If you tried setting a scaled type,\
                                     use enableScaling() and a non-scaled type.");
    }
    
    _type = type;
    
    // Unscaled types are 0,1,2 and scaled
    // types are 3,4 so + 2 is the scaled
    // variant of an unscaled type
    if (_scalingEnabled) type += 2;
    
    _table = pantableDatabase[type];
}

unsigned short CrossfadeUnit::getType() const
{
    return _type;
}

void CrossfadeUnit::setValue(double value)
{
    if (value < -100 || value > 100)
        throw std::invalid_argument("Crossfade value must be between -100 and 100");
    
    _index = value + 100;
    
    _curr = _table->interpolate(_index);
}

double CrossfadeUnit::getValue() const
{
    // From index to value
    return _index - 100;
}

void CrossfadeUnit::enableScaling(bool scalingEnabled)
{
    // Nothing to do if same state
    if (_scalingEnabled == scalingEnabled) return;
    
    // Unscaled types are 0,1,2 and scaled
    // types are 3,4 so + 2 is the scaled
    // variant _type and to get the unscaled
    // type we just use _type
    _table = (scalingEnabled) ? pantableDatabase[_type] : pantableDatabase[_type + 2];
    
    _scalingEnabled = scalingEnabled;
}

bool CrossfadeUnit::scalingEnabled() const
{
    return _scalingEnabled;
}

double CrossfadeUnit::left() const
{
    return _curr.left;
}

double CrossfadeUnit::right() const
{
    return _curr.right;
}

Crossfader::Crossfader(unsigned short type,
                       bool scalingEnabled,
                       unsigned short offset,
                       ModUnit* left,
                       ModUnit* right)

: CrossfadeUnit(type,scalingEnabled,offset),
  _leftUnit(left), _rightUnit(right),
  ModUnit(1,1)
{
    // Initialize ModDocks
    _mods[VALUE].setHigherBoundary(100);
    _mods[VALUE].setLowerBoundary(-100);
    _mods[VALUE].setBaseValue(offset);
}

void Crossfader::setValue(double value)
{
    CrossfadeUnit::setValue(value);
    
    _mods[VALUE].setBaseValue(value);
}

double Crossfader::getValue() const
{
    if (_mods[VALUE].inUse())
    {
        return _mods[VALUE].getBaseValue();
    }
    
    else return CrossfadeUnit::getValue();
}

double Crossfader::modulate(double sample, double depth, double maximum)
{
    // Modulate value
    if (_mods[VALUE].inUse())
    {
        _index = _mods[VALUE].tick() + 100;
    }
    
    // Get left and right ticks (if a ModUnit is available) and fade them appropriately to current
    // crossfading values (left() and right())
    double left = (_leftUnit) ? _leftUnit->modulate(sample, depth, maximum) * this->left() : 0;
    
    double right = (_rightUnit) ? _rightUnit->modulate(sample, depth, maximum) * this->right() : 0;
    
    // Return the combined value 
    return (left + right) * _amp;
}

void Crossfader::setLeftUnit(ModUnit* unit)
{
    _leftUnit = unit;
}

void Crossfader::setRightUnit(ModUnit* unit)
{
    _rightUnit = unit;
}

ModUnit* Crossfader::getLeftUnit() const
{
    return _leftUnit;
}

ModUnit* Crossfader::getRightUnit() const
{
    return _rightUnit;
}