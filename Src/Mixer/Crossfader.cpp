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

Crossfader::Crossfader(unsigned short type,
                       bool scalingEnabled,
                       unsigned short offset,
                       GenUnit* left,
                       GenUnit* right)
: CrossfadeUnit(type,scalingEnabled,offset),
  _leftUnit(left), _rightUnit(right)
{
    // Initialize ModDocks
    _initModDocks();
}

void Crossfader::setValue(short value)
{
    if (value < -100 || value > 100)
        throw std::invalid_argument("Crossfade value must be between -100 and 100");
    
    _value = value;
    
    _index = 100 + value;
}

double Crossfader::tick()
{
    if (_mods[VALUE]->inUse())
    {
        // Only set the index, the value is not changed (constant base value)
        _index = _mods[VALUE]->checkAndTick(_value, -100, 100) + 100;
    }
    
    // Get left and right ticks (if a GenUnit is available) and fade them appropriately
    double left = (_leftUnit) ? _leftUnit->tick() * this->left() : 0;
    double right = (_rightUnit) ? _rightUnit->tick() * this->right() : 0;
    
    // Return the combined value 
    return (left + right) * _amp;
}

void Crossfader::_initModDocks()
{ _mods = { new ModDock(2) }; }

void Crossfader::setLeftUnit(GenUnit* unit)
{ _leftUnit = unit; }

void Crossfader::setRightUnit(GenUnit* unit)
{ _rightUnit = unit; }