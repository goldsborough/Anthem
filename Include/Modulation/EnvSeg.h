/*********************************************************************************************//*!
*
*  @file        EnvSeg.h
*
*  @author      Peter Goldsborough
*
*  @date        18/10/2014
*
*  @brief       EnvSeg and EnvSegSeq classes.
*
*************************************************************************************************/

#ifndef __Anthem__EnvSeg__
#define __Anthem__EnvSeg__

#include "Units.h"
#include <vector>

class LFO;

/******************************************************************************//*!
 *
 *  @brief      An envelope segment
 *
 *  @details    The EnvSeg class is a single segment of a EnvSegSeq. It can be
 *              be either an attack, decay or a sustain segment and increment or
 *              decrement either exponentially, logarithmically or linearly in
 *              case of being a decay or an attack segment. Additionally, it has
 *              an internal modulation oscillator / LFO that can modulate the
 *              envelope values according to the waveform.
 *
 *********************************************************************************/

class EnvSeg : public GenUnit
{
    
public:
    
    typedef unsigned long len_t;
    
    /**********************************************************************************************//*!
     *
     *  @brief      Initalizes an Envelope segment
     *
     *  @param      startAmp the start amplitude of the segment
     *
     *  @param      endAmp the end amplitude of the segment
     *
     *  @param      len the length of the segment in samples
     *
     *  @param      segR The rate of the segment. 1 = Lin, 0 <= rate < 1 = Exp, 1 < rate <= 2 = Log
     *
     *  @param      modW The modulating wave, should be a directly calculated type for best results
     *
     *  @param      modDpth The depth of the modulation, 1 = full, 0 = none and everything inbetween
     *
     *  @param      modR The modulation rate/frequency, must be between 0 and 10
     *
     *  @param      maxLen The maximum length of the segment in samples, this should not really be changed
     *
     **************************************************************************************************/
    
    EnvSeg(double startAmp = 0,
           double endAmp = 0,
           len_t len = 0,
           double segR = 1,
           int modW = -1,
           double modDpth = 1,
           unsigned char modR = 1);
    
    ~EnvSeg();
    
    /******************************************************************//*!
     *
     *  @brief      Ticks an envelope value
     *
     *  @details    Modulates the value if _modWave != None returns _endLevel
     *              amplitude if _sample (count) larger than _len
     *
     *  @return     double precision floating point envelope value
     *
     *********************************************************************/
    
    double tick();
    
    /**********************************************************************//*!
     *
     *  @brief      Sets the length of the segment
     *
     *  @param      sampleLen Length of the segment in __samples__.
     *
     *  @exception  Throws std::invalid_argument if len > _maxLen (60 seconds)
     *
     *************************************************************************/
    
    // In samples
    void setLen(len_t sampleLen);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the length of the segment.
    *
    *  @return     The length of the segment, in samples.
    *
    ****************************************************************************/
    
    len_t getLen() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the rate of the segment
    *
    *  @details     The rate of the segment determines whether the the segment
    *               is exponential (0 to 1), linear (exactly 1) or logarithmic
    *               (1 to 2). The rate increases away from 1, so 0.1 would be
    *               equally exponential as 1.9 would be logarithmic. Note that
    *               the logarithmic scale is later on converted to the equivalent
    *               ranging from 0 to 1, this scale is provided so that there
    *               need not be a second parameter. It finally calls _calcRate()
    *               to do the math.
    *
    *  @param       rate The rate of the segment. 0 ≤ x < 1 is exponential, 1 is
    *               linear and 1 < x ≤ 2 is logarithmic.
    *
    *  @see         _calcRate()
    *
    *  @exception   Throws std::invalid_argument if the range is not between 0 and 2
    *
    ****************************************************************************/
    
    void setRate(double rate);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the current rate.
    *
    *  @return     The rate.
    *
    ****************************************************************************/
    
    double getRate() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the end amplitude of the segment.
    *
    *  @details     Sets _endLevel and calls _calcLevel and _calcRate to
    *               re-calculate the increment.
    *
    *  @param       lv The level. Must be between 0 and 1.
    *
    *  @see         _calcRate()
    *
    *  @see         _calcLevel()
    *
    *  @exception   Throws std::invalid_argument if lv is not between 0 and 1.
    *
    ****************************************************************************/
    
