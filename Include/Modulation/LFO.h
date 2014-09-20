/*********************************************************************************************//*!
*
*  @file        LFO.h
*
*  @author      Peter Goldsborough
*
*  @date        14/10/2014
*
*  @brief       LFOs and LFOUnits.
*
*************************************************************************************************/

#ifndef __Anthem__LFO__
#define __Anthem__LFO__

#include "Units.h"
#include "EnvSeg.h"
#include "Crossfader.h"

class Oscillator;

/*************************************************************************************************//*!
*
*  @brief       A Low Frequency Oscillator (LFO) class.
*
*  @details     The LFO class is essentially an interface to an Oscillator which additionally
*               implements a set of ModDocks for modulation of LFOs (by other modulation  units)
*               and modulates() samples instead of tick()ing them.
*
*****************************************************************************************************/

class LFO : public ModUnit
{
    
public:
    
    /*! Various ModDocks available for modulation */
    enum Docks
    {
        AMP,
        RATE,
        PHASE
    };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an LFO object.
    *
    *  @param       wt The LFO's wavetable, typically a member of WavetableDB::Wavetables.
    *
    *  @param       rate The LFO's initial rate or frequency.
    *
    *  @param       phaseOffset An initial phase offset, in degrees (0-360).
    *
    *****************************************************************************************************/
    
    LFO(short wt = 0, double rate = 1, double amp = 1, double phaseOffset = 0);
    
    ~LFO();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Modulates a sample and returns the result.
    *
    *  @param       sample The sample to modulate.
    *
    *  @param       minBoundary The minimum boundary to check for.
    *
    *  @param       maxBoundary The maximum boundary to check for.
    *
    *  @return      The modulated sample.
    *
    *************************************************************************************************/
    
    double modulate(double sample, double minBoundary, double maxBoundary);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the LFO's wavetable
    *
    *  @param       wt The new wavetable id, typically a member of WavetableDB::Wavetables.
    *
    *****************************************************************************************************/
    
    void setWavetable(short wt);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the LFO's frequency.
    *
    *  @param       Hz The new frequency, in Hertz.
    *
    *****************************************************************************************************/
    
    void setRate(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the LFO's rate.
    *
    *  @return      The rate.
    *
    *****************************************************************************************************/
    
    double getRate() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gives the LFO's phase an offset.
    *
    *  @param       degrees The degrees by which to offset the phase by.
    *
    *****************************************************************************************************/
    
    void setPhaseOffset(double degrees);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the LFO's phase offset.
    *
    *  @return      The phase offset, in degrees.
    *
    *****************************************************************************************************/
    
    double getPhaseOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the LFO's amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the LFO's amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    double getAmp() const;
    
private:
    
    /*! The LFO's rate, or frequency */
    double _rate;
    
    /*! The LFO's phase offset, if any */
    double _phaseOffset;
    
    /*! The oscillator object that LFOs wrap around */
    Oscillator* _osc;
};


/****************************************************************************************************//*!
*
*  @brief       Wrapper around EnvSegSeq for specific LFO purposes.
*
*  @details     The main difference between EnvSegSeq and LFOSeq is that LFOSeq has a rate parameter
*               that sets the rate or frequency of the entire Envelope sequence. Internally, changing
*               this "rate" is equivalent to appropriately adjusting the length of each EnvSeg.
*
********************************************************************************************************/

class LFOSeq : public EnvSegSeq
{
    
public:
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an LFOSeq object.
    *
    *  @param       seqLength The amount of individual envelope segments. Defaults to 20.
    *
    *****************************************************************************************************/
    
    LFOSeq(unsigned short seqLength = 20, double rate = 1);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the rate of the envelope sequence.
    *
    *  @details     Setting the rate of the sequence is equivalent to appropriately adjusting the
    *               length of each individual envelope segment. Given a frequency f, the period or
    *               in this case the length of the sequence is 1/f. This value is then divided by
    *               the number of segments to give the length of an individual segment, which is then
    *               adjusted for each segment.
    *
    *  @param       Hz The rate/frequency to set, in Hz.
    *
    *****************************************************************************************************/
   
    void setRate(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the current rate.
    *
    *  @return      The rate, in Hertz.
    *
    *****************************************************************************************************/
    
    double getRate() const;
    
private:
    
    /*! The current rate */
    double _rate;
};

/****************************************************************************************************//*!
*
*  @brief       The main "LFO" interface for Anthem.
*
*  @details     In Anthem, an LFO is relatively large unit, consisting of two normal LFOs (Oscillators)
*               and two LFOSeqs (EnvSegSeqs). The user can switch between the normal LFO mode and the
*               Sequencer mode. In both cases, the user can then crossfade between the two respective
*               units, e.g. between LFO A and LFO B.
*
********************************************************************************************************/

struct LFOUnit : public GenUnit
{
    
    /*! LFO modes - lfo mode and sequencer mode */
    enum Modes { LFO_MODE, SEQ_MODE };

    /*! The two units each mode has to crossfade between */
    enum Units { A, B };
    
    /****************************************************************************************************//*!
    *
    *  @brief       Constructs an LFO Unit.
    *
    *  @param       mode The initial mode, defaults to LFO_MODE
    *
    ********************************************************************************************************/
    LFOUnit(unsigned short mode = 0);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Sets the LFOUnit's mode.
    *
    *  @param       mode The new mode, usually a member of LFOUnit::Modes.
    *
    ********************************************************************************************************/
    void setMode(unsigned short mode);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @details     An LFOUnit sample is generated by multiplying the crossfaded tick from the two units with
    *               the internal amplitude and the envelope tick.
    *
    *  @return      The generated sample.
    *
    ********************************************************************************************************/
    double tick();

    /****************************************************************************************************//*!
    *
    *  @brief       An LFOUnit specific envelope.
    *
    ********************************************************************************************************/
    struct Env : public EnvSegSeq
    {
        /*! The two parts of the envelope: SegA / SegB \ Together /\ */
        enum Segments { SEG_A, SEG_B };
        
        /*! The adjustable points of the envelope */
        enum Points { BEG, MID, END };
        
        Env();
        
        /****************************************************************************************************//*!
        *
        *  @brief       Sets the level of one of the envelope's points.
        *
        *  @param       point The point of which to set the level. Usually a member of Env::Points.
        *
        *  @param       lvl The new level, between 0 and 1.
        *
        ********************************************************************************************************/
        void setEnvLevel(short point, double lvl);
        
    } env;

    /*! The Crossfader that fades between the A and B units */
    Crossfader fader;

    /*! The step sequencer lfos, activated with setMode() and Modes::SEQ_MODE */
    LFOSeq lfoSeqs[2];

    /*! The normal lfos, activated with setMode() and Modes::LFO_MODE */
    LFO lfos[2];
};

#endif /* defined(__Anthem__LFO__) */
