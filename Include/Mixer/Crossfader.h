/*********************************************************************************************//*!
*
*  @file        Units.h
*
*  @author      Peter Goldsborough
*
*  @date        15/09/2014
*
*  @brief       CrossfadeUnits and Crossfader.
*
*************************************************************************************************/

#ifndef __Anthem__Crossfade__
#define __Anthem__Crossfade__

#include "Units.h"
#include "Sample.h"

namespace CrossfadeTypes
{ enum Types { LINEAR, SINE, SQRT }; }

/*********************************************************************************************//*!
*
*  @brief       Basic crossfading unit.
*
*  @details     This class stores retrievable crossfade values. There are three different tables
*               one for linear crossfading (values add up to 1), one for crossfading with the
*               sine function and one using the sqrt function. The last two don't add up to 1
*               so there is an option to scale them.
*
*************************************************************************************************/

class CrossfadeUnit
{
    
public:
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a CrossfadeUnit.
    *
    *  @param       type The crossfading type, usually a member of the CrossfadeTypes namespace.
    *
    *  @param       scalingEnabled Enable or disable scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       offset An initial value for the crossfading. Between -100 and 100.
    *
    *************************************************************************************************/
    
    CrossfadeUnit(unsigned short type = 0,
                  bool scalingEnabled = false,
                  unsigned short offset = 0);
    
    virtual ~CrossfadeUnit() { }
    
    /*********************************************************************************************//*!
    *
    *  @brief       Enables or disables scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       scaling Boolean whether or not to scale.
    *
    *************************************************************************************************/
    
    virtual void setScalingEnabled(bool scalingEnabled);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the CrossfadeUnit's type.
    *
    *  @param       type The new crossfading type, usually a member of the CrossfadeTypes enum.
    *
    *************************************************************************************************/
    
    virtual void setType(unsigned short type);

    /*********************************************************************************************//*!
    *
    *  @brief       Sets the crossfade value for the two sides.
    *
    *  @param       value The crossfade value, between -100 (full left) and +100 (full right).
    *
    *************************************************************************************************/
    
    virtual void setValue(short value);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns left value.
    *
    *************************************************************************************************/
    
    virtual double left() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns right value.
    *
    *************************************************************************************************/
    
    virtual double right() const;
    
protected:
    
    /*! Checks if scaling is necessary and returns scaled value if appropriate. 
        Scaling is appropriate if scalign is enabled and if the type is either SQRT or SINE. */
    double _scale(const double& value) const;
    
    /*! Current index in the lookup table */
    unsigned short _index;
    
    /*! The crossfading type */
    unsigned short _type;
    
    /*! Boolean whether or not scaling is enabled*/
    bool _scalingEnabled;
    
    /*! Array of Sample tables for crossfading values */
    Sample _tables [3][201];
};

/*********************************************************************************************//*!
*
*  @brief       Crossfader that ticks a crossfaded value of two GenUnits
*
*  @details     The Crossfader class is an extension of the CrossfadeUnit class in the sense
*               that it has two GenUnit pointers as members. It also has a tick() method,
*               which then returns a crossfaded value of the two GenUnit ticks.
*
*************************************************************************************************/

class Crossfader : public CrossfadeUnit, public GenUnit
{
    
public:
    
    /*! ModDocks */
    enum Docks { VALUE };
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a Crossfader object.
    *
    *  @param       type The crossfading type, usually a member of the CrossfadeTypes namespace.
    *
    *  @param       scalingEnabled Enable or disable scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       offset An initial value for the crossfading. Between -100 and 100.
    *
    *  @param       left A GenUnit pointer for the left side, optionally.
    *
    *  @param       right A GenUnit pointer for the right side, optionally.
    *
    *************************************************************************************************/
    
    Crossfader(unsigned short type = 0,
               bool scalingEnabled = false,
               unsigned short offset = 0,
               GenUnit* left = 0,
               GenUnit* right = 0);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the crossfade value for the two sides.
    *
    *  @param       value The crossfade value, between -100 (full left) and +100 (full right).
    *
    *************************************************************************************************/
    
    void setValue(short value);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @details     A sample is generated by first retrieving the two samples from the GenUnits
    *               and then crossfading between them. The crossfaded samples are then added up
    *               and returned.
    *
    *  @return      The crossfaded sample.
    *
    *************************************************************************************************/
    
    double tick();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the left GenUnit.
    *
    *  @param       unit A pointer to a GenUnit.
    *
    *************************************************************************************************/
    
    void setLeftUnit(GenUnit* unit);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the right GenUnit.
    *
    *  @param       unit A pointer to a GenUnit.
    *
    *************************************************************************************************/
    
    void setRightUnit(GenUnit* unit);
    
private:
    
    /*! The current crossfading value */
    short _value;
    
    /*! Initializes the ModDock for the VALUE parameter */
    void _initModDocks();
    
    /*! The pointer to the left GenUnit */
    GenUnit* _leftUnit;
    
    /*! The pointer to the right GenUnit */
    GenUnit* _rightUnit;
};

#endif /* defined(__Anthem__Crossfade__) */
