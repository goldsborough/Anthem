#include "Macro.hpp"
#include "ModDock.hpp"

#include <stdexcept>

Macro::Macro(double value)
: ModUnit(1), value_(value)
{
    mods_[VALUE].setHigherBoundary(1);
    mods_[VALUE].setLowerBoundary(-1);
    mods_[VALUE].setBaseValue(value);
}

void Macro::setValue(double value)
{
    if (value > 1 || value < -1)
    { throw std::invalid_argument("Value must be between -1 and 1!"); }
    
    value_ = value;
    
    mods_[VALUE].setBaseValue(value_);
}

double Macro::getValue() const
{
    if (mods_[VALUE].inUse())
    {
        return mods_[VALUE].getBaseValue();
    }
    
    else return value_;
}

double Macro::modulate(double sample, double depth, double maximum)
{
    if (mods_[VALUE].inUse())
    {
        value_ = mods_[VALUE].tick();
    }
    
    return sample + (maximum * value_ * depth);
}