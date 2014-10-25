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
#include <vector>

/*************************************************************************//*!
*
*  @brief       Delay line class.
*
****************************************************************************/

class Delay : public EffectUnit
{
    
public:
    
    /*! Parameters for modulation (have ModDocks) */
    enum Docks
    {
        DECAY_TIME,
        DECAY_RATE,
        FEEDBACK,
        DRYWET
    };
    
    typedef unsigned long size_t;
    
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
    
    Delay(const Delay& other);
    
    Delay& operator=(const Delay& other);
    
    virtual ~Delay() { }
    
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
    
    /*! @copydoc EffectUnit::setDryWet() */
    virtual void setDryWet(double dw);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the length of the delay line.
    *
    *  @param       delayLen The new length of the delay line, in seconds.
    *
    ****************************************************************************/
    
    virtual void setDelayLen(double delayLen);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current length of the delay line.
    *
    *  @return      The current delay line length, in seconds.
    *
    ****************************************************************************/
    
    virtual double getDelayLen() const;
    
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
    *  @brief       Returns the current decay time.
    *
    *  @return      The decay time, in seconds.
    *
    ****************************************************************************/
    
    virtual double getDecayTime() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the fade-out rate of the delay.
    *
    *  @param       decayRate The new rate of delay (between 0 and 1).
    *
    *
    ****************************************************************************/
    
    virtual void setDecayRate(double decayRate);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current decay rate.
    *
    *  @return      The decay rate.
    *
    ****************************************************************************/
    
    virtual double getDecayRate() const;
    
    /************************************************************************************************//*!
    *
    *  @brief       Determines the amount of output fed back into the delay line.
    *
    *  @param       feedbackLevel How much of the output values should be fed back (between 0 and 1).
    *
    *************************************************************************************************/
    
    virtual void setFeedback(double feedbackLevel);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current feedback value.
    *
    *  @return      The current feedback value.
    *
    ****************************************************************************/
    
    virtual double getFeedback() const;
    
    /************************************************************************************************//*!
    *
    *  @brief       Sets an offset in the delay line.
    *
    *  @param       offset The new offset, in samples.
    *
    *************************************************************************************************/
    
    virtual double offset(unsigned int offset);
    
protected:
    
    typedef std::vector<double> Buffer;
    
    typedef Buffer::iterator iterator;
    
    typedef Buffer::const_iterator const_iterator;
    
    /*! Calculates the _decayValue based on the decay rate, time and delay length*/
    void calcDecay_(double decayRate, double decayTime, double delayLen);
    
    /*! Increments and boundary checks the write pointer */
    void incr_();
    
    /*! Integral part of the read position */
    int readInt_;
    
    /*! Fractional part of the read position */
    double readFract_;
    
    /*! The pointer to the end of the current delay line  */
    iterator end_;
    
    /*! The pointer to the write index */
    iterator write_;
    
    /*! Size of the current delay line*/
    unsigned int delayLen_;
    
    /*! The attenuation value with which to multiply the output */
    double decayValue_;
    
    /*! The rate of decay, or fade-out of the delay signal */
    double decayRate_;
    
    /*! The total decay time */
    double decayTime_;
    
    /*! The value determining how much of the output signal is fed back into the delay line*/
    double feedback_;
    
    /*! The delay line */
    Buffer buffer_;
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
    *************************************************************************************************/
    
    Echo(double delayLen = 1,
         double decayTime = 4,
         double decayRate = 0.01,
         double feedbackLevel = 1,
         double capacity = 10);
    
    /************************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @details     A sample is processed by running the Delay::process() function on it and summing
    *               it with the original sample.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The new sample.
    *
    *************************************************************************************************/
    
    double process(double sample);
};

#endif /* defined(__Anthem__Delay__) */
