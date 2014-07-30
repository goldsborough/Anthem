//
//  Reverb.h
//  Vibe
//
//  Created by Peter Goldsborough on 30/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Reverb__
#define __Vibe__Reverb__

#include "EffectUnit.h"

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
    
    /************************************************************************************************//*!
    *
    *  @brief       Constructs a Reverb object.
    *
    *  @param       reverbTime The time for the reverb to decay/fade out, after an impulse stops.
    *
    *  @param       reverbRate The rate of decay (for the reverb to fade out).
    *
    *  @param       dryWet How much of the reverberated signal should be mixed into the final signal.
    *
    *************************************************************************************************/
    
    Reverb(const double& reverbTime,
           const double& reverbRate = 0.001,
           const double& dryWet = 0.1);
    
    ~Reverb();
    
    /*! @copydoc EffectUnit::process() */
    
    double process(const double& sample);
    
    
    /************************************************************************************************//*!
    *
    *  @brief       Sets the reverberation length/time.
    *
    *  @param       reverbTime The new reverberation time, in seconds.
    *
    *************************************************************************************************/
    
    void setReverbTime(const double& reverbTime);
    
  
    /************************************************************************************************//*!
    *
    *  @brief       Sets the reverberation (decay) rate.
    *
    *  @param       reverbTime The new reverberation decay rate (between 0 and 1).
    *
    *************************************************************************************************/
    
    void setReverbRate(const double& reverbRate);
    
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
    
    void setDryWet(const double& dw);
    
private:

    /*! The input signal attenuation factor */
    double _attenuation;
    
    /*! The array of delay lines*/
    Delay** _delays;
    
    /*! The array of all-pass delays */
    AllPassDelay** _allPasses;
    
};

#endif /* defined(__Vibe__Reverb__) */
