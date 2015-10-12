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

: _lowerBoundary(lowerBoundary), _higherBoundary(higherBoundary),
  _baseValue(baseValue)

{
    // Reserve space so that iterators aren't invalidated
    // when pushing back
    _nonMasterItems.reserve(256);
    _masterItems.reserve(256);
    _modItems.reserve(256);
}

void ModDock::setBaseValue(double baseValue)
{
    _baseValue = baseValue;
}

double ModDock::getBaseValue() const
{
    return _baseValue;
}

void ModDock::setLowerBoundary(double lowerBoundary)
{
    _lowerBoundary = lowerBoundary;
}

double ModDock::getLowerBoundary() const
{
    return _lowerBoundary;
}

void ModDock::setHigherBoundary(double higherBoundary)
{
    _higherBoundary = higherBoundary;
}

double ModDock::getHigherBoundary() const
{
    return _higherBoundary;
}

bool ModDock::inUse() const
{
    return ! _modItems.empty();
}

void ModDock::setSidechain(index_t master, index_t slave)
{
    if (master >= _modItems.size())
    { throw std::out_of_range("Invalid index for sidechain master!"); }
    
    if (slave >= _modItems.size())
    { throw std::out_of_range("Invalid index for sidechain slave!"); }
    
    // Nothing to do here
    if (isSidechain(master, slave)) return;
    
    // Add master to slave's master vector
    _modItems[slave].masters.push_back(master);
    
    _modItems[master].slaves.push_back(slave);
    
    if (std::find(_masterItems.begin(), _masterItems.end(), master) == _masterItems.end())
    {
        // Add master to _masterItems if not already present (in case of multiple slavery (haha))
        _masterItems.push_back(master);
        
        // Erase masterItr from nonMasterItems (because it's a master now)
        _nonMasterItems.erase(std::find(_nonMasterItems.begin(), _nonMasterItems.end(), master));
    }
}

void ModDock::unSidechain(index_t master, index_t slave)
{
    if (master >= _modItems.size())
    { throw std::out_of_range("Invalid index for sidechain master!"); }
    
    if (slave >= _modItems.size())
    { throw std::out_of_range("Invalid index for sidechain slave!"); }
    
    if (! isSidechain(master, slave))
    { throw std::logic_error("Passed dock is not master of passed slave!"); }
    
    indexVec& slaves = _modItems[master].slaves;
    
    // Remove slave from master's slave vector
    slaves.erase(std::find(slaves.begin(), slaves.end(), slave));
    
    // Move master fron _masterItems to _nonMasterItems if master
    // has no more slaves
    if (slaves.empty())
    {
        _masterItems.erase(std::find(_masterItems.begin(), _masterItems.end(), master));
        
        _nonMasterItems.push_back(master);
    }
    
    indexVec& masters = _modItems[slave].masters;
    
    // Remove master from slave's master vec
    masters.erase(std::find(masters.begin(), masters.end(), master));
}

void ModDock::clearSlaves(index_t master)
{
    if (master >= _modItems.size())
    { throw std::out_of_range("Invalid master index!"); }
    
    // Nothing to do if this isn't actually a master
    if (_modItems[master].slaves.empty()) return;
    
    // Remove master index from slaves' vectors
    for (auto slave : _modItems[master].slaves)
    {
        indexVec& masters = _modItems[slave].masters;
        
        masters.erase(std::find(masters.begin(), masters.end(), master));
    }
    
    // Clear out the slave vector
    _modItems[master].slaves.clear();
    
    // Remove master index from masterItems
    _masterItems.erase(std::find(_masterItems.begin(), _masterItems.end(), master));
    
    // Push into nonMasterItems
    _nonMasterItems.push_back(master);
}

bool ModDock::isSidechain(index_t master, index_t slave) const
{
    const indexVec& slaves = _modItems[master].slaves;
    
    return std::find(slaves.begin(), slaves.end(), slave) != slaves.end();
}

bool ModDock::isMaster(index_t index) const
{
    return ! _modItems[index].slaves.empty();
}

bool ModDock::isSlave(index_t index) const
{
    return ! _modItems[index].masters.empty();
}

double ModDock::tick()
{
    return modulate(_baseValue);
}

double ModDock::modulate(double sample)
{
    // If ModDock is not in use, return original sample immediately
    if (! inUse()) return sample;
    
    // Sidechaining
    
    // For every non-master
    for (indexVecItr nonMasterItr = _nonMasterItems.begin(), nonMasterEnd = _nonMasterItems.end();
         nonMasterItr != nonMasterEnd;
         ++nonMasterItr)
    {
        // If it isn't a slave, nothing to do
        if (! isSlave(*nonMasterItr)) continue;
        
        ModItem& slave = _modItems[*nonMasterItr];
        
        // Set to zero initially
        slave.depth = 0;
        
        // Then sum up the depth from all masters
        for (indexVecItr_const masterItr = slave.masters.begin(), masterEnd = slave.masters.end();
             masterItr != masterEnd;
             ++masterItr)
        {
            ModItem& master = _modItems[*masterItr];
            
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
    for(indexVecItr_const itr = _nonMasterItems.begin(), end = _nonMasterItems.end();
        itr != end;
        ++itr)
    {
        // Add to result so we can average later
        // Use the sample as base, the modItem's depth as depth and the
        // higherBoundary as maximum
        temp += _modItems[*itr].mod->modulate(sample,
                                              _modItems[*itr].depth,
                                              _higherBoundary);
    }
    
    // Average
    sample = temp / _nonMasterItems.size();
    
    // Boundary checking
    if (sample > _higherBoundary) { sample = _higherBoundary; }
    
    else if (sample < _lowerBoundary) { sample = _lowerBoundary; }
    
    return sample;
}

void ModDock::setDepth(index_t index, double depth)
{
    if (index >= _modItems.size())
    { throw std::out_of_range("ModDock index out of bounds!"); }
    
    if (depth < -1 || depth > 1)
    { throw std::out_of_range("Invalid depth value, must be between -1 and 1!"); }
    
    _modItems[index].depth = _modItems[index].baseDepth = depth;
}

double ModDock::getDepth(index_t index)
{
    if (index >= _modItems.size())
    { throw std::out_of_range("ModDock index out of bounds!"); }
    
    return _modItems[index].baseDepth;
}

void ModDock::attach(ModUnit* mod)
{
    // Put ModItem into _modItems
    _modItems.push_back(ModItem(mod));
    
    _nonMasterItems.push_back(_modItems.size() - 1);
}

void ModDock::detach(index_t index)
{
    if (index >= _modItems.size())
    { throw std::out_of_range("ModDock index out of bounds!"); }
    
    if (isMaster(index))
    {
        // Remove sidechain between master and all its slaves
        for(indexVecItr itr = _modItems[index].slaves.begin(), end = _modItems[index].slaves.end();
            itr != end;
            ++itr)
        {
            unSidechain(index,*itr);
        }
        
        _masterItems.erase(std::find(_masterItems.begin(),_masterItems.end(),index));
    }
    
    else
    { _nonMasterItems.erase(std::find(_nonMasterItems.begin(), _nonMasterItems.end(), index)); }
    
    _modItems.erase(_modItems.begin() + index);
}

unsigned long ModDock::size() const
{
    return _modItems.size();
}