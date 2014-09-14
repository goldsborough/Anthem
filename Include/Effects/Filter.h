/*********************************************************************************************//*!
*
*  @file        Filter.h
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       Defines the Filter class.
*
*************************************************************************************************/

#ifndef __Anthem__Filter__
#define __Anthem__Filter__

#include "Units.h"

/********************************************************************************************//*!
*
*  @brief       Bi-Quad Filter.
*
*  @details     This is Anthem's main filter class. It is an IIR, Bi-Quad
*               filter implemented in Direct Form II. Lots of great info
*               can be found here: http://goo.gl/XiA8jo
*
*               References:
*
*               + http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
*
*               + http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/
*
*  @todo        Possibly FIR filters - Users create frequency responses which are then
*               converted into filter kernels, with which the signal is then convoluted.
*               This would also solve the Gray noise problem in the Noise class.
*
*  @todo        Fix weird jump in amplitude when modulating cutoff
*
***********************************************************************************************/

class Filter : public EffectUnit
{

public:
    
    /*! Available ModDocks */
    enum DOCKS
    {
        CUTOFF,
        Q,
        GAIN,
        DRYWET
    };
    
    /*! Filter modes */
    
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
    
    Filter(unsigned short mode,
           double cutoff,
           double q = 1,
           double gain = 0);
    
    /*************************************************************************//*!
    *
    *  @brief       Filters a sample.
    *
    *  @details     Uses Direct Form II for the Bi-Quad filter.
    *
    *  @param       sample The sample to filter.
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
    
    void setMode(unsigned short mode);
    
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
    
    void setCutoff(double cutoff);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the Q factor.
    *
    *  @details     The Q factor more or less describes the bandwidth or slope
    *               of the transition band. Between 0.01 and 20.
    *
    *  @param       q The Q, or Quality, factor. http://goo.gl/94Neeg
    *
    ****************************************************************************/
    void setQ(double q);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the filter's overall gain.
    *
    *  @details     The gain is the filter's overall amplitude.
    *
    *  @param       gain The new gain.
    *
    ****************************************************************************/
    
    void setGain(double gain);
    
private:
    
    /*************************************************************************//*!
    *
    *  @brief       Recalcuates the filter coefficients.
    *
    *  @details     The function calculates the various filter coefficients
    *               according to the filter's parameters (Q factor, cutoff
    *               frequency, filter mode and gain).
    *
    ****************************************************************************/
    
    void _calcCoefs(short mode, double cutoff, double q, double gain);
    
    /*! ModDock initialization */
    void _initModDocks();
    
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

#endif /* defined(__Anthem__Filter__) */
