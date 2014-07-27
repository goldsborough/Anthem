//
//  Delay.h
//  Vibe
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Delay__
#define __Vibe__Delay__

#include "EffectUnit.h"

/*************************************************************************//*!
*
*  @brief       Delay line class.
*
****************************************************************************/

class Delay : public EffectUnit
{
    
public:
    
    typedef unsigned long size_t;
    typedef double* iterator;
    
    /***********************************************************************************************//*!
    *
    *  @brief       Constructs a Delay object.
    *
    *  @param       delayLen The length of the delay line, in seconds.
    *
    *  @param       decayTime The time for the attenuation of the delay to reach inaudible levels.
    *
    *  @param       decayRate The rate at which the delay becomes inaudible.
    *
    *  @param       feedbackLevel The amount of output from the delay line to feed back.
    *
    **************************************************************************************************/
    
    Delay(const double& delayLen,
          const double& decayTime = 10,
          const double& decayRate = 0.005,
          const double& feedbackLevel = 1);
    
    virtual ~Delay();
    
    /*************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @details     A sample is processed by pushing it into the delay line,
    *               whilst outputting a delayed sample that was stored at
    *               that position previously.
    *
    *  @param       sample The sample to be processed by the delay line.
    *
    ****************************************************************************/
    
    virtual void process(double& sample);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the length of the delay line.
    *
    *  @param       delayLen The new length of the delay line.
    *
    ****************************************************************************/
    
    virtual void setDelayLen(const double& delayLen);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the time for the delay to fade out.
    *
    *  @param       decayTime The fade-out time, in seconds.
    *
    ****************************************************************************/
    
    virtual void setDecayTime(unsigned int decayTime);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the fade-out rate of the delay.
    *
    *  @param       decayRate The new rate of delay (between 0 and 1).
    *
    *
    ****************************************************************************/
    
    virtual void setDecayRate(const double& decayRate);
    
    /************************************************************************************************//*!
    *
    *  @brief       Determines the amount of output fed back into the delay line.
    *
    *  @param       feedbackLevel How much of the output values should be fed back (between 0 and 1).
    *
    *************************************************************************************************/
    
    virtual void setFeedback(const double& feedbackLevel);
    
protected:
    
    /*! The pointer to the end of the whole delay line (total capacity, not tap point) */
    iterator _end;
    
    /*! The pointer to the current delay line index */
    iterator _curr;
    
    /*! The current end of the delay line, or tap point */
    iterator _tap;
    
    /*! The attenuation value with which to multiply the output */
    double _decayValue;
    
    /*! The rate of decay, or fade-out of the delay signal */
    double _decayRate;
    
    /*! The value determining how much of the output signal is fed back into the delay line*/
    double _feedback;
    
    /*! The delay line */
    double * _buffer;
};

/************************************************************************************************//*!
*
*  @brief       Echo effect class.
*
*  @details     The Echo effect class is a derivation of the Delay class that adds the delay line's
*               output samples to the currently passed on samples.
*
*************************************************************************************************/

struct Echo : public Delay
{
    /***********************************************************************************************//*!
    *
    *  @brief       Constructs an Echo object.
    *
    *  @param       delayLen The length of the echo, in seconds.
    *
    *  @param       decayTime The time for the attenuation of the echo to reach inaudible levels.
    *
    *  @param       decayRate The rate at which the echo becomes inaudible.
    *
    *  @param       feedbackLevel The amount of output from the echo to feed back.
    *
    **************************************************************************************************/
    
    Echo(const double& delayLen,
         const double& decayTime = 10,
         const double& decayRate = 0.005,
         const double& feedbackLevel = 1)
    : Delay(delayLen,decayTime,decayRate,feedbackLevel)
    { }
    
    /*************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @details     A sample is processed by pushing it into the Ehco's delay line,
    *               whilst outputting a delayed sample and adding it to the
    *               inputted sample (instead of only assigning the input to the
    *               outputted sample, as is the case with the Delay class)
    *
    *  @param       sample The sample to be processed by the delay line.
    *
    ****************************************************************************/
    
    void process(double& sample);
};

#endif /* defined(__Vibe__Delay__) */
