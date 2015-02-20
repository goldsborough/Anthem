/********************************************************************************************//*!
*
*  @file        ModDock.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "ModDock.hpp"
#include "Units.hpp"

#include <stdexcept>

ModDock::ModDock()
{ }

ModDock::ModDock(double lowerBoundary,
                 double higherBoundary,
                 double baseValue)

: lowerBoundary_(lowerBoundary), higherBoundary_(higherBoundary),
  baseValue_(baseValue)

{
    // Reserve space so that iterators aren't invalidated
    // when pushing back
    nonMasterItems_.reserve(256);
    masterItems_.reserve(256);
    modItems_.reserve(256);
}

void ModDock::setBaseValue(double baseValue)
{
    baseValue_ = baseValue;
}

double ModDock::getBaseValue() const
{
    return baseValue_;
}

void ModDock::setLowerBoundary(double lowerBoundary)
{
    lowerBoundary_ = lowerBoundary;
}

double ModDock::getLowerBoundary() const
{
    return lowerBoundary_;
}

void ModDock::setHigherBoundary(double higherBoundary)
{
    higherBoundary_ = higherBoundary;
}

double ModDock::getHigherBoundary() const
{
    return higherBoundary_;
}

bool ModDock::inUse() const
{
    return ! modItems_.empty();
}

void ModDock::setSidechain(index_t master, index_t slave)
{
    if (master >= modItems_.size())
    { throw std::invalid_argument("Invalid index for sidechain master!"); }
    
    if (slave >= modItems_.size())
    { throw std::invalid_argument("Invalid index for sidechain slave!"); }
    
    // Nothing to do here
    if (isSidechain(master, slave)) return;
    
    // Add master to slave's master vector
    modItems_[slave].masters.push_back(master);
    
    modItems_[master].slaves.push_back(slave);
    
    if (std::find(masterItems_.begin(), masterItems_.end(), master) == masterItems_.end())
    {
        // Add master to masterItems_ if not already present (in case of multiple slavery (haha))
        masterItems_.push_back(master);
        
        // Erase masterItr from nonMasterItems (because it's a master now)
        nonMasterItems_.erase(std::find(nonMasterItems_.begin(), nonMasterItems_.end(), master));
    }
}

void ModDock::unSidechain(index_t master, index_t slave)
{
    if (master >= modItems_.size())
    { throw std::invalid_argument("Invalid index for sidechain master!"); }
    
    if (slave >= modItems_.size())
    { throw std::invalid_argument("Invalid index for sidechain slave!"); }
    
    if (! isSidechain(master, slave))
    { throw std::logic_error("Passed dock is not master of passed slave!"); }
    
    indexVec& slaves = modItems_[master].slaves;
    
    // Remove slave from master's slave vector
    slaves.erase(std::find(slaves.begin(), slaves.end(), slave));
    
    // Move master fron masterItems_ to nonMasterItems_ if master
    // has no more slaves
    if (slaves.empty())
    {
        masterItems_.erase(std::find(masterItems_.begin(), masterItems_.end(), master));
        
        nonMasterItems_.push_back(master);
    }
    
    indexVec& masters = modItems_[slave].masters;
    
    // Remove master from slave's master vec
    masters.erase(std::find(masters.begin(), masters.end(), master));
}

bool ModDock::isSidechain(index_t master, index_t slave) const
{
    const indexVec& slaves = modItems_[master].slaves;
    
    return std::find(slaves.begin(), slaves.end(), slave) != slaves.end();
}

bool ModDock::isMaster(index_t index) const
{
    return ! modItems_[index].slaves.empty();
}

bool ModDock::isSlave(index_t index) const
{
    return ! modItems_[index].masters.empty();
}

double ModDock::tick()
{
    return modulate(baseValue_);
}

double ModDock::modulate(double sample)
{
    // If ModDock is not in use, return original sample immediately
    if (! inUse()) return sample;
    
    // Sidechaining
    
    // For every non-master
    for (indexVecItr nonMasterItr = nonMasterItems_.begin(), nonMasterEnd = nonMasterItems_.end();
         nonMasterItr != nonMasterEnd;
         ++nonMasterItr)
    {
        // If it isn't a slave, nothing to do
        if (! isSlave(*nonMasterItr)) continue;
        
        ModItem& slave = modItems_[*nonMasterItr];
        
        // Set to zero initially
        slave.depth = 0;
        
        // Then sum up the depth from all masters
        for (indexVecItr_const masterItr = slave.masters.begin(), masterEnd = slave.masters.end();
             masterItr != masterEnd;
             ++masterItr)
        {
            ModItem& master = modItems_[*masterItr];
            
            // Using the baseDepth as the base value and the master's depth as
            // the depth for modulation and 1 as the maximum boundary
            slave.depth += master.mod->modulate(slave.baseDepth, master.depth, 1);
        }
        
        // Average the depth
        slave.depth /= slave.masters.size();
    }
    
    // Modulation
    
    double temp = 0;
    
    // Get modulation from all non-master items
    for(indexVecItr_const itr = nonMasterItems_.begin(), end = nonMasterItems_.end();
        itr != end;
        ++itr)
    {
        // Add to result so we can average later
        // Use the sample as base, the modItem's depth as depth and the
        // higherBoundary as maximum
        temp += modItems_[*itr].mod->modulate(sample,
                                              modItems_[*itr].depth,
                                              higherBoundary_);
    }
    
    // Average
    sample = temp / nonMasterItems_.size();
    
    // Boundary checking
    if (sample > higherBoundary_) { sample = higherBoundary_; }
    
    else if (sample < lowerBoundary_) { sample = lowerBoundary_; }
    
    return sample;
}

void ModDock::setDepth(index_t index, double depth)
{
    if (index >= modItems_.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    if (depth < -1 || depth > 1)
    { throw std::invalid_argument("Invalid depth value, must be between -1 and 1!"); }
    
    modItems_[index].depth = modItems_[index].baseDepth = depth;
}

double ModDock::getDepth(index_t index)
{
    if (index >= modItems_.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    return modItems_[index].baseDepth;
}

void ModDock::attach(ModUnit* mod)
{
    // Put ModItem into modItems_
    modItems_.push_back(ModItem(mod));
    
    nonMasterItems_.push_back(modItems_.size() - 1);
}

void ModDock::detach(index_t index)
{
    if (index >= modItems_.size())
    { throw std::invalid_argument("ModDock index out of bounds!"); }
    
    if (isMaster(index))
    {
        // Remove sidechain between master and all its slaves
        for(indexVecItr itr = modItems_[index].slaves.begin(), end = modItems_[index].slaves.end();
            itr != end;
            ++itr)
        {
            unSidechain(index,*itr);
        }
        
        masterItems_.erase(std::find(masterItems_.begin(),masterItems_.end(),index));
    }
    
    else
    { nonMasterItems_.erase(std::find(nonMasterItems_.begin(), nonMasterItems_.end(), index)); }
    
    modItems_.erase(modItems_.begin() + index);
}

unsigned long ModDock::size() const
{
    return modItems_.size();
}