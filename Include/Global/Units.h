/*********************************************************************************************//*!
*
*  @file        Units.h
*
*  @author      Peter Goldsborough
*
*  @date        09/10/2014
*
*  @brief       Abstract base classes for units.
*
*  @details     This file defines abstract base classes for practically all units in Anthem.
*
*************************************************************************************************/

#ifndef __Anthem__GenUnits__
#define __Anthem__GenUnits__

#include <vector>
#include "Wavetable.h"

class ModDock;
class GenUnit;

/*********************************************************************************************//*!
*
*  @brief       Basic unit - any object with parameters.
*
*  @details     This is the base class for any unit with parameters, thus including operators,
*               filters, effects, envelopes and more. All Units have an std::vector of ModDocks
*               that allow for modulation of any Unit's parameters by a GenUnit class (any class
*               that ticks samples and does not process samples i.e. LFOs or envelopes and not
*               filters or effects).
*
*************************************************************************************************/

class Unit
{
    
public:
    
    typedef unsigned char docknum_t;
    typedef unsigned short index_t;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the macro depth for all modulation units.
    *
    *  @details     This function reduces the overall depth of all modulation units currently in use.
    *
    *  @param       dockNum The dock number of the dock to set the master depth for (e.g. Operator::AMP).
    *
    *  @param       depth The new depth level, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setDockMasterDepth(docknum_t dockNum, double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the depth for a single modulation unit of a single dock.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @param       depth The new depth value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setDepth(docknum_t dockNum,
                          index_t modNum,
                          double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Attaches a new modulation unit to a dock.
    *
    *  @param       dockNum The dock number of the dock to attach the new modulation unit to.
    *
    *  @param       modNum The number of the modulation space into which to place the mod unit.
    *
    *  @param       mod A pointer to a GenUnit or one of its derived class.
    *
    *  @see         detachMod()
    *
    *****************************************************************************************************/
    
    virtual void attachMod(docknum_t dockNum,
                           index_t modNum,
                           GenUnit* mod);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Detaches a modulation unit from a dock.
    *
    *  @param       dockNum The dock number of the dock to detach from.
    *
    *  @param       modNum The number of the modulation space to detach.
    *
    *  @see         attachMod()
    *
    *****************************************************************************************************/
    
    virtual void detachMod(docknum_t dockNum,
                           index_t modNum);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Deconstructs all ModDocks.
    *
    *************************************************************************************************/
    
    virtual ~Unit();
    
protected:
    
    /*! Pure virtual method to ensure ModDock initialization. Can be left empty if unapplicable. */
    virtual void _initModDocks() = 0;
    
    /*! The std::vector holding the modulation docks. */
    std::vector<ModDock*> _mods;
};

/*********************************************************************************************//*!
*
*  @brief       Base class for any Unit that processes rather than ticks samples.
*
*  @details     The EffectUnit class is the base class for all Units that process ticks, such
*               as filters or effects. They differ from GenUnits in the sense that they do not
*               produce samples on their own but only "process" existing samples and modulate
*               them in some way such as filtering. Also, EffectUnits have a dry/wet parameter.
*
*  @see         GenUnit
*
*************************************************************************************************/

class EffectUnit : public Unit
{
    
public:
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructor for the EffectUnit class.
    *
    *  @param       dryWet Initial dry/wet value (between 0 and 1), defaults to 1.
    *
    *****************************************************************************************************/
    
    EffectUnit(const double& dryWet = 1)
    : _dw(dryWet)
    { }
    
    virtual ~EffectUnit() { }
    
    /*********************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The processed sample.
    *
    *************************************************************************************************/
    
    virtual double process(double sample) = 0;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the dry/wet parameter for the EffectUnit.
    *
    *  @param       dw The new dry/wet value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setDryWet(const double& dw);
    
protected:
    
    /*! Method that controls the dry/wet content of the signal */
    double _dryWet(const double& originalSample, const double& processedSample);
    
    /*! Dry/wet level */
    double _dw;
};

/*********************************************************************************************//*!
*
*  @brief       Base class for Units generating ticks or samples.
*
*  @details     This is the base class for all Units that generate samples, such as Oscillators,
*               Operators, Envelopes and more. Its members are a pure virtual method for generating
*               a sample, tick(), as well as an amplitude member, _amp, along with a method to
*               set the amplitude.
*
*  @see         EffectUnit
*
*************************************************************************************************/

class GenUnit : public Unit
{
    
public:
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a GenUnit.
    *
    *  @param       amp An initial amplitude value between 0 and 1, defaults to 1.
    *
    *****************************************************************************************************/
    
    GenUnit(const double& amp = 1) { setAmp(amp); }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the GenUnit's amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setAmp(const double& amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @return      The generated sample.
    *
    *****************************************************************************************************/
    
    virtual double tick() = 0;
    
    virtual ~GenUnit() { }
    
protected:
    
    /*! The amplitude value */
    double _amp;
};

#endif /* defined(__Anthem__GenUnits__) */
