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

#include "Oscillator.h"
#include "Units.h"
#include "EnvSeg.h"
#include "Crossfader.h"

/*************************************************************************************************//*!
*
*  @brief       A Low Frequency Oscillator (LFO) class.
*
*  @details     The LFO class is essentially an interface to an Oscillator which additionally
*               implements a set of ModDocks for modulation of LFOs (by other modulation units)
*               and modulates() samples instead of tick()ing them.
*
*****************************************************************************************************/

class LFO : public ModUnit, public Oscillator
{
    
public:
    
    /*! Various ModDocks available for modulation */
    enum Docks
    {
        FREQ,
        PHASE,
        AMP
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
    
    LFO(short wt = -1, double rate = 1, double amp = 1, double phaseOffset = 0);
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the LFO's frequency.
    *
    *  @param       Hz The new frequency, in Hertz.
    *
    *  @see         getFreq()
    *
    *****************************************************************************************************/
    
    void setFreq(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the LFO's frequency.
    *
    *  @return      The LFO's current frequency.
    *
    *  @see         setFreq()
    *
    *****************************************************************************************************/
    
    double getFreq() const;
    
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
    *  @brief       Sets the LFO's oscillator's amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the LFO's oscillator's amplitude value.
    *
    *  @return      The amplitude value.
    *
    *****************************************************************************************************/
    
    double getAmp() const;
};

/****************************************************************************************************//*!
*
*  @brief       Derived class from EnvSegSeq for specific LFO purposes.
*
*  @details     The main difference between EnvSegSeq and LFOSeq is that LFOSeq has a rate parameter
*               that sets the rate or frequency of the entire Envelope sequence. Internally, changing
*               this "rate" is equivalent to appropriately adjusting the length of each EnvSeg.
*
********************************************************************************************************/

class LFOSeq : public ModEnvSegSeq
{
    
public:
    
    /*! ModDocks for segments */
    enum Seg_Docks
    {
        MOD_FREQ,
        MOD_PHASE,
        MOD_DEPTH
    };
    
    /*! ModDocks for LFOSeq (entire unit, not segments) */
    enum Docks { RATE };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an LFOSeq object.
    *
    *  @param       seqLength The amount of individual envelope segments. Defaults to 10.
    *
    *****************************************************************************************************/
    
    LFOSeq(unsigned short seqLength = 10, double rate = 1);
    
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
    *  @brief       Returns the current envelope rate.
    *
    *  @return      The rate, in Hertz.
    *
    *****************************************************************************************************/
    
    double getRate() const;
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      seg The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *********************************************************************************/
    
    void setSegRate(seg_t seg, double rate);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the depth of a segment's modulator.
    *
    *  @details     Do not confuse with setModUnitDepth_Seg to set the depth of a modulator in a
    *               segment's wave's ModDock. 
    *
    *  @param       seg The segment to set the depth for.
    *
    *  @param       depth The new depth value.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    void setModDepth(seg_t seg, double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the depth of a segment's modulator.
    *
    *  @param       seg The segment to return the depth from.
    *
    *  @return      The depth of a segment's modulator.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    double getModDepth(seg_t seg) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the rate of a segment's modulator.
    *
    *  @param       seg The segment to set the rate for.
    *
    *  @param       freq The new rate value.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    void setModFreq(seg_t seg, double freq);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the rate of a segment's modulator.
    *
    *  @param       seg The segment to return the rate from.
    *
    *  @return      The rate of a segment's modulator.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    double getModFreq(seg_t seg) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the phase offset of a segment's modulator.
    *
    *  @param       seg The segment to set the phase offset for.
    *
    *  @param       degrees The new phase offset value, in degrees.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    void setModPhaseOffset(seg_t seg, double degrees);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the phase offset of a segment's modulator.
    *
    *  @param       seg The segment to return the phase offset from.
    *
    *  @return      The phase offset of a segment's modulator.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    double getModPhaseOffset(seg_t seg);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets a segment's lfo's wavetable.
    *
    *  @param       seg The segment to set the lfo's wavetable for.
    *
    *  @param       wt The new wavetable id, usually a member of WavetableDB::Wavetables.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    void setModWavetable(seg_t seg, short wt);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the wavetable id of a segment's modulator.
    *
    *  @param       seg The segment to return the depth from.
    *
    *  @return      The wavetable id of a segment's modulator.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    short getModWavetableID(seg_t seg) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Adds a segment to the sequence.
    *
    *  @details     Maximum number of segments is 10.
    *
    *****************************************************************************************************/
    
    void addSegment();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Removes the last segment from the sequence.
    *
    *  @throws      std::runtime_error if segment number is 0.
    *
    *****************************************************************************************************/
    
    void removeLastSegment();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Removes a segment from the sequence.
    *
    *  @param       seg The segment to return the depth from.
    *
    *  @throws      std::invalid_argument if segment number invalid.
    *
    *****************************************************************************************************/
    
    void removeSegment(seg_t seg);
    
    /*! @copydoc EnvSegSeq::increment() */
    void increment();
    
    /*! @copydoc EnvSegSeq::setSegLen() */
    void setSegLen(seg_t seg, unsigned long ms);
    
    /*! @copydoc ModEnvSegSeq::setModUnitDepth_Seg() */
    void setModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum, double depth);
    
