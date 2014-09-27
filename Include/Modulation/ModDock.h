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
    *  @brief       Constructs a ModDock.
    *
    *  @param       masterDepth The inital master depth, defaults to 1.
    *
    *************************************************************************************************/
    
    ModDock(double masterDepth = 1);
    
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
    *  @param       minBoundary The minimum boundary to check for.
    *
    *  @param       maxBoundary The maximum boundary to check for.
    *
    *  @return      The modulated sample.
    *
    *************************************************************************************************/
    
    double modulate(double sample,
                    double minBoundary,
                    double maxBoundary);
    
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
        explicit ModItem(ModUnit* modUnit, double dpth = 1)
                 : mod(modUnit), depth(dpth)
                 { }
    
        ModUnit* mod;
        
        double depth;
    };
    
    /*! Vector of ModItems */
    std::vector<ModItem> _mods;
    
    /*! The master depth value */
    double _masterDepth;
};

#endif /* defined(__Anthem__ModDock__) */
