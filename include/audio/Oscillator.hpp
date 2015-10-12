/*********************************************************************************************//*!
*
*  @file        Oscillator.hpp
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

#include <memory>

class Wavetable;

/*************************************************************************************************//*!
*
*  @brief       The Oscillator class.
*
*  @details     An Oscillator is the most basic audio-sample-generating unit. It interpolates
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
    *  @param       wt The initial wavetable id, defaults to 0, or WavetableDatabase::SINE.
    *
    *  @param       frq The Oscillator's initial frequency.
    *
    *  @param       phaseOffset A phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    Oscillator(unsigned short wavetable = 0,
               double freq = 1,
               short phaseOffset = 0);
    
    Oscillator(const Oscillator& other);
    
    Oscillator& operator= (const Oscillator& other);
    
    virtual ~Oscillator();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the current sample.
    *
    *****************************************************************************************************/
    
    virtual double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Updates the oscillator's index in the wavetable. 
    *
    *****************************************************************************************************/
    
    virtual void update();
    
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
    
    virtual void setWavetable(unsigned short id);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the oscillator's wavetable id.
    *
    *  @return      The wavetable id.
    *
    *****************************************************************************************************/
    
    virtual std::shared_ptr<Wavetable> getWavetable() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Resets the oscillator's phase.
    *
    *****************************************************************************************************/
    
    virtual void reset();
    
protected:
    
    /*! Increments the oscillator's index with value. */
    void _increment(double value);
    
    /*! The current frequency */
    double _freq;
    
    /*! The current wavetable index */
    double _index;
    
    /*! The wavetable index increment per sample */
    double _incr;
    
    /*! The current phase offset value */
    double _phaseOffset;
    
    /*! The wavetable member currently in use */
    std::shared_ptr<Wavetable> _wavetable;
};

#endif /* defined(__Anthem__Oscillator__) */
