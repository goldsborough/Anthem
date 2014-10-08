/*********************************************************************************************//*!
*
*  @file        EnvSeg.h
*
*  @author      Peter Goldsborough
*
*  @date        18/10/2014
*
*  @brief       EnvSeg, EnvSegSeq and ModEnvSegSeq classes.
*
*************************************************************************************************/

#ifndef __Anthem__EnvSeg__
#define __Anthem__EnvSeg__

#include "Units.h"
#include <vector>

/******************************************************************************//*!
 *
 *  @brief      An envelope segment
 *
 *  @details    The EnvSeg class is a single segment of a EnvSegSeq. It can be
 *              be either an attack, decay or a sustain segment and increment or
 *              decrement either exponentially, logarithmically or linearly in
 *              case of being a decay or an attack segment.
 *
 *********************************************************************************/

class EnvSeg : public GenUnit
{
    
public:
    
    enum Docks
    {
        RATE,
        START_LEVEL,
        END_LEVEL
    };
    
    typedef unsigned long len_t;
    
    /**********************************************************************************************//*!
     *
     *  @brief      Initalizes an Envelope segment
     *
     *  @param      startLevel The starting level of the segment.
     *
     *  @param      endAmp The ending level of the segment.
     *
     *  @param      len The length of the segment in samples.
     *
     *  @param      rate The rate of the segment. 1 = Lin, 0 <= rate < 1 = Exp, 1 < rate <= 2 = Log
     *
     **************************************************************************************************/
    
    EnvSeg(double startAmp = 0,
           double endAmp = 0,
           len_t len = 0,
           double rate = 1);
    
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
    
    /*! Calculates the amplitude range and assigns it to range_ */
    void calcRange_();
    
    /*! Calculates the increment for curr_ and assigns it to incr_ */
    void calcIncr_();
    
    /*! Ticks a value (calculates it from data members) */
    double calc_();
    
    /*! The rate determining the type (lin,log,exp) */
    double rate_;
    
    /*! Starting amplitude */
    double startAmp_;
    
    /*! End amplitude */
    double endAmp_;
    
    /*! Difference between end and start amplitude */
    double range_;

    /*! Current segment value */
    double curr_;
    
    /*! Increment value for curr_ */
    double incr_;
    
    /*! Length of segment in samples */
    len_t len_;
    
    /*! Maximum possible segment length, a good value is 60 seconds */
    static const len_t maxLen_;
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

    typedef unsigned long seg_t;
    
    /******************************************************************************//*!
    *
    *  @brief       Constructs an EnvSegSeq.
    *
    *  @param       seqLen The number of segments in the sequence.
    *
    *********************************************************************************/
    
    EnvSegSeq(seg_t seqLen);
    
    virtual ~EnvSegSeq() { }

    /******************************************************************************//*!
    *
    *  @brief      Ticks an envelope value.
    *
    *  @details      The envelope value is the tick of the current segment.
    *
    *********************************************************************************/
    
    virtual double tick();
    
    /******************************************************************************//*!
    *
    *  @brief      Adds a segment to the sequence.
    *
    *********************************************************************************/
    
    virtual void addSegment();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Removes the last segment from the sequence.
    *
    *  @throws      std::runtime_error if segment number is 0.
    *
    *****************************************************************************************************/
    
    virtual void removeSegment();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Removes the last segment from the sequence.
    *
    *  @throws      std::invalid_argument if segment number invalid.
    *
    *****************************************************************************************************/
    
    virtual void removeSegment(seg_t seg);
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      seg The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *********************************************************************************/
    
    virtual void setSegRate(seg_t seg, double rate);
    
