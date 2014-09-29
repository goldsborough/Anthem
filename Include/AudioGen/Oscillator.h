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

#include "Units.h"

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

class Oscillator : public GenUnit
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
    *  @param       amp The initial amplitude.
    *
    *  @param       phaseOffset A phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    Oscillator(short wt = -1, double frq = 1,
               double amp = 1, short phaseOffset = 0);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *****************************************************************************************************/
    
    double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the oscillator's frequency.
    *
    *  @param       Hz The new frequency, in Hertz.
    *
    *  @see         getFreq()
    *
    *****************************************************************************************************/
    
    void setFreq(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the oscillator's frequency.
    *
    *  @return      The oscillator's current frequency.
    *
    *  @see         setFreq()
    *
    *****************************************************************************************************/
    
    double getFreq() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gives the oscillator's frequency an offset, in semitones.
    *
    *  @param       semis The number of semitones to offset by (±48).
    *
    *****************************************************************************************************/
    
    void setSemitoneOffset(short semitoneOffset);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Oscillator's current semitone offset, if any.
    *
    *  @return      The semitone offset.
    *
    *****************************************************************************************************/
    
    short getSemitoneOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gives the Oscillator's frequency an offset, in cents.
    *
    *  @param       cents The number of cents to offset by, between 0 and 100.
    *
    *****************************************************************************************************/
    
    void setCentOffset(short centOffset);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Oscillator's current cent offset, if any.
    *
    *  @return      The cent offset.
    *
    *****************************************************************************************************/
    
    short getCentOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gives the oscillator's phase an offset.
    *
    *  @param       degrees The degrees by which to offset the phase by.
    *
    *****************************************************************************************************/
    
    void setPhaseOffset(short degrees);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Oscillator's phase offset.
    *
    *  @return      The current phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    short getPhaseOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the oscillator's wavetable
    *
    *  @param       wt The new wavetable id, usually a member of WavetableDB::Wavetables.
    *
    *****************************************************************************************************/
    
    void setWavetable(short wt);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Resets the oscillator's phase.
    *
    *****************************************************************************************************/
    
    void reset();
    
private:
    
    /*! The current frequency */
    double _freq;
    
    /*! The current wavetable index */
    double _ind;
    
    /*! The wavetable index increment per sample */
    double _indIncr;
    
    /*! The current phase offset value */
    double _phaseOffset;
    
    /*! The current semitone offset of the frequency */
    double _semitoneOffset;
    
    /*! The current cent offset of the frequency */
    double _centOffset;
    
    /*! The wavetable member currently in use */
    Wavetable _wt;
};

#endif /* defined(__Anthem__Oscillator__) */
