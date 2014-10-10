/*********************************************************************************************//*!
*
*  @file        Envelope.h
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

#include "EnvSeg.h"

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

class Envelope : public ModEnvSegSeq
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
    *************************************************************************************************/
    
    double modulate(double sample,
                    double depth,
                    double);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Sets The level of a segment.
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
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the depth for a single modulation unit of a segment's dock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @param       depth The new depth value, between 0 and 1.
    *
    *****************************************************************************************************/
    
    void setModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum, double depth);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the depth for a single modulation unit of a segment's  dock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the modulation unit (e.g. Operator::AMP).
    *
    *  @param       modNum  The modulation unit number within the dock.
    *
    *  @return      The depth of that modulation unit.
    *
    *****************************************************************************************************/
    
    double getModUnitDepth_Seg(seg_t segNum, index_t dockNum, index_t modNum) const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Attaches a new modulation unit to a segment's dock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
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
    
    void attachMod_Seg(seg_t segNum, index_t dockNum, ModUnit* mod);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Detaches a modulation unit from a segment's dock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The dock number of the dock to detach from.
    *
    *  @param       modNum The number of the modulation space to detach.
    *
    *  @see         attachMod()
    *
    *****************************************************************************************************/
    
    void detachMod_Seg(seg_t segNum, index_t dockNum, index_t modNum);
    
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
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       master The index of the ModUnit in the ModDock to be made master.
    *
    *  @param       slave The index of the slave in the ModDock.
    *
    *************************************************************************************************/
    
    void setSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Cuts the sidechain connection between two ModUnits in a segment's ModDock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
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
    
    void unSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave);
    
    /*******************************************************************************************************//*!
    *
    *  @brief       Returns true if there is a sidechain connection between master and slave in a segment's ModDock.
    *
    *  @param       segNum The segment index in the sequence to do this for.
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
    
    bool isSidechain_Seg(seg_t segNum, index_t dockNum, index_t master, index_t slave) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a segment's ModDock is a master.
    *
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a master of one or more slaves, else false.
    *
    *************************************************************************************************/
    
    bool isMaster_Seg(seg_t segNum, index_t dockNum, index_t index) const;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Whether or not a certain ModUnit in a segment's Moddock is a slave.
    *
    *  @param       segNum The segment index in the sequence to do this for.
    *
    *  @param       dockNum The index of the ModDock in which to connect the two ModUnits.
    *
    *  @param       index The index of the ModUnit in the ModDock.
    *
    *  @return      Boolean, true if the ModUnit is a slave of one or more masters, else false.
    *
    *************************************************************************************************/
    
    bool isSlave_Seg(seg_t segNum, index_t dockNum, index_t index) const;
    
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
    
    /*! Whether or not to return the last tick forever after SEG_C finished */
    bool sustainEnabled_;
    
    /*! Current segment number in the sequence */
    seg_t currSegNum_;
};

#endif /* defined(__Anthem__Envelope__) */
