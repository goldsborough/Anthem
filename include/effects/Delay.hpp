/*********************************************************************************************//*!
*
*  @file        Delay.hpp
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

#include "Units.hpp"

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
    *  @param       delayLength The length of the delay line, in seconds.
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
    
    Delay(double delayLength = 1,
          double decayTime = 4,
          double decayRate = 0.001,
          double feedbackLevel = 0,
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
    
    /*! @copydoc EffectUnit::getDryWet() */
    virtual double getDryWet() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the length of the delay line.
    *
    *  @param       delayTime The new length of the delay line, in seconds.
    *
    ****************************************************************************/
    
    virtual void setDelayTime(double delayTime);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current length of the delay line.
    *
    *  @return      The current delay line length, in seconds.
    *
    ****************************************************************************/
    
    virtual double getDelayTime() const;
    
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
    void _calcDecay();
    
    /*! Increments and boundary checks the write pointer */
    inline void _writeAndIncrement(double sample);
    
    /*! Integral part of the read position */
    int _readIntegral;
    
    /*! Fractional part of the read position */
    double _readFractional;
    
    /*! The pointer to the write index */
    iterator _write;
    
    /*! The attenuation value with which to multiply the output */
    double _decayValue;
    
    /*! The rate of decay, or fade-out of the delay signal */
    double _decayRate;
    
    /*! The total decay time */
    double _decayTime;
    
    /*! The value determining how much of the output signal is fed back into the delay line */
    double _feedback;
    
    /*! The delay line */
    Buffer _buffer;
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
    AllPassDelay(const double& delayLength = 1,
                 const double& decayTime = 4,
                 const double& decayRate = 0.001,
                 const double& feedbackLevel = 1)
    : Delay(delayLength,decayTime,decayRate,feedbackLevel)
    { }
    
    /*! @copydoc Delay::process() */
    double process(double sample);
};
#endif /* defined(__Anthem__Delay__) */
