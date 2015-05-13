/*********************************************************************************************//*!
*
*  @file        Crossfader.hpp
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

#include "Units.hpp"
#include "Sample.hpp"
#include "LookupTable.hpp"

#include <memory>

using Pantable = LookupTable<Sample>;

/*********************************************************************************************//*!
*
*  @brief       Basic crossfading unit.
*
*  @details     This class stores retrievable crossfade values with a left() and a right()
*               method to retrieve the respective channel values and a setValue() method to
*               to adjust the crossfading/panning value from -100 (all left) to 100 (all right).
*
*************************************************************************************************/

class CrossfadeUnit
{
    
public:
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a CrossfadeUnit.
    *
    *  @param       type The crossfading type, usually a member of the Pantables::Type enum.
    *
    *  @param       scalingEnabled Enable or disable scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       offset An initial value for the crossfading. Between -100 and 100.
    *
    *************************************************************************************************/
    
    CrossfadeUnit(unsigned short type = 0,
                  bool scalingEnabled = false,
                  unsigned short offset = 0);
    
    CrossfadeUnit(const CrossfadeUnit& other);
    
    CrossfadeUnit& operator= (const CrossfadeUnit& other);
    
    virtual ~CrossfadeUnit();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Enables or disables scaling to 1 for sine and sqrt crossfading.
    *
    *  @param       scaling Boolean whether or not to scale.
    *
    *************************************************************************************************/
    
    virtual void enableScaling(bool scalingEnabled);
    
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
    *  @param       type The new crossfading type, usually a member of the Types enum.
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
    
    virtual void setValue(double value);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the current value.
    *
    *  @return      The current value.
    *
    *************************************************************************************************/
    
    virtual double getValue() const;
    
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
    
    /*! Current index in the lookup table. Double because of interpolation. */
    double index_;
    
    /*! The crossfading type */
    unsigned short type_;
    
    /*! Whether or not a scaled table is used */
    bool scalingEnabled_;
    
    /*! The current Sample from the Pantable */
    Sample curr_;
    
    /*! Array of Sample tables for crossfading values */
    std::shared_ptr<Pantable> table_;
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
    
    /*! @copydoc CrossfadeUnit::setValue() */
    void setValue(double value);
    
    /*! @copydoc CrossfadeUnit::getValue() */
    double getValue() const;
    
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
    ModUnit* leftUnit_;
    
    /*! The pointer to the right GenUnit */
    ModUnit* rightUnit_;
};

#endif /* defined(__Anthem__Crossfade__) */
