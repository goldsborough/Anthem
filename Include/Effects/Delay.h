/*********************************************************************************************//*!
*
*  @file        Delay.h
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       Delay lines and all-pass delays.
*
*************************************************************************************************/

#ifndef __Anthem__Delay__
#define __Anthem__Delay__

#include "Units.h"

/*************************************************************************//*!
*
*  @brief       Delay line class.
*
****************************************************************************/

class Delay : public EffectUnit
{
    
public:
    
    /*! Modulatable ModDocks */
    enum DOCKS
    {
        TIME,
        RATE,
        FEEDBACK,
        DRYWET
    };
    
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
    *  @param       capacity The total capacity of the delay line, in seconds (must exceed length). 
    *
    **************************************************************************************************/
    
    Delay(double delayLen = 1,
          double decayTime = 4,
          double decayRate = 0.01,
          double feedbackLevel = 1,
          double capacity = 10);
    
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
    *  @return      The new sample.
    *
    ****************************************************************************/
    
    virtual double process(double sample);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the length of the delay line.
    *
    *  @param       delayLen The new length of the delay line.
    *
    ****************************************************************************/
    
    virtual void setDelayLen(double delayLen);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the time for the delay to fade out.
    *
    *  @param       decayTime The fade-out time, in seconds.
    *
    ****************************************************************************/
    
    virtual void setDecayTime(double decayTime);
    
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
    
    /************************************************************************************************//*!
    *
    *  @brief       Sets an offset in the delay line.
    *
    *  @param       offset The new offset.
    *
    *************************************************************************************************/
    
    virtual double offset(const unsigned int& offset);
    
protected:
    
    /*! ModDock initialization */
    void _initModDocks();
    
    /*! Calculates the _decayValue based on the decay rate, time and delay length*/
    void _calcDecay(double decayRate, double decayTime, double delayLen);
    
    /*! Increments and boundary checks the write pointer */
    void _incr();
    
    /*! Integral part of the read position */
    int _readInt;
    
    /*! Fractional part of the read position */
    double _readFract;
    
    /*! Pointer to the actual end of the delay line (total capacity) */
    iterator _capacity;
    
    /*! The pointer to the end of the current delay line  */
    iterator _end;
    
    /*! The pointer to the write index */
    iterator _write;
    
    /*! Actual, total size of the delay line */
    const unsigned int _delayCapacity;
    
    /*! Size of the current delay line, equal to _end - _buffer */
    unsigned int _delayLen;
    
    /*! The attenuation value with which to multiply the output */
    double _decayValue;
    
    /*! The rate of decay, or fade-out of the delay signal */
    double _decayRate;
    
    /*! The total decay time */
    double _decayTime;
    
    /*! The value determining how much of the output signal is fed back into the delay line*/
    double _feedback;
    
    /*! The delay line */
    double * _buffer;
};

/************************************************************************************************//*!
*
*  @brief       All-pass-delay class
*
*  @details     This class is an extension of the Delay class and necessary to implement the 
*               Schroeder Reverb. It essentially changes the color of the tone (by filtering).
*
*************************************************************************************************/

struct AllPassDelay : public Delay
{
    AllPassDelay(const double& delayLen = 10,
                 const double& decayTime = 0,
                 const double& decayRate = 0.001,
                 const double& feedbackLevel = 1)
    : Delay(delayLen,decayTime,decayRate,feedbackLevel)
    { }
    
    /*! @copydoc Delay::process() */
    double process(double sample);
};
#endif /* defined(__Anthem__Delay__) */
