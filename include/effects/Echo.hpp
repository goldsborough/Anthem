/*********************************************************************************************//*!
*
*  @file        Echo.hpp
*
*  @author      Peter Goldsborough
*
*  @date        28/12/2014
*
*  @brief       Defines the Echo effect class.
*
*************************************************************************************************/

#ifndef Anthem_Echo_hpp
#define Anthem_Echo_hpp

#include "Delay.hpp"

/************************************************************************************************//*!
*
*  @brief       Echo class
*
*  @details     This class is the same as the Delay class, except for the fact that it sums the
*               the input with the output of the delay line before returning.
*
*************************************************************************************************/

struct Echo : public Delay
{
    /************************************************************************************************//*!
    *
    *  @brief       Constructs an Echo object.
    *
    *  @param       delayLen The length of the delay in seconds.
    *
    *  @param       decayTime The time for the signal to fade out after the impulse stops.
    *
    *  @param       decayRate The rate of decay/fade-out.
    *
    *  @param       feedbackLevel How much of the output to feed back into the delay line.
    *
    *  @param       capacity The total capacity of the delay line, in seconds (must exceed length).
    *
    *************************************************************************************************/
    
    Echo(double delayLen = 1,
         double decayTime = 4,
         double decayRate = 0.01,
         double feedbackLevel = 1,
         double capacity = 10);
    
    ~Echo();
    
    /************************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @details     A sample is processed by calling the Delay::process() function on it and summing
    *               it with the original sample.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The new sample.
    *
    *************************************************************************************************/
    
    double process(double sample);
};


#endif
