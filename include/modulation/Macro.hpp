/*********************************************************************************************//*!
*
*  @file        Macro.hpp
*
*  @author      Peter Goldsborough
*
*  @date        25/10/2014
*
*  @brief       Macro class.
*
*************************************************************************************************/

#include "Units.hpp"

/*********************************************************************************************//*!
*
*  @brief       Macro control class.
*
*  @details     The Macro class is a ModUnit that lets the user set a value between -1 and +1
*               which then can be used at various places throughout the system, thereby giving
*               the user the possibility to set many different controls with just one "macro"
*               control. Also has a ModDock for its value.
*
*************************************************************************************************/

class Macro : public ModUnit
{
    
public:
    
    /*! Available ModDocks */
    enum Docks { VALUE };
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a Macro object.
    *
    *  @param       value An initial value, defaults to 0.
    *
    *************************************************************************************************/
    
    Macro(double value = 0);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the Macro's value.
    *
    *  @param       value The new value, between -1 and +1.
    *
    *  @throws      std::invalid_argument if value not in range.
    *
    *************************************************************************************************/
    
    void setValue(double value);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the Macro's value.
    *
    *  @return      The current value, between -1 and +1. Double precision.
    *
    *************************************************************************************************/
    
    double getValue() const;
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
private:
    
    /*! The current value */
    double value_;
};