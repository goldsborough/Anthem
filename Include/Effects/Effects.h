/*********************************************************************************************//*!
*
*  @file        Effects.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Defines various effects.
*
*************************************************************************************************/

#ifndef __Anthem__Effects__
#define __Anthem__Effects__

#include "Delay.h"

/************************************************************************************************//*!
*
*  @brief       A reverb-effect class.
*
*  @details     The reverb algorithm currently implemented is the well-known and moderately
*               popular Schroeder-Reverb, which is made up of four parallel comb filters/delay
*               lines and two all-pass filters in series. It is a good first reverb effect but
*               will definitely need re-implementation some time (soon).
*
*************************************************************************************************/

class Reverb : public EffectUnit
{
    
public:
    
    enum Docks
    {
        REVERB_TIME,
        REVERB_RATE,
        DRYWET
    };
    
    /************************************************************************************************************//*!
    *
    *  @brief       Constructs a Reverb object.
    *
    *  @param       reverbTime The time for the reverb to decay/fade out, after an impulse stops (in seconds).
    *
    *  @param       reverbRate The rate of decay (for the reverb to fade out).
    *
    *  @param       dryWet How much of the reverberated signal should be mixed into the final signal.
    *
    ************************************************************************************************************/
    
    Reverb(double reverbTime, double reverbRate = 0.001, double dryWet = 0.1);
    
    ~Reverb();
    
    /*! @copydoc EffectUnit::process() */
    double process(double sample);
    
    /************************************************************************************************//*!
    *
    *  @brief       Sets the reverberation length/time.
    *
    *  @param       reverbTime The new reverberation time, in seconds.
    *
    *************************************************************************************************/
    
    void setReverbTime(double reverbTime);
    
    /************************************************************************************************//*!
    *
    *  @brief       Returns the reverberation length/time.
    *
    *  @return      The reverberation time, in seconds.
    *
    *************************************************************************************************/
    
    double getReverbTime() const;
    
    /************************************************************************************************//*!
    *
    *  @brief       Sets the reverberation (decay) rate.
    *
    *  @param       reverbTime The new reverberation decay rate (between 0 and 1).
    *
    *************************************************************************************************/
    
    void setReverbRate(double reverbRate);
    
    /************************************************************************************************//*!
    *
    *  @brief       Returns the reverberation rate.
    *
    *  @return      The reverberation rate.
    *
    *************************************************************************************************/
    
    double getReverbRate() const;
    
    /************************************************************************************************//*!
    *
    *  @brief       Controls the mixture between original signal and reverberated signal.
    *
    *  @details     This value should usually stick around 0.1 for best sound and least distortion.
    *               Note that when the reverb becomes more wet, the input signal is attenuated 
    *               proportionally, so if you have dry/wet at 0.5, the input signal will also be
    *               halved, otherwise the signal's amplitude would blow up, big time.
    *
    *  @param       dw The dry/wet ratio (between 0 and 1).
    *
    *************************************************************************************************/
    
    void setDryWet(double dw);
    
private:
    
    /*! The input signal attenuation factor */
    double attenuation_;
    
    /*! The *suprise* reverb time */
    double reverbTime_;
    
    /*! Exactly that. */
    double reverbRate_;
    
    /*! The array of delay lines */
    Delay** delays_;
    
    /*! The array of all-pass delays */
    AllPassDelay** allPasses_;
    
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
         double capacity = 10)
    : Delay(delayLen,decayTime,decayRate,feedbackLevel,capacity)
    { }
    
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
    
    double process(double sample)
    { return sample + Delay::process(sample); }
};

class LFO;

class Flanger : public EffectUnit
{
public:
    
    Flanger(const double& center = 0.01,
            const double& depth = 0.01,
            const double& rate = 0.15,
            const double& feedback = 0);
    
    ~Flanger();
    
    double process(double sample);
    
    void setFeedback(const double& feedback);
    
    void setCenter(const double& center);
    
    void setDepth(const double& depth);
    
    void setRate(const double& rate);
    
private:
    
    double center_;
    double depth_;
    double feedback_;
    
    LFO* lfo_;
    Delay delay_;
};

#endif /* defined(__Anthem__Effects__) */
