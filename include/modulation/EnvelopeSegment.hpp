/*********************************************************************************************//*!
*
*  @file        EnvelopeSegment.hpp
*
*  @author      Peter Goldsborough
*
*  @date        18/10/2014
*
*  @brief       EnvelopeSegment, EnvelopeSegmentSequence and ModEnvelopeSegmentSequence classes.
*
*************************************************************************************************/

#ifndef __Anthem__EnvelopeSegment__
#define __Anthem__EnvelopeSegment__

#include "Units.hpp"
#include <vector>

/******************************************************************************//*!
 *
 *  @brief      An envelope segment
 *
 *  @details    The EnvelopeSegment class is a single segment of a EnvelopeSegmentSequence. It can be
 *              be either an attack, decay or a sustain segment and increment or
 *              decrement either exponentially, logarithmically or linearly in
 *              case of being a decay, an attack or a release segment.
 *
 *********************************************************************************/

class EnvelopeSegment : public GenUnit
{
    
public:
    
    friend class ModEnvelopeSegmentSequence;
    
    /*! Parameters for modulation */
    enum Docks
    {
        RATE,
        START_LEVEL,
        END_LEVEL
    };
    
    typedef unsigned long length_t;
    
    /**********************************************************************************************//*!
     *
     *  @brief      Initalizes an Envelope segment
     *
     *  @param      startLevel The starting level of the segment.
     *
     *  @param      endLevel The ending level of the segment.
     *
     *  @param      len The length of the segment in samples.
     *
     *  @param      rate The rate of the segment. 1 = Lin, 0 <= rate < 1 = Exp, 1 < rate <= 2 = Log
     *
     **************************************************************************************************/
    
    EnvelopeSegment(double startLevel = 0,
           double endLevel = 0,
           length_t len = 0,
           double rate = 1);
    
    /******************************************************************//*!
     *
     *  @brief      Ticks the current envelope value
     *
     *  @details    Modulates the value if _modWave != None returns _endLevel
     *              amplitude if _sample (count) larger than _len
     *
     *  @return     double precision floating point envelope value
     *
     *  @see        update()
     *
     *********************************************************************/
    
    double tick();
    
    /*************************************************************************//*!
    *
    *  @brief      Increments the envelope. Call after ticking.
    *
    ****************************************************************************/
    
    void update();
    
    /**********************************************************************//*!
     *
     *  @brief      Sets the length of the segment
     *
     *  @param      sampleLength Length of the segment in __samples__.
     *
     *  @exception  Throws std::invalid_argument if len > _maxLength (60 seconds)
     *
     *************************************************************************/

    void setLength(length_t sampleLength);
    
    /*************************************************************************//*!
    *
    *  @brief      Returns the length of the segment.
    *
    *  @return     The length of the segment, in samples.
    *
    ****************************************************************************/
    
    length_t getLength() const;
    
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
    *  @param       lv The new level.
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
    
    /*! The rate determining the type (lin,log,exp) */
    double rate_;
    
    /*! Starting amplitude */
    double startLevel_;
    
    /*! End amplitude */
    double endLevel_;
    
    /*! Difference between end and start amplitude */
    double range_;

    /*! Current segment value */
    double curr_;
    
    /*! Increment value for curr_ */
    double incr_;
    
    /*! Length of segment in samples */
    length_t len_;
};

/******************************************************************************//*!
*
*  @brief       A sequence of EnvelopeSegments.
*
*  @details     An EnvelopeSegmentSequence is a sequence of envelope segments that are activated
*               one after the other. Control over the individual segments is possible
*               as well as looping.
*
*********************************************************************************/

class EnvelopeSegmentSequence
{
    
public:

    typedef unsigned long segment_t;
    
    /******************************************************************************//*!
    *
    *  @brief       Constructs an EnvelopeSegmentSequence.
    *
    *  @param       seqLength The number of segments in the sequence.
    *
    *********************************************************************************/
    
    EnvelopeSegmentSequence(segment_t seqLength);
    
    EnvelopeSegmentSequence(const EnvelopeSegmentSequence& other);
    
    EnvelopeSegmentSequence& operator= (const EnvelopeSegmentSequence& other);
    
    virtual ~EnvelopeSegmentSequence() { }

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
    *  @brief      Increments the sequence value.
    *
    *********************************************************************************/
    
    virtual void update();
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      segment The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *********************************************************************************/
    
    virtual void setSegmentRate(segment_t segment, double rate);
    
