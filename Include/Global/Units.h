/*********************************************************************************************//*!
*
*  @file        Units.h
*
*  @author      Peter Goldsborough
*
*  @date        09/09/2014
*
*  @brief       Abstract base classes for units.
*
*  @details     This file defines abstract base classes for practically all units in Anthem.
*
*************************************************************************************************/

#ifndef __Anthem__Units__
#define __Anthem__Units__

#include "Wavetable.h"

class ModDock;
class ModUnit;

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
    
    typedef unsigned short index_t;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a Unit.
    *
    *  @param       numDocks The number of ModDocks for the Unit.
    *
    *****************************************************************************************************/
    
    Unit(index_t numDocks = 0);
    
    Unit(const Unit& other);
    
    Unit& operator=(const Unit& other);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the number of ModDocks.
    *
    *  @return      The number of ModDocks.
    *
    *****************************************************************************************************/
    
    virtual std::vector<ModDock*>::size_type numDocks() const;
    
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
    
    virtual void setModUnitDepth(index_t dockNum,
                                 index_t modNum,
                                 double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the depth for a single modulation unit of a single dock.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @return      The depth of that modulation unit.
    *
    *****************************************************************************************************/
    
    virtual double getModUnitDepth(index_t dockNum, index_t modNum) const;
    
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns boolean whether or not a dock is in use.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @return      True if dock has at least one ModUnit, else false.
    *
    *****************************************************************************************************/
    
    virtual bool dockInUse(index_t dockNum) const;
    
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
    
    virtual void attachMod(index_t dockNum,
                           ModUnit* mod);
    
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
    
    virtual void detachMod(index_t dockNum,
                           index_t modNum);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Makes a ModUnit in a ModDock sidechain another's depth.
    *
    *  @details     Sidechaining means that one ModUnit sidechains/modulates another's depth
    *               value. Multiple mastery as well as multiple slavery is possible, so you
    *               can call setSidechain for any two indices you wish as long as the connection
    *               isn't already established, in which case nothing happens. Note that a ModUnit
    *               cannot simultaneously be a master of slaves and contribute to the ModDock's
    *               modulation value, it is either-or.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the ModUnit in the ModDock to be made master.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *************************************************************************************************/
    
    virtual void setSidechain(index_t dockNum, index_t master, index_t slave);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Cuts the sidechain connection between two ModUnits in a ModDock.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the master in the ModDock.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *  @throws      std::logic_error if the two ModUnits aren't connected.
    *
    *************************************************************************************************/
    
    virtual void unSidechain(index_t dockNum, index_t master, index_t slave);
    
    /*******************************************************************************************************//*!
    *
    *  @brief       Returns true if there is a sidechain connection between master and slave in a ModDock.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the master in the ModDock.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *  @return      Boolean, whether or not master is sidechaining slave.
    *
    ***********************************************************************************************************/
    
    virtual bool isSidechain(index_t dockNum, index_t master, index_t slave) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a ModDock is a master.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a master of one or more slaves, else false.
    *
    *************************************************************************************************/
    
    virtual bool isMaster(index_t dockNum, index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a Moddock is a slave.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a slave of one or more masters, else false.
    *
    *************************************************************************************************/
    
    virtual bool isSlave(index_t dockNum, index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the size of the ModDock.
    *
    *  @param       dockNum The index of the ModDock
    *
    *  @return      The number of ModUnits in the ModDock.
    *
    *************************************************************************************************/
    
    virtual unsigned long dockSize(index_t dockNum) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Deconstructs all ModDocks.
    *
    *************************************************************************************************/
    
    virtual ~Unit();
    
protected:
    
    /*! The std::vector holding the modulation docks. */
    std::vector<ModDock*> mods_;
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
    *  @param       numDocks The number of ModDocks for the EffectUnit.
    *
    *  @param       dryWet Initial dry/wet value (between 0 and 1), defaults to 1.
    *
    *****************************************************************************************************/
    
    EffectUnit(unsigned short numDocks = 0, double dryWet = 1);
    
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
    *  @brief       Sets the dry/wet parameter.
    *
    *  @param       dw The new dry/wet value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setDryWet(double dw);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the dry/wet parameter for the EffectUnit.
    *
    *  @param       dw The new dry/wet value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual double getDryWet() const;
    
protected:
    
    /*! Method that controls the dry/wet content of the signal */
    double dryWet_(double originalSample,
                   double processedSample);
    
    /*! Overloaded version of dryWet_ for setting a custom dryWet value */
    double dryWet_(double originalSample,
                   double processedSample,
                   double dryWetValue);
    
    /*! Dry/wet level */
    double dw_;
};

/*********************************************************************************************//*!
*
*  @brief       Base class for Units generating ticks or samples.
*
*  @details     This is the base class for all Units that generate samples, such as Oscillators,
*               Operators, Envelopes and more. Its members are a pure virtual method for generating
*               a sample, tick(), as well as an amplitude member, amp_, along with a method to
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
    *  @param       numDocks The number of ModDocks for the EffectUnit.
    *
    *  @param       amp An initial amplitude value between 0 and 1, defaults to 1.
    *
    *****************************************************************************************************/
    
    GenUnit(unsigned short numDocks = 0, double amp = 1);
    
    virtual ~GenUnit() { }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the current amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the current amplitude value.
    *
    *  @return      The amplitude.
    *
    *****************************************************************************************************/
    
    virtual double getAmp() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @return      The generated sample.
    *
    *****************************************************************************************************/
    
    virtual double tick() = 0;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Increments values in the GenUnit.
    *
    *****************************************************************************************************/
    
    virtual void increment() = 0;
    
protected:
    
    /*! The current amplitude value */
    double amp_;
};

class ModUnit : public Unit
{
    
public:
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a ModUnit.
    *
    *  @param       numDocks The number of ModDocks for the EffectUnit.
    *
    *  @param       amp An initial amplitude value between 0 and 1, defaults to 1.
    *
    *****************************************************************************************************/
    
    ModUnit(unsigned short numDocks = 0, double amp = 1);
    
    virtual ~ModUnit() { }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the ModUnit's amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the ModUnit's amplitude value.
    *
    *  @return      The amplitude.
    *
    *****************************************************************************************************/
    
    virtual double getAmp() const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Modulates a sample and returns the result.
    *
    *  @param       sample The sample to modulate.
    *
    *  @param       depth The modulation depth.
    *
    *  @param       maximum The maximum value that the sample may reach during modulation.
    *
    *  @return      The modulated sample.
    *
    *************************************************************************************************/
    
    virtual double modulate(double sample, double depth, double maximum) = 0;
    
protected:
    
    /*! The amplitude value */
    double amp_;
};

#endif /* defined(__Anthem__Units__) */
