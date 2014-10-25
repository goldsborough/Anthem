#include "Crossfader.h"
#include "Global.h"
#include "ModDock.h"
#include "Sample.h"

#include <stdexcept>
#include <cmath>

CrossfadeUnit::CrossfadeUnit(unsigned short type,
                             bool scalingEnabled,
                             unsigned short offset)
: scalingEnabled_(scalingEnabled),
  tables_(new std::unique_ptr<Sample[]> [3])
{
    for (unsigned short i = 0; i < 3; ++i)
    {
        tables_[i].reset(new Sample[201]);
    }
    
    for (unsigned short j = 0; j <= 200; ++j)
    {
        double value = (j - 100) / 100.0;
        
        // Linear table
        tables_[CrossfadeTypes::LINEAR][j].left = (1 - value) / 2.0;
        
        tables_[CrossfadeTypes::LINEAR][j].right = (1 + value) / 2.0;
        
        // Sine table
        tables_[CrossfadeTypes::SINE][j].left = sin((1 -  value) / 2 * Global::pi/2);
        
        tables_[CrossfadeTypes::SINE][j].right = sin((1 +  value) / 2 * Global::pi/2);
        
        // Square root table
        tables_[CrossfadeTypes::SQRT][j].left = sqrt((1 -  value) / 2);
        
        tables_[CrossfadeTypes::SQRT][j].right = sqrt((1 +  value) / 2);
    }
    
    setType(type);
    
    setValue(offset);
}

CrossfadeUnit::CrossfadeUnit(const CrossfadeUnit& other)
: scalingEnabled_(other.scalingEnabled_),
  type_(other.type_), index_(other.index_),
  tables_(new std::unique_ptr<Sample[]> [3])
{
    for (unsigned short i = 0; i < 3; ++i)
    {
        tables_[i].reset(new Sample[201]);
        
        for (unsigned short j = 0; j <= 200; ++j)
        {
            tables_[i][j] = other.tables_[i][j];
        }
    }
}

CrossfadeUnit& CrossfadeUnit::operator= (const CrossfadeUnit& other)
{
    if (this != &other)
    {
        scalingEnabled_ = other.scalingEnabled_;
        
        type_ = other.type_;
        
        index_ = other.index_;
        
        // Table data is the same
    }
    
    return *this;
}

CrossfadeUnit::~CrossfadeUnit() { }

void CrossfadeUnit::setType(unsigned short type)
{
    if (type > CrossfadeTypes::SQRT)
    { throw std::invalid_argument("Type argument out of range!"); }
    
    type_ = type;
}

unsigned short CrossfadeUnit::getType() const
{
    return type_;
}

void CrossfadeUnit::setValue(short value)
{
    if (value < -100 || value > 100)
        throw std::invalid_argument("Crossfade value must be between -100 and 100");
    
    index_ = 100 + value;
}

short CrossfadeUnit::getValue() const
{
    // From index to value
    return index_ - 100;
}

void CrossfadeUnit::setScalingEnabled(bool scalingEnabled)
{
    scalingEnabled_ = scalingEnabled;
}

bool CrossfadeUnit::scalingEnabled() const
{
    return scalingEnabled_;
}

double CrossfadeUnit::scale_(const double& value) const
{
    // Scale values for sine and sqrt so that the
    // values add up to 1
    if (type_ == CrossfadeTypes::SINE ||
        type_ == CrossfadeTypes::SQRT)
    { return value * Global::sqrt2; }
    
    // Avoid passing parameter by valuem by passing
    // as reference-to-const but have to do this
    // return here if no scale
    return value;
}

double CrossfadeUnit::left() const
{
    return scale_(tables_[type_][index_].left);
}

double CrossfadeUnit::right() const
{
    return scale_(tables_[type_][index_].right);
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

void Crossfader::setValue(short value)
{
    CrossfadeUnit::setValue(value);
    
    mods_[VALUE].setBaseValue(value);
}

short Crossfader::getValue() const
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