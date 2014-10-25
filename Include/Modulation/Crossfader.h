/*********************************************************************************************//*!
*
*  @file        Crossfader.h
*
*  @author      Peter Goldsborough
*
*  @date        15/09/2014
*
*  @brief       CrossfadeUnit and Crossfader.
*
*************************************************************************************************/

#ifndef __Anthem__Crossfade__
#define __Anthem__Crossfade__

#include "Units.h"
#include "Sample.h"

namespace CrossfadeTypes
{
    /*! Different types of crossfading*/
    enum { LINEAR, SINE, SQRT };
}

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
    *  @brief       Whether or not scaling is enabled.
    *
    *  @return      Boolean, whether or not scaling is enabled.
    *
    *************************************************************************************************/
    
    virtual bool scalingEnabled() const;
    
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
    *  @brief       Returns the current type.
    *
    *  @return      The current type.
    *
    *************************************************************************************************/
    
    virtual unsigned short getType() const;
    
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
    *  @brief       Returns the current value.
    *
    *  @return      The current value.
    *
    *************************************************************************************************/
    
    virtual short getValue() const;
    
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

class Crossfader : public CrossfadeUnit, public ModUnit
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
    *  @param       left A ModUnit pointer for the left side, optionally.
    *
    *  @param       right A ModUnit pointer for the right side, optionally.
    *
    *************************************************************************************************/
    
    Crossfader(unsigned short type = 0,
               bool scalingEnabled = false,
               unsigned short offset = 0,
               ModUnit* left = 0,
               ModUnit* right = 0);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the crossfade value for the two sides.
    *
    *  @param       value The crossfade value, between -100 (full left) and +100 (full right).
    *
    *************************************************************************************************/
    
    void setValue(short value);
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the left ModUnit.
    *
    *  @param       unit A pointer to a ModUnit.
    *
    *************************************************************************************************/
    
    void setLeftUnit(ModUnit* unit);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the left ModUnit.
    *
    *  @return      A pointer to the left ModUnit.
    *
    *************************************************************************************************/
    
    ModUnit* getLeftUnit() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the right ModUnit.
    *
    *  @param       unit A pointer to a ModUnit.
    *
    *************************************************************************************************/
    
    void setRightUnit(ModUnit* unit);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the right ModUnit.
    *
    *  @return      A pointer to the right ModUnit.
    *
    *************************************************************************************************/
    
    ModUnit* getRightUnit() const;
    
private:

    /*! The pointer to the left GenUnit */
    ModUnit* _leftUnit;
    
    /*! The pointer to the right GenUnit */
    ModUnit* _rightUnit;
};

#endif /* defined(__Anthem__Crossfade__) */