    /******************************************************************************//*!
    *
    *  @brief       Returns the current rate of a segment.
    *
    *  @param       seg The segment, of which to get the rate.
    *
    *  @return      The rate of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegRate(seg_t seg) const;
    
    /*****************************************************************************************************//*!
    *
    *  @brief       Sets the ending level of a segment and the starting level of the next.
    *
    *  @details     This method 'links' the levels of two adjacent segments by making any
    *               segment's ending level equal to the starting level of the next. This
    *               applies to all segments except the last one, since it has no next seg.
    *
    *  @param       seg The segment in the sequence for which to set the ending amp (and starting of seg+1).
    *
    *  @param       lv The new level.
    *
    ********************************************************************************************************/
    
    virtual void setLinkedLevel(seg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the starting level of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegStartLevel(seg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current starting level of a segment.
    *
    *  @param     seg The segment, of which to get the starting level.
    *
    *  @return     The starting level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegStartLevel(seg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the end level of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegEndLevel(seg_t seg, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current ending level of a segment.
    *
    *  @param      seg The segment, of which to get the ending level.
    *
    *  @return     The ending level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegEndLevel(seg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the both starting and ending levels at the same time.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegBothLevels(seg_t seg, double lv);
        
    /******************************************************************************//*!
    *
    *  @brief       Sets the length of a segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       ms The new length of the segment, in milliseconds.
    *
    *********************************************************************************/

    virtual void setSegLen(seg_t seg, unsigned long ms);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current length of a segment.
    *
    *  @param     seg The segment, of which to get the length.
    *
    *  @return     The length of a segment.
    *
    *********************************************************************************/
    
    virtual unsigned long getSegLen(seg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop start segment.
    *
    *  @param       seg The segment to start the loop from.
    *
    *********************************************************************************/
    
    virtual void setLoopStart(seg_t seg);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop start segment.
    *
    *  @return     The index of the loop start segment.
    *
    *********************************************************************************/
    
    virtual seg_t getLoopStart() const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop end segment.
    *
    *  @param       seg The segment to end the loop at.
    *
    *********************************************************************************/
    
    virtual void setLoopEnd(seg_t seg);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop end segment.
    *
    *  @return     The index of the loop end segment.
    *
    *********************************************************************************/
    
    virtual seg_t getLoopEnd() const;
    
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
    virtual void changeSeg_(segItr seg);
    
    /*! Executes various steps to reset a loop (go from end back to start) */
    virtual void resetLoop_();
    
    /*! The number of samples passed since starting the current segment */
    unsigned long currSample_;
    
    /*! Iterator pointing to the current segment */
    segItr currSeg_;
    
    /*! Iterator pointing to the loop start segment */
    segItr loopStart_;
    
    /*! Iterator pointing to the loop end segment */
    segItr loopEnd_;
    
    /*! Current number of loops executed */
    seg_t loopCount_;
    
    /*! Maximum number of loops */
    seg_t loopMax_;
 
    /*! Boolean whether or not to loop infinitely */
    bool loopInf_;
    
    /*! The last ticked value */
    double lastTick_;
    
    /*! The segment sequence */
    std::vector<EnvSeg> segs_;
};


/******************************************************************************//*!
*
*  @brief      An EnvSegSeq with ModDocks.
*
*  @details
*
*
*
*
*********************************************************************************/

class ModEnvSegSeq : public EnvSegSeq, public ModUnit
{
    
public:
    
    /*! Available ModDocks. */
    enum Docks
    {
        SEG_RATE,
        SEG_LEVEL
    };
    
    ModEnvSegSeq(seg_t numSegs,
                 seg_t dockNum = 0,
                 double amp = 1);
    
    virtual ~ModEnvSegSeq() { }
    
    /*! @copydoc Unit::setModUnitDepth() */
    virtual void setSegModUnitDepth(seg_t segNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth);
    
    /*! @copydoc Unit::attachMod() */
    virtual void attachSegMod(seg_t segNum,
                              index_t dockNum,
                              ModUnit* mod);
    
    /*! @copydoc Unit::detachMod() */
    virtual void detachSegMod(seg_t segNum,
                              index_t dockNum,
                              index_t modNum);
};

#endif /* defined(__Anthem__EnvSeg__) */