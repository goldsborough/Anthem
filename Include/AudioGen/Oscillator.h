/*********************************************************************************************//*!
*
*  @file        Oscillator.h
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       Defines the Oscillator class.
*
*************************************************************************************************/

#ifndef __Anthem__Oscillator__
#define __Anthem__Oscillator__

#include "Wavetable.h"

/*************************************************************************************************//*!
*
*  @brief       The Oscillator class.
*
*  @details     An Oscillator is the most basic audio-samples-generating unit. It interpolates
*               samples from a wavetable, which it gets from WavetableDB. It has methods to set
*               its frequency, wavetable, phase offset and more. It is the basis for LFOs and
*               Operators.
*
*****************************************************************************************************/

class Oscillator
{
    
public:
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an Oscillator object.
    *
    *  @param       wt The initial wavetable id, defaults to -1, or WavetableDB::NONE.
    *
    *  @param       frq The Oscillator's initial frequency.
    *
    *  @param       phaseOffset A phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    Oscillator(short wt = -1, double frq = 1, short phaseOffset = 0);
    
    virtual ~Oscillator() { }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the current sample.
    *
    *****************************************************************************************************/
    
    virtual double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Increments the oscillator in the wavetable. 
    *
    *****************************************************************************************************/
    
    virtual void increment();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the oscillator's frequency.
    *
    *  @param       Hz The new frequency, in Hertz.
    *
    *  @see         getFrequency()
    *
    *****************************************************************************************************/
    
    virtual void setFrequency(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the oscillator's frequency.
    *
    *  @return      The oscillator's current frequency.
    *
    *  @see         setFrequency()
    *
    *****************************************************************************************************/
    
    virtual double getFrequency() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gives the oscillator's phase an offset.
    *
    *  @param       degrees The degrees by which to offset the phase by.
    *
    *****************************************************************************************************/
    
    virtual void setPhaseOffset(short degrees);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Oscillator's phase offset.
    *
    *  @return      The current phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    virtual double getPhaseOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the oscillator's wavetable
    *
    *  @param       wt The new wavetable id, usually a member of WavetableDB::Wavetables.
    *
    *****************************************************************************************************/
    
    virtual void setWavetable(short wt);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the oscillator's wavetable id.
    *
    *  @return      The wavetable id.
    *
    *****************************************************************************************************/
    
    virtual short getWavetableID() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Resets the oscillator's phase.
    *
    *****************************************************************************************************/
    
    virtual void reset();
    
protected:
    
    /*! The current frequency */
    double freq_;
    
    /*! The current wavetable index */
    double ind_;
    
    /*! The wavetable index increment per sample */
    double indIncr_;
    
    /*! The current phase offset value */
    double phaseOffset_;
    
    /*! The wavetable member currently in use */
    Wavetable wt_;
};

#endif /* defined(__Anthem__Oscillator__) */
