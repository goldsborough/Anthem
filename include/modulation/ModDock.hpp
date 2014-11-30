/*********************************************************************************************//*!
*
*  @file        ModDock.hpp
*
*  @author      Peter Goldsborough
*
*  @date        18/10/2014
*
*  @brief       Modulation Dock a.k.a. ModDock.
*
*************************************************************************************************/

#ifndef __Anthem__ModDock__
#define __Anthem__ModDock__

#include <vector>

class ModUnit;

/*********************************************************************************************//*!
*
*  @brief       Modulation Dock to modulate any parameters.
*
*  @details     The ModDock class permits any parameter to be modulated via a set of ModUnits.
*
*************************************************************************************************/

class ModDock
{
    
public:

    typedef unsigned short index_t;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a ModDock without any initial settings.
    *
    *  @details     Values must be initialized through appropriate methods thereafter.
    *
    *************************************************************************************************/
    
    ModDock();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a ModDock.
    *
    *  @param       lowerBoundary The initial lower boundary value to scale to.
    *
    *  @param       higherBoundary The initial higher boundary value to scale to.
    *
    *  @param       baseValue The initial base value to modulate.
    *
    *************************************************************************************************/
    
    ModDock(double lowerBoundary,
            double higherBoundary,
            double baseValue);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Ticks the value obtained from calling modulated() with the base value member.
    *
    *  @see         modulate()
    *
    *************************************************************************************************/
    
    double tick();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Modulates a sample and returns the result.
    *
    *  @details     A sample is modulated by calling all ModUnits' modulate() method on the sample
    *               and then returning a mixture between the original and the modulated sample,
    *               according to _masterDepth.
    *
    *  @param       sample The sample to modulate.
    *
    *  @return      The modulated sample.
    *
    *  @see         tick()
    *
    *************************************************************************************************/
    
    double modulate(double sample);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the ModDock's base value for modulation.
    *
    *  @param       baseValue The new base value.
    *
    *************************************************************************************************/
    
    void setBaseValue(double baseValue);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the ModDock's current base value.
    *
    *  @return      The ModDock's current base value.
    *
    *************************************************************************************************/
    
    double getBaseValue() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Checks whether or not the ModDock is in use.
    *
    *  @details     The ModDock is in use if it isn't empty.
    *
    *************************************************************************************************/
    
    bool inUse() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the lower boundary.
    *
    *  @param       lowerBoundary The new lower boundary value.
    *
    *************************************************************************************************/
    
    void setLowerBoundary(double lowerBoundary);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the lower boundary value.
    *
    *  @return      The lower boundary.
    *
    *************************************************************************************************/
    
    double getLowerBoundary() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the higher boundary.
    *
    *  @param       higherBoundary The new higher boundary value.
    *
    *************************************************************************************************/
    
    void setHigherBoundary(double higherBoundary);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the higher boundary value.
    *
    *  @return      The higher boundary.
    *
    *************************************************************************************************/
    
    double getHigherBoundary() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets a ModUnit's depth.
    *
    *  @param       index The index of the ModUnit to set the depth value for.
    *
    *  @param       depth The new depth value, between 0 and 1.
    *
    *************************************************************************************************/
    
    void setDepth(index_t index, double depth);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the depth of one of the ModDock's ModUnits.
    *
    *  @param       index The index of the ModUnit to get the depth value for.
    *
    *  @return      The depth value of a ModUnit within the ModDock.
    *
    *************************************************************************************************/
    
    double getDepth(index_t index);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Attaches a ModUnit to the ModDock.
    *
    *  @param       mod A pointer to a ModUnit.
    *
    *************************************************************************************************/
    
    void attach(ModUnit* mod);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Detaches a ModUnit from the ModDock.
    *
    *  @param       index The index of the ModDock to detach.
    *
    *************************************************************************************************/
    
    void detach(index_t index);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Makes a ModUnit sidechain another's depth.
    *
    *  @details     Sidechaining means that one ModUnit sidechains/modulates another's depth
    *               value. Multiple mastery as well as multiple slavery is possible, so you
    *               can call setSidechain for any two indices you wish as long as the connection
    *               isn't already established, in which case nothing happens. Note that a ModUnit
    *               cannot simultaneously be a master of slaves and contribute to the ModDock's
    *               modulation value, it is either-or.
    *
    *  @param       master The index of the ModUnit to be made master.
    *
    *  @param       slave The index of the slave.
    *
    *************************************************************************************************/
    
    void setSidechain(index_t master, index_t slave);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Cuts the sidechain connection between two ModUnits.
    *
    *  @param       master The index of the master.
    *
    *  @param       slave The index of the slave.
    *
    *  @throws      std::logic_error if the two ModUnits aren't connected.
    *
    *************************************************************************************************/
    
    void unSidechain(index_t master, index_t slave);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns true if there is a sidechain connection between master and slave.
    *
    *  @param       master The index of the master.
    *
    *  @param       slave The index of the slave.
    *
    *  @return      Boolean, whether or not master is sidechaining slave.
    *
    *************************************************************************************************/
    
    bool isSidechain(index_t master, index_t slave) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit is a master.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a master of one or more slaves, else false.
    *
    *************************************************************************************************/
    
    bool isMaster(index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit is a slave.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a slave of one or more masters, else false.
    *
    *************************************************************************************************/
    
    bool isSlave(index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the size of the ModDock.
    *
    *  @return      The number of ModUnit docks in the ModDock.
    *
    *************************************************************************************************/
    
    unsigned long size() const;
    
private:
    
    struct ModItem;
    
    typedef std::vector<ModItem> modVec;
    
    // Not using iterators because they're invalidated when
    // pushing back/erasing from modItems_ and not using
    // pointers to ModItems because then it's difficult
    // to interact with modItems_
    typedef std::vector<modVec::size_type> indexVec;
    
    typedef indexVec::iterator indexVecItr;
    
    typedef indexVec::const_iterator indexVecItr_const;
    
    /*! A ModItem contains a ModUnit* and a depth value */
    struct ModItem
    {
        ModItem(ModUnit* modUnit, double dpth = 1)
        : mod(modUnit), depth(dpth), baseDepth(dpth)
        { }
    
        /*! The actual ModUnit pointer. */
        ModUnit* mod;
        
        /*! The current modulation depth. */
        double depth;
        
        /*! For sidechaining */
        double baseDepth;
        
        /*! Vector of indexes of all the master's indexes in modItems_*/
        indexVec masters;
        
        /* Vector of all slaves */
        indexVec slaves;
    };
    
    /*! Pointers to all ModItems that are masters for sidechaining
        and thus don't contribute to the ModDocks modulation value */
    indexVec masterItems_;
    
    /*! Pointer to all ModItems excluding sidechaining masters */
    indexVec nonMasterItems_;
    
    /*! All ModItems */
    modVec modItems_;
    
    /*! This is the base value that the modulation happens around */
    double baseValue_;
    
    /*! Lower boundary value to scale to when modulation trespasses it */
    double lowerBoundary_;
    
    /*! Higher boundary value to scale to when modulation trespasses it */
    double higherBoundary_;
};

#endif /* defined(__Anthem__ModDock__) */