    void setEndLevel(double lv);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the ending level of the segment.
    *
    *  @return     The end level.
    *
    ****************************************************************************/
    
    double getEndLevel() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the starting amplitude of the segment.
    *
    *  @details     It sets _startLevel and calls _calcLevel and _calcRate to
    *               re-calculate the increment.
    *
    *  @param       lv The level. Must be between 0 and 1.
    *
    *  @see         _calcRate()
    *
    *  @see         _calcLevel()
    *
    *  @exception   Throws std::invalid_argument if lv is not between 0 and 1.
    *
    ****************************************************************************/
    
    void setStartLevel(double lv);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the ending level of the segment.
    *
    *  @return     The end level.
    *
    ****************************************************************************/

    double getStartLevel() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Convenience function for setting a sustain amplitude.
    *
    *  @details     Calls setStartLevel and setEndLevel with the parameter.
    *
    *  @param       The level.
    *
    *  @see         setStartLevel()
    *
    *  @see         setEndLevel()
    *
    ****************************************************************************/
    
    void setBothLevels(double lv);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the modulation wave acting on the envelope segment.
    *
    *  @details     The _modWave member is updated and the internal oscillator's
    *               wave is set. Since the segment should begin and end as would
    *               the unmodulated segment, the phase of certain waves has to be
    *               offset accordingly. Sine and Triangle waves are offset 90° and
    *               square waves are offset 215°.
    *
    *  @param       modW A Wavetable::Modes member.
    *
    ****************************************************************************/
    
    void setModWave(int modW);
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the rate of the modulation of the segment.
    *
    *  @details     It is important to note that this rate, ranging from 0 to
    *               to 255, is not the frequency in the sense of cycles per
    *               per second, but rather oscillations per __segment__, so
    *               a rate of 10 means there will be 10 cycles, or oscillations
    *               going through the segment. This means that the rate can
    *               actually be modified either through this function or just
    *               by changing the length.
    *
    *  @param       rate The modulation rate, ranging from 0 to 255.
    *
    *  @see         _calcModFreq()
    *
    *  @see         setLen()
    *
    ****************************************************************************/
    
    void setModRate(unsigned short rate);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the mod wave's rate.
    *
    *  @return     The mod wave's rate.
    *
    ****************************************************************************/
    
    double getModRate() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Sets the depth of the modulation.
    *
    *  @details     This setting, ranging from 0 to 1, determines how much of
    *               of the modulation goes through. The function itself just
    *               sets the private _modDepth member, it is at each tick in
    *               the
    *
    *  @param       dpth The depth between 0 and 1
    *
    *  @exception   Throws std::invalid_argument if dpth less than 0 or greater 1
    *
    ****************************************************************************/
    
    void setModDepth(double dpth);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the mod wave's depth.
    *
    *  @return     The mod wave's depth.
    *
    ****************************************************************************/
    
    double getModDepth() const;
    
    /*************************************************************************//*!
    *
    *  @brief       Resets the envelope segment.
    *
    *  @details     Resets the sample count to 0 and calls _calcRate() to 
    *               reset the increment.
    *
    *  @see         _calcRate()
    *
    ****************************************************************************/
    
    void reset();
    
private:
    
    /***********************************************************************************//*!
    *
    *  @brief   Rate of segment. Attack, decay or sustain. Just for internal reference.
    *
    ***********************************************************************************/
    
    enum ADS
    {
        ATK,
        DEC,
        SUST
    };
    
    /***********************************************************************************//*!
    *
    *  @brief   Type of segment. Attack, decay or sustain. Just for internal reference.
    *
    ***********************************************************************************/
    
    enum Type
    {
        LIN,
        LOG,
        EXP
    };
    
    /*************************************************************************//*!
    *
    *  @brief   Calculates the rate of the segment
    *
    *  @details Calculates, according to _segRate and_len the increment /
    *           decrement of the segment, determines whether the segment
    *           has a linear, exponential or logarithmic type and sets
    *           the internal member _type accordingly.
    *
    *  @see     _calcLevel()
    *
    ****************************************************************************/
    
    void _calcRate();
    