    /*! @copydoc ModEnvSegSeq::getModUnitDepth_Seg() */
    double getModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum) const;
    
    /*! @copydoc ModEnvSegSeq::attachMod_Seg() */
    void attachMod_Seg(seg_t segNum, index_t dockNum, ModUnit* mod);
    
    /*! @copydoc ModEnvSegSeq::detachMod_Seg() */
    void detachMod_Seg(seg_t segNum, index_t dockNum, index_t modNum);
    
    /*! @copydoc ModEnvSegSeq::setSidechain_Seg() */
    void setSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave);
    
    /*! @copydoc ModEnvSegSeq::unSidechain_Seg() */
    void unSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave);
    
    /*! @copydoc ModEnvSegSeq::isSidechain_Seg() */
    bool isSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave) const;
    
    /*! @copydoc ModEnvSegSeq::isMaster_Seg() */
    bool isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const;
    
    /*! @copydoc ModEnvSegSeq::isSlave_Seg() */
    bool isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const;
    
    /*! @copydoc ModEnvSegSeq::dockSize_Seg() */
    unsigned long dockSize_Seg(seg_t segNum, index_t dockNum) const;
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
private:
    
    struct Mod
    {
        Mod(double f = 1)
        : freq(f)
        { }
        
        LFO lfo;
        
        double freq;
    };
    
    std::vector<Mod> lfos_;
    
    void setScaledModFreq_(seg_t seg);
    
    void resizeSegsFromRate_(double rate);
    
    /*! The current rate of the sequence */
    double rate_;
};

/****************************************************************************************************//*!
*
*  @brief       The main "LFO" interface for Anthem.
*
*  @details     In Anthem, an LFO is relatively large unit, consisting of two normal LFOs (Oscillators)
*               and two LFOSeqs (EnvSegSeqs). The user can switch between the normal LFO mode and the
*               Sequencer mode. In both cases, the user can then crossfade between the two respective
*               units, e.g. between LFO A and LFO B. There is also a dedicated Envelope.
*
********************************************************************************************************/

class LFOUnit : public ModUnit
{
    
public:
    
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
    
    LFOUnit(unsigned short mode = LFO_MODE);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Sets the LFOUnit's mode.
    *
    *  @param       mode The new mode, usually a member of LFOUnit::Modes.
    *
    ********************************************************************************************************/
    
    void setMode(bool mode);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Returns the LFOUnit's current mode.
    *
    *  @return      Boolean mode, either LFO_Mode or SEQ_Mode;
    *
    ********************************************************************************************************/
    
    bool getMode() const;
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);

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
    
private:
    
    bool _mode;
};

#endif /* defined(__Anthem__LFO__) */
