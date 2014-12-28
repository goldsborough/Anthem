/*********************************************************************************************//*!
*
*  @file        Flanger.hpp
*
*  @author      Peter Goldsborough
*
*  @date        28/12/2014
*
*  @brief       Defines the Flanger effect class.
*
*************************************************************************************************/

#ifndef Anthem_Flanger_hpp
#define Anthem_Flanger_hpp

#include "Units.hpp"

class Delay;
class LFO;

/************************************************************************************************//*!
*
*  @brief       Flanger class
*
*  @details     Implements a flanger/phaser/chorus effect. What it really is depends on the
*               various settings and parameters.
*
*************************************************************************************************/

class Flanger : public EffectUnit
{
public:
    
    /***********************************************************************************************//*!
    *
    *  @brief       Constructs a Flanger object.
    *
    *  @param       center The center delay time around which the Flanger oscillates.
    *
    *  @param       depth The maximum delay time added/subtracted from the center value.
    *
    *  @param       rate The rate in Hz at which the lfo adds/subtracts the depth from the center.
    *
    *  @param       feedback Controls the brightness/coloration of the produced sound.
    *
    ************************************************************************************************/
    
    Flanger(double center = 0.01,
            double depth = 0.005,
            double rate = 0.1,
            double feedback = 0);
    
    Flanger(const Flanger& other);
    
    ~Flanger();
    
    Flanger& operator= (const Flanger& other);
    
    /*************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @details     A sample is processed by first modulating the center
    *               delay time and then adding the input sample to the
    *               output sample of the delay line.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The new sample.
    *
    **************************************************************************/
    
    double process(double sample);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the center/nominal delay time.
    *
    *  @param       center The new center delay time in seconds.
    *
    **************************************************************************/
    
    void setCenter(double center);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the center/nominal delay time.
    *
    **************************************************************************/
    
    double getCenter() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the depth of oscillation.
    *
    *  @details     The depth controls the by what amount the center delay
    *               time is modulated. Essentially the amplitude of the
    *               modulation LFO.
    *
    *  @param       depth The new depth value, in seconds.
    *
    **************************************************************************/
    
    void setDepth(double depth);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the depth in seconds.
    *
    **************************************************************************/
    
    double getDepth() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the rate of oscillation.
    *
    *  @param       rate The new rate/frequency in Hertz.
    *
    **************************************************************************/
    
    void setRate(double rate);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current rate of oscillation.
    *
    **************************************************************************/
    
    double getRate() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the feedback level.
    *
    *  @details     Controls the brightness/coloration of the produced sound.
    *
    *  @param       feedback The new feedback value.
    *
    **************************************************************************/
    
    void setFeedback(double feedback);
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the current feedback level.
    *
    **************************************************************************/
    
    double getFeedback() const;
    
private:
    
    /*! Center delay time around which modulation occurs. */
    double center_;
    
    /*! Feedback level, controls coloration of sound. */
    double feedback_;
    
    /*! LFO to modulate center value. */
    std::unique_ptr<LFO> lfo_;
    
    /*! The delay line to produce the effect. */
    std::unique_ptr<Delay> delay_;
};

#endif
