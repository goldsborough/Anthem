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
*               implements a set of ModDocks for modulation of LFOs (by other modulation  units).
*
*****************************************************************************************************/

class LFO : public GenUnit
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
    
    LFO(short wt = 0, double rate = 1, double phaseOffset = 0);
    
    ~LFO();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @details     An LFO sample is generated by first checking for modulation by any ModDocks and
    *               then returning the internal oscillator's tick.
    *
    *****************************************************************************************************/
    
    double tick();
    
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
    *  @brief       Gives the LFO's phase an offset.
    *
    *  @param       degrees The degrees by which to offset the phase by.
    *
    *****************************************************************************************************/
    
    void setPhaseOffset(double degrees);
    
private:
    
    /*! Allocates new ModDocks */
    void _initModDocks();
    
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
    
    LFOSeq(unsigned int seqLength = 20);
    
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
    
private:
    
    /*! The amount of segments, stored to divide the whole sequence period by it */
    const unsigned int _seqLen;
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

    enum Modes { LFO_MODE, SEQ_MODE };

    enum Units { A, B };
    
    LFOUnit(unsigned short mode = 0);
    
    void setMode(unsigned short mode);
    
    double tick();

    struct Env : public EnvSegSeq
    {
        /*! The two parts of the envelope: SegA / SegB \ Together /\ */
        enum Segments { SEG_A, SEG_B };
        
        enum Points { BEG, MID, END };
        
        Env();
        
        void setEnvLevel(short point, double lvl);
        
    } env;

    Crossfader fader;

    LFOSeq lfoSeqs[2];

    LFO lfos[2];
};

#endif /* defined(__Anthem__LFO__) */
