/*********************************************************************************************//*!
*
*  @file        Reverb.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Defines the Reverb effect.
*
*************************************************************************************************/

#ifndef __Anthem__Reverb__
#define __Anthem__Reverb__

#include "Units.hpp"

#include <memory>

class Delay;
class AllPassDelay;

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
    
    /*********************************************************************************************************//*!
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
    
    Reverb(double reverbTime = 1, double reverbRate = 0.001, double dryWet = 0.1);
    
    Reverb(const Reverb& other);
    
    Reverb& operator= (const Reverb& other);
    
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
    
    /*! @copydoc EffectUnit::getDryWet() */
    double getDryWet() const;
    
private:
    
    /*! The input signal attenuation factor */
    double _attenuation;
    
    /*! The *suprise* reverb time */
    double _reverbTime;
    
    /*! Exactly that. */
    double _reverbRate;
    
    /*! The array of delay lines */
    std::unique_ptr<Delay[]> _delays;
    
    /*! The array of all-pass delays */
    std::unique_ptr<AllPassDelay[]> _allPasses;
};

#endif /* defined(__Anthem__Reverb__) */
