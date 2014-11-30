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
: type_(other.type_), scalingEnabled_(other.scalingEnabled_),
  index_(other.index_), table_(other.table_)
{ }

CrossfadeUnit& CrossfadeUnit::operator=(const CrossfadeUnit &other)
{
    if (this != &other)
    {
        type_ = other.type_;
        
        scalingEnabled_ = other.scalingEnabled_;
        
        index_ = other.index_;
        
        table_ = other.table_;
    }
    
    return *this;
}

CrossfadeUnit::~CrossfadeUnit() { }

void CrossfadeUnit::setType(unsigned short type)
{
    if (type > PantableDatabase::SQRT)
    {
        throw std::invalid_argument("Invalid type! If you tried setting a scaled type,\
                                     use enableScaling() and a non-scaled type.");
    }
    
    type_ = type;
    
    // Unscaled types are 0,1,2 and scaled
    // types are 3,4 so + 2 is the scaled
    // variant of an unscaled type
    if (scalingEnabled_) type += 2;
    
    table_ = pantableDatabase[type];
}

unsigned short CrossfadeUnit::getType() const
{
    return type_;
}

void CrossfadeUnit::setValue(double value)
{
    if (value < -100 || value > 100)
        throw std::invalid_argument("Crossfade value must be between -100 and 100");
    
    index_ = value + 100;
    
    curr_ = table_.interpolate(index_);
}

double CrossfadeUnit::getValue() const
{
    // From index to value
    return index_ - 100;
}

void CrossfadeUnit::enableScaling(bool scalingEnabled)
{
    // Nothing to do if same state
    if (scalingEnabled_ == scalingEnabled) return;
    
    // Unscaled types are 0,1,2 and scaled
    // types are 3,4 so + 2 is the scaled
    // variant type_ and to get the unscaled
    // type we just use type_
    table_ = (scalingEnabled) ? pantableDatabase[type_] : pantableDatabase[type_ + 2];
    
    scalingEnabled_ = scalingEnabled;
}

bool CrossfadeUnit::scalingEnabled() const
{
    return scalingEnabled_;
}

double CrossfadeUnit::left() const
{
    return curr_.left;
}

double CrossfadeUnit::right() const
{
    return curr_.right;
}

Crossfader::Crossfader(unsigned short type,
                       bool scalingEnabled,
                       unsigned short offset,
                       ModUnit* left,
                       ModUnit* right)

: CrossfadeUnit(type,scalingEnabled,offset),
  leftUnit_(left), rightUnit_(right),
  ModUnit(1,1)
{
    // Initialize ModDocks
    mods_[VALUE].setHigherBoundary(100);
    mods_[VALUE].setLowerBoundary(-100);
    mods_[VALUE].setBaseValue(offset);
}

void Crossfader::setValue(double value)
{
    CrossfadeUnit::setValue(value);
    
    mods_[VALUE].setBaseValue(value);
}

double Crossfader::getValue() const
{
    if (mods_[VALUE].inUse())
    {
        return mods_[VALUE].getBaseValue();
    }
    
    else return CrossfadeUnit::getValue();
}

double Crossfader::modulate(double sample, double depth, double maximum)
{
    // Modulate value
    if (mods_[VALUE].inUse())
    {
        index_ = mods_[VALUE].tick() + 100;
    }
    
    // Get left and right ticks (if a ModUnit is available) and fade them appropriately to current
    // crossfading values (left() and right())
    double left = (leftUnit_) ? leftUnit_->modulate(sample, depth, maximum) * this->left() : 0;
    
    double right = (rightUnit_) ? rightUnit_->modulate(sample, depth, maximum) * this->right() : 0;
    
    // Return the combined value 
    return (left + right) * amp_;
}

void Crossfader::setLeftUnit(ModUnit* unit)
{
    leftUnit_ = unit;
}

void Crossfader::setRightUnit(ModUnit* unit)
{
    rightUnit_ = unit;
}

ModUnit* Crossfader::getLeftUnit() const
{
    return leftUnit_;
}

ModUnit* Crossfader::getRightUnit() const
{
    return rightUnit_;
}