    /*************************************************************************//*!
    *
    *  @brief     Updates and recalculates anything in dependance of the length.
    *
    *  @details   It basically checks whether the difference between the start
    *             and end amplitude is negative, meaning there is an increase,
    *             making the segment and the _ads member an attack segment, or
    *             positive, meaning there is a decay, or 0, which indicates a
    *             sustain segment.
    *
    ****************************************************************************/
    
    void _calcLevel();
    
    /*************************************************************************//*!
    *
    *  @brief       Calculates the frequency / rate of the modulation lfo.
    *
    *  @details     Calculates the frequency that the modulation oscillator must have for
    *               a _modRate of 1 to be ONE cycle throughout the segment and multiplies
    *               that by _modRate. The _modRate is not an absolute value of cycles per
    *               second, rather it is the number of cycles per SEGMENT. Must be called
    *               either when the _modRate changes or when _len changes.
    *
    *  @see         setModRate()
    *
    ****************************************************************************/
    
    void _calcModRate();
    
    /*! Attack, Decay or Sustain identifier */
    ADS _ads;
    
    /*! Linear, Logarithmic or Exponential identifier */
    Type _type;
    
    /*! Sample count */
    len_t _sample;
    
    /*! Modulation wave */
    int _modWave;
    
    /*! The rate determining the type (lin,log,exp) */
    double _segRate;
    
    /*! Starting amplitude */
    double _startLevel;
    
    /*! End amplitude */
    double _endLevel;
    
    /*! Depth of the modulation, between 0 and 1 */
    double _modDepth;
    
    /*! Rate of modulation (modulation cycles per __segment__) */
    unsigned short _modRate;
    
    /*! Current increment value */
    double _segIncr;
    
    /*! Current envelope value */
    double _segCurr;
    
    /*! Difference between end and start amplitude */
    double _diff;
    
    /*! Offset for exponential increment */
    double _offset;
    
    /*! The modulation oscillator */
    LFO * _lfo;
    
    /*! Maximum possible segment length, a good value is 60 seconds */
    static const len_t _maxLen;
    
    /*! Length of segment in samples */
    len_t _len;
};

/******************************************************************************//*!
*
*  @brief       A sequence of EnvSegs.
*
*  @details     An EnvSegSeq is a sequence of envelope segments that are activated
*               one after the other. Control over the individual segments is possible
*               as well as looping.
*
*********************************************************************************/

class EnvSegSeq
{
    
public:

    typedef unsigned long subseg_t;
    
    /******************************************************************************//*!
    *
    *  @brief       Constructs an EnvSegSeq.
    *
    *  @param       seqLen The number of segments in the sequence.
    *
    *********************************************************************************/
    
    EnvSegSeq(subseg_t seqLen);
    
    virtual ~EnvSegSeq() { }

    /******************************************************************************//*!
    *
    *  @brief      Ticks an envelope value.
    *
    *  @param      The envelope value is the tick of the current segment.
    *
    *********************************************************************************/
    
    virtual double tick();
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the EnvSegSeq's amplitude.
    *
    *  @param      amp The new amplitude value.
    *
    *********************************************************************************/
    
    virtual void setAmp(double amp);
    
    /******************************************************************************//*!
    *
    *  @brief      Gets the EnvSegSeq's amplitude.
    *
    *  @return     The current amplitude value.
    *
    *********************************************************************************/
    
    virtual double getAmp() const;
    
    /******************************************************************************//*!
    *
    *  @brief      Adds a segment to the sequence.
    *
    *********************************************************************************/
    
    virtual void addSegment();
    
    /******************************************************************************//*!
    *
    *  @brief      Removes the last segment in the sequence.
    *
    *********************************************************************************/
    
    virtual void removeSegment();
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      seg The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *********************************************************************************/
    
