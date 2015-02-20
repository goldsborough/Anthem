/*********************************************************************************************//*!
*
*  @file        Envelope.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Holds the Envelope class.
*
*************************************************************************************************/

#ifndef __Anthem__Envelope__
#define __Anthem__Envelope__

#include "EnvSeg.hpp"

/*********************************************************************************************//*!
*
*  @brief       Anthem's main Envelope class.
*
*  @details     This Envelope is not quite like the usual ADSR envelope as it has five segments
*               instead of four. The first is ATK, the attack segment, then three freely
*               adjustable segments, SEG_A, SEG_B and SEG_C, and finally the release segment, REL.
*               Additionally, there is a delay segment which delays the signal by some milliseconds,
*               prior to ATK, however the only thing adjustable for this segment is the length.
*               Usually the Envelope will go from ATK to SEG_C and then stay in sustain until
*               a noteOff signal is sent, however the sustain feature can also be disabled so
*               that the envelope will go straight to REL after finishing SEG_C. Moreover it is
*               possible to set loop points between two segments and a loopMax value to control
*               how many times the Envelope should loop between the two loop points. Finally,
*               this class also has a hidden connector segment which is activated before switching
*               from the loop end segment to the loop start segment. This segment is samplerate/40
*               samples long and ensures that the change in amplitude between the two loop points
*               is not too abrupt. There are also various modulation docks available.
*
*************************************************************************************************/

class Envelope : public ModEnvSegSeqFlexible
{
public:

    typedef unsigned short seg_t;
    
    /*! Available ModDocks for segments. */
    enum Seg_Docks
    {
        SEG_RATE,
        SEG_LEVEL
    };
    
    /*! Available ModDocks. */
    enum Docks { AMP };
    
    /*! The Envelope's segments */
    enum Segment
    {
        DEL,
        ATK,
        SEG_A,
        SEG_B,
        SEG_C,
        REL
    };
    
    /*********************************************************************************************************************//*!
    *
    *  @brief       Constructs an Envelope object.
    *
    *  @details     The initial configurations for the Envelope is a 500ms long ATK segment
    *               from 0 to 0.8 amplitude and a 250ms long SEG_A from 0.8 down to 0.5.
    *
    *  @param       sustainEnabled Whether or not tick the last SEG_C value until noteOff.
    *
    *************************************************************************************************************************/
    
    Envelope(bool sustainEnabled = true);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Modulates a sample and returns the result.
    *
    *  @param       sample The sample to modulate.
    *
    *  @param       depth The depth value to modulate with.
    *
    *  @param       maximum The symmetric maximum boundary value (e.g. 1 for amplitude).
    *
    *************************************************************************************************/
    
    double modulate(double sample,
                    double depth,
                    double maximum);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets the level of a segment.
    *
    *  @details     Setting the level of a segment sets its end level and the start level of
    *               the next segment.
    *
    *  @param       seg The segment in the sequence.
    *
    *  @param       lv The new level, between 0 and 1.
    *
    ********************************************************************************************/
    
    void setSegLevel(seg_t seg, double lv);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Returns the level of a segment.
    *
    *  @details     Equivalent to EnvSegSeq::getSegEndLevel()
    *
    *  @param       seg The segment in the sequence.
    *
    *  @return      The segment's level, between 0 and 1.
    *
    ********************************************************************************************/
    
    double getSegLevel(seg_t seg) const;
    
    /******************************************************************************//*!
    *
    *  @brief      Sets the rate of a segment.
    *
    *  @param      seg The segment in the sequence.
    *
    *  @param      rate The new rate. 0-1 is exp., 1 is linear and 1-2 is log.
    *
    *********************************************************************************/
    
    void setSegRate(seg_t seg, double rate);
    
    /******************************************************************************//*!
    *
    *  @brief       Returns the current rate of a segment.
    *
    *  @param       seg The segment, of which to get the rate.
    *
    *  @return      The rate of a segment.
    *
    *********************************************************************************/
    
    double getSegRate(seg_t seg) const;
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets the loop start point.
    *
    *  @details     Note that you must set a loopMax value to enable looping.
    *
    *  @param       seg The segment in the sequence to start the loop from.
    *
    *  @see         setLoopEnd()
    *
    *  @see         EnvSegSeq::setLoopMax()
    *
    ********************************************************************************************/
    
    void setLoopStart(seg_t seg);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets the loop end point.
    *
    *  @details     Note that you must set a loopMax value to enable looping.
    *
    *  @param       seg The segment in the sequence to end the loop at.
    *
    *  @see         setLoopStart()
    *
    *  @see         EnvSegSeq::setLoopMax()
    *
    ********************************************************************************************/
    
    void setLoopEnd(seg_t seg);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sends the noteOff signal.
    *
    *  @details     When a note is released the envelope will immediately jump to the release
    *               segment (unless it already is in it).
    *
    ********************************************************************************************/
    
    void noteOff();
    
    /*****************************************************************************************//*!
    *
    *  @brief       Enabled or disables the sustain feature.
    *
    *  @details     Enabling the sustain means that after the end of SEG_C the envelope will
    *               continue ticking the last value from SEG_C, forever.
    *
    *  @param       sustainEnabled Boolean whether or not to enable sustain.
    *
    *  @see         sustainEnabled()
    *
    ********************************************************************************************/
    
    void setSustainEnabled(bool sustainEnabled);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Whether or not sustain is enabled.
    *
    *  @return      Boolean whether or not sustain is enabled.
    *
    *  @see         setSustainEnabled()
    *
    ********************************************************************************************/
    
    bool sustainEnabled() const;
    
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
    
    /*! @copydoc GenUnit::setAmp() */
    void setAmp(double amp);
    
    /*! @copydoc GenUnit::getAmp() */
    double getAmp() const;
    
private:
    
    /*! Connects the end and starting levels of the loop
        to avoid harsh transitions */
    enum { CONNECTOR = 6 };
    
    /*! Changes the current segment in the sequence */
    void changeSeg_(segItr itr);
    
    /*! Does the actual work for modulate() */
    double tick_();
    
    /*! Resets the loop segments and changes to hidden connector segment */
    void resetLoop_();
    
    /*! The last ticked value */
    double lastTick_;
    
    /*! Whether or not to return the last tick forever after SEG_C finished */
    bool sustainEnabled_;
};

#endif /* defined(__Anthem__Envelope__) */
