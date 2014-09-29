/*********************************************************************************************//*!
*
*  @file        ModDock.h
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


/*! @todo ModDock sidechaining */

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
    *  @param       masterDepth The inital master depth, defaults to 1.
    *
    *************************************************************************************************/
    
    ModDock(double lowerBoundary,
            double higherBoundary,
            double baseValue,
            double masterDepth = 1);
    
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
    *  @details     The ModDock is in use it isn't empty and if the master depth is greater than 1.
    *
    *************************************************************************************************/
    
    bool inUse() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the master depth.
    *
    *  @param       depth The new depth value.
    *
    *************************************************************************************************/
    
    void setMasterDepth(double depth);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the master depth.
    *
    *  @return      The master depth.
    *
    *************************************************************************************************/
    
    double getMasterDepth() const;
    
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
    
    double getDepth(index_t index) const;
    
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
    *  @brief       Returns the size of the ModDock.
    *
    *  @return      The number of ModUnit docks in the ModDock.
    *
    *************************************************************************************************/
    
    unsigned long size() const;
    
private:
    
    /*! A ModItem contains a ModUnit* and a depth value */
    struct ModItem
    {
        ModItem(ModUnit* modUnit, double dpth = 1)
        : mod(modUnit), depth(dpth)
        { }
    
        ModUnit* mod;
        
        double depth;
    };
    
    /*! Vector of ModItems */
    std::vector<ModItem> _mods;
    
    /*! This is the base value that the modulation happens around */
    double _baseValue;
    
    /*! The master depth value */
    double _masterDepth;
    
    /*! Lower boundary value to scale to when modulation trespasses it */
    double _lowerBoundary;
    
    /*! Higher boundary value to scale to when modulation trespasses it */
    double _higherBoundary;
};

#endif /* defined(__Anthem__ModDock__) */