    virtual void setSegRate(subseg_t seg, double rate);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current rate of a segment.
    *
    *  @param     seg The segment, of which to get the rate.
    *
    *  @return     The rate of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegRate(subseg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the starting level of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegStartLevel(subseg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current starting level of a segment.
    *
    *  @param     seg The segment, of which to get the starting level.
    *
    *  @return     The starting level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegStartLevel(subseg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the end level of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegEndLevel(subseg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current ending level of a segment.
    *
    *  @param     seg The segment, of which to get the ending level.
    *
    *  @return     The ending level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegEndLevel(subseg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the both starting and ending levels at the same time.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegBothLevels(subseg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the modulation wave of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       mod The wavetable id of the wavetable (WavetableDB::Wavetables).
    *
    *********************************************************************************/
    
    virtual void setSegModWave(subseg_t seg, int wavetableId);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the modulation depth of a segment's mod wave.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       dpth The new depth value, between 0 and 1.
    *
    *********************************************************************************/
    
    virtual void setSegModDepth(subseg_t seg, double dpth);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current mod depth of a segment.
    *
    *  @param     seg The segment, of which to get the mod depth level.
    *
    *  @return     The mod depth level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegModDepth(subseg_t seg) const;
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets the modulation rate of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       rate The new rate, not that this is cycles per segment and not per second.
    *
    ********************************************************************************************/
    
    virtual void setSegModRate(subseg_t seg, unsigned short rate);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current mod rate of a segment.
    *
    *  @param     seg The segment, of which to get the mod rate.
    *
    *  @return     The mod rate of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegModRate(subseg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the length of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       ms The new length of the segment, in milliseconds.
    *
    *********************************************************************************/

    virtual void setSegLen(subseg_t seg, unsigned long ms);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current length of a segment.
    *
    *  @param     seg The segment, of which to get the length.
    *
    *  @return     The length of a segment.
    *
    *********************************************************************************/
    
    virtual unsigned long getSegLen(subseg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop start segment.
    *
    *  @param       seg The segment to start the loop from.
    *
    *********************************************************************************/
    
    virtual void setLoopStart(subseg_t seg);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop start segment.
    *
    *  @return     The index of the loop start segment.
    *
    *********************************************************************************/
    
    virtual subseg_t getLoopStart() const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop end segment.
    *
    *  @param       seg The segment to end the loop at.
    *
    *********************************************************************************/
    
    virtual void setLoopEnd(subseg_t seg);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop end segment.
    *
    *  @return     The index of the loop end segment.
    *
    *********************************************************************************/
    
    virtual subseg_t getLoopEnd() const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the maximum number of times to loop.
    *
    *  @details     Note that a loop count greater 64 sets the loop-to-infinity property to true.
    *
    *  @param       n The number of times to loop.
    *
    *********************************************************************************/
    
    virtual void setLoopMax(unsigned short n);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the maximum number of loops to loop through.
    *
    *  @return     The maximum number of loops to loop through.
    *
    *********************************************************************************/
    
    virtual unsigned short getLoopMax() const;
    
    /******************************************************************************//*!
    *
    *  @brief       Enables/disables the loop-to-infinity property.
    *
    *  @param       state Boolean whether or not to loop to infinity.
    *
    *********************************************************************************/
    
    virtual void setLoopInf(bool state = true);
    
    /******************************************************************************//*!
    *
    *  @brief      Whether or not the sequence is looping to infinity.
    *
    *  @return     Boolean, whether or not the sequence is looping to infinity.
    *
    *********************************************************************************/
    
    virtual bool loopInf() const;
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the sequence size/length.
    *
    *  @return     The sequence size/length.
    *
    *********************************************************************************/

    virtual std::vector<EnvSeg>::size_type size() const;
    
protected:
    
    typedef std::vector<EnvSeg>::iterator segItr;
    
    /*! Changes the current segment in the sequence */
    virtual void _changeSeg(segItr seg);
    
    /*! Executes various steps to reset a loop (go from end back to start) */
    virtual void _resetLoop();
    
    /*! The number of samples passed since starting the current segment */
    unsigned long _currSample;
    
    /*! Iterator pointing to the current segment */
    segItr _currSeg;
    
    /*! Iterator pointing to the loop start segment */
    segItr _loopStart;
    
    /*! Iterator pointing to the loop end segment */
    segItr _loopEnd;
    
    /*! Current number of loops executed */
    subseg_t _loopCount;
    
    /*! Maximum number of loops */
    subseg_t _loopMax;
 
    /*! Boolean whether or not to loop infinitely */
    bool _loopInf;
    
    /*! The last ticked value */
    double _lastTick;
    
    /*! The amplitude value */
    double _amp;
    
    /*! The segment sequence */
    std::vector<EnvSeg> _segs;
};


#endif /* defined(__Anthem__EnvSeg__) */