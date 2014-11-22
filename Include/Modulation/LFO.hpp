/*********************************************************************************************//*!
*
*  @file        LFO.hpp
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

#include "Oscillator.hpp"
#include "EnvSeg.hpp"

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
    *  @param       wt The LFO's wavetable, typically a member of WavetableDatabase::Wavetables.
    *
    *  @param       rate The LFO's initial rate or frequency.
    *
    *  @param       phaseOffset An initial phase offset, in degrees (0-360).
    *
    *****************************************************************************************************/
    
    LFO(short wt = 0, double rate = 1, double amp = 1, double phaseOffset = 0);
    
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
    
    void setFrequency(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the LFO's frequency.
    *
    *  @return      The LFO's current frequency.
    *
    *  @see         setFreq()
    *
    *****************************************************************************************************/
    
    double getFrequency() const;
    
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
    enum Docks { AMP, RATE };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an LFOSeq object.
    *
    *  @param       seqLength The amount of individual envelope segments. Defaults to 10.
    *
    *****************************************************************************************************/
    
    LFOSeq(unsigned short seqLength = 5, double rate = 1);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the amplitude value.
    *
    *  @return      The amplitude value.
    *
    *****************************************************************************************************/
    
    double getAmp() const;
    
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
    *  @param       wt The new wavetable id, usually a member of WavetableDatabase::Wavetables.
    *
    *  @throws      std::invalid_argument if seg out of range.
    *
    *****************************************************************************************************/
    
    void setModWavetable(seg_t seg, unsigned short wt);
    
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
    
    unsigned short getModWavetableID(seg_t seg) const;
    
    /****************************************************************************************************//*!
    *
    *  @brief       Returns the segments' length.
    *
    *  @details     The length is the same for all LFOSeqs and depends on the rate. Computed in setRate().
    *
    *  @return      The segment's length.
    *
    *  @see      setRate()
    *
    ********************************************************************************************************/
    
    unsigned long getSegLen() const;
    
    /******************************************************************************//*!
    *
    *  @brief      Adds a segment to the sequence.
    *
    *********************************************************************************/
    
    void addSegment();
    
    /******************************************************************************//*!
    *
    *  @brief      Adds a segment to the sequence.
    *
    *  @param      pos The position where to insert the new segment.
    *
    *********************************************************************************/
    
    void insertSegment(seg_t pos);
    
    /******************************************************************************//*!
    *
    *  @brief      Adds a segment to the sequence.
    *
    *  @param      pos The position where to insert the new segment.
    *
    *  @param      seg The EnvSeg to insert.
    *
    *********************************************************************************/
    
    void insertSegment(seg_t pos, const EnvSeg& seg);
    
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
    *  @brief       Removes the last segment from the sequence.
    *
    *  @throws      std::invalid_argument if segment number invalid.
    *
    *****************************************************************************************************/
    
    void removeSegment(seg_t seg);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Converts cycles per second to cycles per segment.
    *
    *  @details     If you call setModFreq() you're inputting the cycles per segment but that value
    *               has to be converted to cycles per second for the modulating LFO/Oscillator. That's
    *               what this function is for.
    *
    *  @param       The cycles per second value, in Hertz.
    *
    *  @return      The cycles per segment value.
    *
    *****************************************************************************************************/
    
    double getScaledModFreqValue(double freq) const;
    
    /*! @copydoc EnvSegSeq::increment() */
    void increment();
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
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
    
private:
    
    struct LFOSeq_LFO
    {
        LFOSeq_LFO(double f = 1)
        : freq(f)
        { }
        
        LFO lfo;
        
        double freq;
    };
    
    /*! Vector of above Mod structs */
    std::vector<LFOSeq_LFO> lfos_;
    
    /*! Calls getScaledModFreqValue() for seg */
    void setScaledModFreq_(seg_t seg);
    
    /*! Resets the length of all segments according 
        to the sequence's rate parameter */
    void resizeSegsFromRate_(double rate);
    
    /*! The current rate of the sequence */
    double rate_;
    
    /*! Length of the segments */
    unsigned long segLen_;
};

class Crossfader;

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

class LFOUnit : public ModUnit
{
    
public:
    
    /*! LFO modes - lfo mode and sequencer mode */
    enum Modes { LFO_MODE, SEQ_MODE };
    
    /*! One of the two units of a mode (e.g. LFO A and B)*/
    enum Units { A, B };
    
    /*! Available ModDocks */
    enum Docks { AMP };
    
    /****************************************************************************************************//*!
    *
    *  @brief       Constructs an LFO Unit.
    *
    *  @param       mode The initial mode, defaults to LFO_MODE
    *
    ********************************************************************************************************/
    
    LFOUnit(unsigned short mode = LFO_MODE);
    
    LFOUnit(const LFOUnit& other);
    
    LFOUnit& operator= (const LFOUnit& other);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the amplitude value.
    *
    *  @param       amp The new amplitude value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    void setAmp(double amp);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Gets the amplitude value.
    *
    *  @return      The amplitude value.
    *
    *****************************************************************************************************/
    
    double getAmp() const;
    
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
    
    /****************************************************************************************************//*!
    *
    *  @brief       Returns one of the LFOUnit's LFOSeqs.
    *
    *  @param       unit The unit number, LFOUnit::A or ::B.
    *
    *  @return      A reference to the LFOSeq.
    *
    ********************************************************************************************************/
    
    LFOSeq& seqs(bool unit);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Returns one of the LFOUnit's LFOs.
    *
    *  @param       unit The unit number, LFOUnit::A or ::B.
    *
    *  @return      A reference to the LFO.
    *
    ********************************************************************************************************/
    
    LFO& lfos(bool unit);
    
    /****************************************************************************************************//*!
    *
    *  @brief       Returns the LFOUnit's crossfader.
    *
    *  @return      A reference to the Crossfader object.
    *
    ********************************************************************************************************/
    
    Crossfader& fader();
    
    /*! @copydoc GenUnit::increment() */
    void increment();
    
    /*! @copydoc ModUnit::modulate() */
    double modulate(double sample, double depth, double maximum);
    
private:
    
    /*! The Crossfader that fades between the A and B units */
    std::unique_ptr<Crossfader> fader_;
    
    /*! The step sequencer lfos, activated with setMode() and Modes::SEQ_MODE */
    LFOSeq lfoSeqs_ [2];
    
    /*! The normal lfos, activated with setMode() and Modes::LFO_MODE */
    LFO lfos_ [2];
    
    bool mode_;
};

#endif /* defined(__Anthem__LFO__) */