    /******************************************************************************//*!
    *
    *  @brief       Returns the current rate of a segment.
    *
    *  @param       segment The segment, of which to get the rate.
    *
    *  @return      The rate of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegmentRate(segment_t segment) const;
    
    /*****************************************************************************************************//*!
    *
    *  @brief       Sets the ending level of a segment and the starting level of the next.
    *
    *  @details     This method 'links' the levels of two adjacent segments by making any
    *               segment's ending level equal to the starting level of the next. This
    *               applies to all segments except the last one, since it has no next segment.
    *
    *  @param       segment The segment in the sequence for which to set the ending amp (and starting of segment+1).
    *
    *  @param       lv The new level.
    *
    ********************************************************************************************************/
    
    virtual void setLinkedLevel(segment_t segment, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the starting level of a segment.
    *
    *  @param       segment The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegmentStartLevel(segment_t segment, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current starting level of a segment.
    *
    *  @param     segment The segment, of which to get the starting level.
    *
    *  @return     The starting level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegmentStartLevel(segment_t segment) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the end level of a segment.
    *
    *  @param       segment The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegmentEndLevel(segment_t segment, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current ending level of a segment.
    *
    *  @param      segment The segment, of which to get the ending level.
    *
    *  @return     The ending level of a segment.
    *
    *********************************************************************************/
    
    virtual double getSegmentEndLevel(segment_t segment) const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the both starting and ending levels at the same time.
    *
    *  @param       segment The segment in the sequence.
    *
    *  @param       lv The new level.
    *
    *********************************************************************************/
    
    virtual void setSegmentBothLevels(segment_t segment, double lv);
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop start segment.
    *
    *  @param       segment The segment to start the loop from.
    *
    *********************************************************************************/
    
    virtual void setLoopStart(segment_t segment);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop start segment.
    *
    *  @return     The index of the loop start segment.
    *
    *********************************************************************************/
    
    virtual segment_t getLoopStart() const;
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the sequence's loop end segment.
    *
    *  @param       segment The segment to end the loop at.
    *
    *********************************************************************************/
    
    virtual void setLoopEnd(segment_t segment);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the index of the loop end segment.
    *
    *  @return     The index of the loop end segment.
    *
    *********************************************************************************/
    
    virtual segment_t getLoopEnd() const;
    
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

    virtual std::vector<EnvelopeSegment>::size_type size() const;
    
    /******************************************************************************//*!
    *
    *  @brief      Resets the EnvelopeSegmentSequence to its first sample.
    *
    *********************************************************************************/
    
    virtual void reset();
    
protected:
    
    typedef std::vector<EnvelopeSegment>::iterator segmentItr;
    
    /*! Changes the current segment in the sequence */
    virtual void changeSegment_(segmentItr segment);
    
    /*! Executes various steps to reset a loop (go from end back to start) */
    virtual void resetLoop_();
    
    /*! The number of samples passed since starting the current segment */
    unsigned long currSample_;
    
    /*! Number of the current segment in the sequence */
    segment_t currSegmentNum_;
    
    /*! Iterator pointing to the current segment */
    segmentItr currSegment_;
    
    /*! Iterator pointing to the loop start segment */
    segmentItr loopStart_;
    
    /*! Iterator pointing to the loop end segment */
    segmentItr loopEnd_;
    
    /*! Current number of loops executed */
    segment_t loopCount_;
    
    /*! Maximum number of loops */
    segment_t loopMax_;
 
    /*! Boolean whether or not to loop infinitely */
    bool loopInf_;
    
    /*! The segment sequence */
    std::vector<EnvelopeSegment> segments_;
};


/******************************************************************************//*!
*
*  @brief      An EnvelopeSegmentSequence with ModDocks.
*
*  @details    ModEnvelopeSegmentSequence is an abstract class that implements methods for
*              envelope classes that have to handle the modulation of their
*              own ModDocks (e.g. for rate or master depth) as well as those
*              of their segments (rate, amplitude).
*
*********************************************************************************/

class ModEnvelopeSegmentSequence : public EnvelopeSegmentSequence, public ModUnit
{
    
public:
    
    /******************************************************************************//*!
    *
    *  @param       numSegments The number of segments in the sequence.
    *
    *  @param       numDocks The number of ModDocks for the sequence (not segments).
    *
    *  @param       amp An initial master amplitude value for the sequence.
    *
    *********************************************************************************/
    
    ModEnvelopeSegmentSequence(segment_t numSegments,
                 segment_t numDocks = 0,
                 double masterAmp = 1);
    
    virtual ~ModEnvelopeSegmentSequence() { }
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the number of ModDocks of a segment.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @return      The number of ModDocks.
    *
    *****************************************************************************************************/
    
    virtual std::vector<ModDock*>::size_type numDocks_Segment(segment_t segmentNum) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the depth for a single modulation unit of a segment's dock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @param       depth The new depth value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    virtual void setModUnitDepth_Segment(segment_t segmentNum, index_t dockNum, index_t modNum, double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the depth for a single modulation unit of a segment's  dock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @return      The depth of that modulation unit.
    *
    *****************************************************************************************************/
    
    virtual double getModUnitDepth_Segment(segment_t segmentNum, index_t dockNum, index_t modNum) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Attaches a new modulation unit to a segment's dock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the dock to attach the new modulation unit to.
    *
    *  @param       modNum The number of the modulation space into which to place the mod unit.
    *
    *  @param       mod A pointer to a GenUnit or one of its derived class.
    *
    *  @see         detachMod()
    *
    *****************************************************************************************************/
    
    virtual void attachMod_Segment(segment_t segmentNum, index_t dockNum, ModUnit* mod);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Detaches a modulation unit from a segment's dock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the dock to detach from.
    *
    *  @param       modNum The number of the modulation space to detach.
    *
    *  @see         attachMod()
    *
    *****************************************************************************************************/
    
    virtual void detachMod_Segment(segment_t segmentNum, index_t dockNum, index_t modNum);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns boolean whether or not a dock of a segment is in use.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @return      True if dock has at least one ModUnit, else false.
    *
    *****************************************************************************************************/
    
    virtual bool dockInUse_Segment(segment_t segmentNum, index_t dockNum) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Makes a ModUnit in a segment's ModDock sidechain another's depth.
    *
    *  @details     Sidechaining means that one ModUnit sidechains/modulates another's depth
    *               value. Multiple mastery as well as multiple slavery is possible, so you
    *               can call setSidechain for any two indices you wish as long as the connection
    *               isn't already established, in which case nothing happens. Note that a ModUnit
    *               cannot simultaneously be a master of slaves and contribute to the ModDock's
    *               modulation value, it is either-or.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the ModUnit in the ModDock to be made master.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *************************************************************************************************/
    
    virtual void setSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Cuts the sidechain connection between two ModUnits in a segment's ModDock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the master in the ModDock.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *  @throws      std::logic_error if the two ModUnits aren't connected.
    *
    *************************************************************************************************/
    
    virtual void unSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave);
    
    /*******************************************************************************************************//*!
    *
    *  @brief       Returns true if there is a sidechain connection between master and slave in a segment's ModDock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the master in the ModDock.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *  @return      Boolean, whether or not master is sidechaining slave.
    *
    ***********************************************************************************************************/
    
    virtual bool isSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a segment's ModDock is a master.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a master of one or more slaves, else false.
    *
    *************************************************************************************************/
    
    virtual bool isMaster_Segment(segment_t segmentNum, index_t dockNum, index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a segment's Moddock is a slave.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a slave of one or more masters, else false.
    *
    *************************************************************************************************/
    
    virtual bool isSlave_Segment(segment_t segmentNum, index_t dockNum, index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Returns the size of a segment's ModDock.
    *
    *  @param       segmentNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock
    *
    *  @return      The number of ModUnits in the ModDock.
    *
    *************************************************************************************************/
    
    virtual unsigned long dockSize_Segment(segment_t segmentNum, index_t dockNum) const;
    
protected:
    
    /*! Sets the ModDock base value for a segment. */
    void setSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum, double value);
    
    /*! Returns the ModDock base value for a segment. */
    double getSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum) const;
};

/******************************************************************************//*!
 *
 *  @brief      An Extension to the ModEnvelopeSegmentSequence for flexible segment lengths.
 *
 *  @details    Neither the EnvelopeSegmentSequence nor the ModEnvelopeSegmentSequence classes allow for
 *              the adjustment of segments' length because some classes that
 *              inherit from them, like LFOSequence, don't let the user adjust the
 *              lengths of individual segments. This class extends ModEnvelopeSegmentSequence
 *              and implements two methods for getting and setting an individual
 *              segment's length.
 *
 *********************************************************************************/

class ModEnvelopeSegmentSequenceFlexible : public ModEnvelopeSegmentSequence
{
    
public:
    
    /*! @copydoc ModEnvelopeSegmentSequence::ModEnvelopeSegmentSequence() */
    ModEnvelopeSegmentSequenceFlexible(segment_t numSegments,
                         segment_t numDocks = 0,
                         double masterAmp = 1);
    
    virtual ~ModEnvelopeSegmentSequenceFlexible() { }
    
    /******************************************************************************//*!
    *
    *  @brief       Sets the length of a segment.
    *
    *  @param       segment The segment in the sequence.
    *
    *  @param       ms The new length of the segment, in milliseconds.
    *
    *********************************************************************************/
    
    virtual void setSegmentLength(segment_t segment, unsigned long ms);
    
    /******************************************************************************//*!
    *
    *  @brief      Returns the current length of a segment.
    *
    *  @param     segment The segment, of which to get the length.
    *
    *  @return     The length of a segment.
    *
    *********************************************************************************/
    
    virtual unsigned long getSegmentLength(segment_t segment) const;
};

#endif /* defined(__Anthem__EnvelopeSegment__) */