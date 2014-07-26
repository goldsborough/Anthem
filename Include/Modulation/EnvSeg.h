//
//  EnvSeg.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__EnvSeg__
#define __Synth__EnvSeg__

#include "GenUnits.h"
#include <vector>

class LFO;

/******************************************************************************//*!
 *
 *  @brief      An envelope segment
 *
 *  @details    The EnvSeg class is a single segment of an envelope, it can be
 *              be either an attack, decay or a sustain segment and increment or
 *              decrement either exponentially, logarithmically or linearly in
 *              case of being a decay or an attack segment. Additionally, it has
 *              an internal modulation oscillator / LFO that can modulate the
 *              envelope values according to the waveform.
 *
 *********************************************************************************/

class EnvSeg : public ModUnit
{
    
public:
    
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
    
    EnvSeg(double startAmp = 0, double endAmp = 0, unsigned long len = 0, double segR = 1,
           int modW = -1, double modDpth = 1,
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
    void setLen(unsigned long sampleLen);
    
   /*************************************************************************//*!
    *
    *  @brief      Returns the length of the segment
    *
    *  @return     The length of the segment
    *
    ****************************************************************************/
    
    unsigned long getLen() { return _len; };
    
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
    *  @brief       Returns the the end level of the segment.
    *
    *  @return      The end level of the segment.
    *
    ****************************************************************************/
    
    double getEndLevel() { return _endLevel;};
    
    /*************************************************************************//*!
    *
    *  @brief       Returns the the starting level of the segment.
    *
    *  @return      The starting level of the segment.
    *
    ****************************************************************************/
    
    double getStartLevel() {return _startLevel; };
    
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
    
    void setModRate(unsigned char rate);
    
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
    *  @brief       Resets the envelope segment.
    *
    *  @details     Resets the sample count to 0 and calls _calcRate() to 
    *               reset the increment.
    *
    *  @see         _calcRate()
    *
    ****************************************************************************/
    
    void reset()
    {
        _sample = 0;
        
        // reset _segCurr
        _calcRate();
    }
    
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
    unsigned long _sample;
    
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
    unsigned char _modRate;
    
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
    static const unsigned long _maxLen;
    
    /*! Length of segment in samples */
    unsigned long _len;
};

class EnvSegSeq : public ModUnit
{
    
public:
    
    typedef std::vector<EnvSeg> segVector;
    typedef unsigned int subseg_t;
    
    EnvSegSeq(subseg_t seqLen);

    virtual double tick();
    
    virtual void setSegRate(subseg_t seg, double rate) { _segs[seg].setRate(rate); }
    
    virtual void setSegStartLevel(subseg_t seg, double lv)  { _segs[seg].setStartLevel(lv); }
    
    virtual void setSegEndLevel(subseg_t seg, double lv)  { _segs[seg].setEndLevel(lv); }
    
    virtual void setSegBothLevels(subseg_t seg, double lv) { setSegStartLevel(seg, lv), setSegEndLevel(seg, lv); }
    
    virtual void setSegModWave(subseg_t seg, int mod) { _segs[seg].setModWave(mod); }
    
    virtual void setSegModDepth(subseg_t seg, double dpth) { _segs[seg].setModDepth(dpth); }
    
    virtual void setSegModRate(subseg_t seg, unsigned char rate) { _segs[seg].setModRate(rate); }
    
    virtual void setSegLen(subseg_t seg, unsigned int ms);
    
    virtual void setLoopStart(subseg_t seg);
    
    virtual void setLoopEnd(subseg_t seg);
    
    virtual void setLoopMax(unsigned char n);
    
    virtual void setLoopInf(bool state = true) { _loopInf = state; }
    
    virtual void setOneShot(bool state = true) { _oneShot = state; }
    
    virtual subseg_t getSeqLen() { return _seqLen; }
    
    virtual ~EnvSegSeq() {}
    
protected:
    
    virtual void _changeSeg(subseg_t seg);
    
    virtual void _resetLoop();
    
    unsigned long _currSample;
    
    subseg_t _currSegNum;
    
    subseg_t _loopStart;
    subseg_t _loopEnd;
    
    subseg_t _loopCount;
    subseg_t _loopMax;
    
    bool _loopInf;
    
    bool _oneShot;
    
    double _lastTick;
    
    // The pointer to the
    // current segment
    EnvSeg * _currSeg;
    
    // The current final
    // sample count
    unsigned long _currSegLen;
    
    segVector _segs;
    
    const subseg_t _seqLen;
};


#endif /* defined(__Synth__EnvSeg__) */
