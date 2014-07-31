//
//  Filter.h
//  Vibe
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Filter__
#define __Vibe__Filter__

#include "EffectUnit.h"

/*************************************************************************//*!
*
*  @brief       Bi-Quad Filter.
*
*  @details     This is Vibe's main filter class. It is an IIR, Bi-Quad
*               filter implemented in Direct Form II. Lots of great
*               can be found here: http://goo.gl/XiA8jo
*
****************************************************************************/

class Filter : public EffectUnit
{

public:
    
    /*************************************************************************//*!
    *
    *  @brief   Filter modes.
    *
    ****************************************************************************/
    
    enum Mode
    {
        LOW_PASS,
        HIGH_PASS,
        BAND_PASS,
        BAND_REJECT,
        ALL_PASS,
        PEAK,
        LOW_SHELF,
        HIGH_SHELF
    };
    
    /*************************************************************************//*!
    *
    *  @brief       Constructs a filter object.
    *
    *  @param       mode The filter mode, see the Mode enum for the different modes.
    *
    *  @param       cutoff The cutoff frequency, in Hertz.
    *
    *  @param       q The Q, or Quality, factor. http://goo.gl/94Neeg
    *
    *  @param       gain The overall filter gain, comparable to amplitude.
    *
    ****************************************************************************/
    
    Filter(const unsigned short& mode,
           const double& cutoff,
           const double& q = 1,
           const double& gain = 1);
    
    /*************************************************************************//*!
    *
    *  @brief       Filters a sample.
    *
    *  @details     Uses Direct Form II for the Bi-Quad filter.
    *
    *  @param       sample The sample to filter.
    *
    *
    ****************************************************************************/
    
    double process(double sample);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the filter mode.
    *
    *  @details     See the Mode enum for the different filter types.
    *
    *  @param       mode The new mode.
    *
    ****************************************************************************/
    
    void setMode(const unsigned short& mode);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the cutoff frequency.
    *
    *  @details     The cutoff frequency is the frequency at which the filter
    *               actually starts reducing the amplitude.
    *
    *  @param       cutoff The new cutoff frequency, in Hertz.
    *
    ****************************************************************************/
    
    void setCutoff(const double& cutoff);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the Q factor.
    *
    *  @details     The Q factor more or less describes the bandwidth or slope
    *               of the transition band.
    *
    *  @param       q The Q, or Quality, factor. http://goo.gl/94Neeg
    *
    ****************************************************************************/
    void setQ(const double& q);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the filter's overall gain.
    *
    *  @details     The gain is the filter's overall amplitude.
    *
    *  @param       gain The new gain.
    *
    ****************************************************************************/
    
    void setGain(const short& gain);
    
private:
    
    /*************************************************************************//*!
    *
    *  @brief       Recalcuates the filter coefficients.
    *
    *  @details     The function calculates the various filter coefficients
    *               according to the filter's parameters (Q factor, cutoff
    *               frequency and filter mode).
    *
    ****************************************************************************/
    
    void _calcCoefs();
    
    /*! The filter mode */
    unsigned short _mode;
    
    /*! The cutoff frequency (frequency at which the filter starts acting) */
    double _cutoff;
    
    /*! The Q factor */
    double _q;
    
    /*! The overall gain/amplitude */
    double _gain;
    
    /*! The first delay buffer */
    double _delayA;
    
    /*! The second delay buffer */
    double _delayB;
    
    /*! The first output coefficient */
    double _coefA1;
    
    /*! The second output coefficient */
    double _coefA2;
    
    /*! The first input coefficient */
    double _coefB0;
    
    /*! The second input coefficient */
    double _coefB1;
    
    /*! The third input coefficient */
    double _coefB2;
};

#endif /* defined(__Vibe__Filter__) */
