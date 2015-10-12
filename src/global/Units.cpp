/********************************************************************************************//*!
*
*  @file        Units.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Units.hpp"
#include "ModDock.hpp"
#include "Wavetable.hpp"

#include <stdexcept>

Unit::Unit(index_t numDocks)
: _mods(new ModDock [numDocks]),
  _numDocks(numDocks), _active(false)
{ }

Unit::Unit(const Unit& other)
: _mods(new ModDock [other._numDocks]),
  _active(other._active)
{
    for (index_t i = 0; i < other._numDocks; ++i)
    {
        _mods[i] = other._mods[i];
    }
}

Unit& Unit::operator=(const Unit& other)
{
    if (this != &other)
    {
        _active = true;
        
        _mods.reset(new ModDock [other._numDocks]);
        
        for (index_t i = 0; i < other._numDocks; ++i)
        {
            _mods[i] = other._mods[i];
        }
    }
    
    return *this;
}

Unit::~Unit() { }

Unit::index_t Unit::numDocks() const
{
    return _numDocks;
}

void Unit::setModUnitDepth(index_t dockNum,
                           index_t modNum,
                           double depth)
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    if (depth > 1 || depth < -1)
    {
        throw std::invalid_argument("Depth value must be between -1 and 1!");
    }
    
    _mods[dockNum].setDepth(modNum, depth);
}

double Unit::getModUnitDepth(index_t dockNum, index_t modNum) const
{
    return _mods[dockNum].getDepth(modNum);
}

void Unit::attachMod(index_t dockNum,
                     ModUnit* mod)
{
    if (mod == this)
    {
        throw std::invalid_argument("Unit cannot modulate itself!");
    }
    
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    _mods[dockNum].attach(mod);
}

void Unit::detachMod(index_t dockNum,
                     index_t modNum)
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    _mods[dockNum].detach(modNum);
}

bool Unit::dockInUse(index_t dockNum) const
{
    return _mods[dockNum].inUse();
}

void Unit::setSidechain(index_t dockNum, index_t master, index_t slave)
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    _mods[dockNum].setSidechain(master, slave);
}

void Unit::unSidechain(index_t dockNum, index_t master, index_t slave)
{
    if (dockNum >= _numDocks)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    _mods[dockNum].unSidechain(master, slave);
}

bool Unit::isSidechain(index_t dockNum, index_t master, index_t slave) const
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    return _mods[dockNum].isSidechain(master,slave);
}

bool Unit::isMaster(index_t dockNum, index_t index) const
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    return _mods[dockNum].isMaster(index);
}

bool Unit::isSlave(index_t dockNum, index_t index) const
{
    if (dockNum >= _numDocks)
    {
        throw std::invalid_argument("Dock index out of range!");
    }
    
    return _mods[dockNum].isSlave(index);
}

unsigned long Unit::dockSize(index_t dockNum) const
{
    return _mods[dockNum].size();
}

void Unit::setActive(bool state)
{
    _active = state;
}

bool Unit::isActive() const
{
    return _active;
}

EffectUnit::EffectUnit(unsigned short numDocks, double dryWet)
: _dw(dryWet), Unit(numDocks)
{ }

void EffectUnit::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    {
        throw std::invalid_argument("Dry/wet level must be between 0 and 1!");
    }
    
    _dw = dw;
}

double EffectUnit::getDryWet() const
{
    return _dw;
}

double EffectUnit::_dryWet(double originalSample, double processedSample)
{
    return (originalSample * (1 - _dw)) + (processedSample * _dw);
}

double EffectUnit::_dryWet(double originalSample, double processedSample, double dryWet)
{
    return (originalSample * (1 - dryWet)) + (processedSample * dryWet);
}

GenUnit::GenUnit(unsigned short numDocks, double amp)
: Unit(numDocks), _amp(amp)
{ }

void GenUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    {
        throw std::invalid_argument("Amplitude must be between 0 and 1!");
    }
    
    _amp = amp;
}

double GenUnit::getAmp() const
{
    return _amp;
}

ModUnit::ModUnit(unsigned short numDocks, double amp)
: Unit(numDocks), _amp(amp)
{ }

void ModUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    {
        throw std::invalid_argument("Amplitude must be between 0 and 1!");
    }
    
    _amp = amp;
}

double ModUnit::getAmp() const
{
    return _amp;
}