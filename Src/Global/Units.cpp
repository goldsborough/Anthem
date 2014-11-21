//
//  GenUnits.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Units.hpp"
#include "ModDock.hpp"
#include "Wavetable.hpp"

#include <stdexcept>

Unit::Unit(index_t numDocks)
: mods_(new ModDock [numDocks]),
  numDocks_(numDocks)
{ }

Unit::Unit(const Unit& other)
: mods_(new ModDock [other.numDocks_])
{
    for (index_t i = 0; i < other.numDocks_; ++i)
    {
        mods_[i] = other.mods_[i];
    }
}

Unit& Unit::operator=(const Unit& other)
{
    if (this != &other)
    {
        mods_.reset(new ModDock [other.numDocks_]);
        
        for (index_t i = 0; i < other.numDocks_; ++i)
        {
            mods_[i] = other.mods_[i];
        }
    }
    
    return *this;
}

Unit::~Unit() { }

Unit::index_t Unit::numDocks() const
{
    return numDocks_;
}

void Unit::setModUnitDepth(index_t dockNum,
                           index_t modNum,
                           double depth)
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    if (depth > 1 || depth < 0)
    { throw std::invalid_argument("Depth value must be between 0 and 1!");}
    
    mods_[dockNum].setDepth(modNum, depth);
}

double Unit::getModUnitDepth(index_t dockNum, index_t modNum) const
{
    return mods_[dockNum].getDepth(modNum);
}

void Unit::attachMod(index_t dockNum,
                     ModUnit* mod)
{
    if (mod == this)
    { throw std::invalid_argument("Unit cannot modulate itself!"); }
    
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    mods_[dockNum].attach(mod);
}

void Unit::detachMod(index_t dockNum,
                     index_t modNum)
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    mods_[dockNum].detach(modNum);
}

bool Unit::dockInUse(index_t dockNum) const
{
    return mods_[dockNum].inUse();
}

void Unit::setSidechain(index_t dockNum, index_t master, index_t slave)
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    mods_[dockNum].setSidechain(master, slave);
}

void Unit::unSidechain(index_t dockNum, index_t master, index_t slave)
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    mods_[dockNum].unSidechain(master, slave);
}

bool Unit::isSidechain(index_t dockNum, index_t master, index_t slave) const
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    return mods_[dockNum].isSidechain(master,slave);
}

bool Unit::isMaster(index_t dockNum, index_t index) const
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    return mods_[dockNum].isMaster(index);
}

bool Unit::isSlave(index_t dockNum, index_t index) const
{
    if (dockNum >= numDocks_)
    { throw std::invalid_argument("Dock index out of range!"); }
    
    return mods_[dockNum].isSlave(index);
}

unsigned long Unit::dockSize(index_t dockNum) const
{
    return mods_[dockNum].size();
}

EffectUnit::EffectUnit(unsigned short numDocks, double dryWet)
: dw_(dryWet), Unit(numDocks)
{ }

void EffectUnit::setDryWet(double dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet level must be between 0 and 1!"); }
    
    dw_ = dw;
}

double EffectUnit::getDryWet() const
{
    return dw_;
}

double EffectUnit::dryWet_(double originalSample, double processedSample)
{
    return (originalSample * (1 - dw_)) + (processedSample * dw_);
}

double EffectUnit::dryWet_(double originalSample, double processedSample, double dryWet)
{
    return (originalSample * (1 - dryWet)) + (processedSample * dryWet);
}

GenUnit::GenUnit(unsigned short numDocks, double amp)
: Unit(numDocks), amp_(amp)
{ }

void GenUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    amp_ = amp;
}

double GenUnit::getAmp() const
{
    return amp_;
}

ModUnit::ModUnit(unsigned short numDocks, double amp)
: Unit(numDocks), amp_(amp)
{ }

void ModUnit::setAmp(double amp)
{
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    amp_ = amp;
}

double ModUnit::getAmp() const
{
    return amp_;
}