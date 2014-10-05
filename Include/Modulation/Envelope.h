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

class Envelope : public ModEnvSegSeq::ModEnvSegSeq
{
public:

    typedef unsigned short seg_t;
    
    /*! Available ModDocks. */
    enum Docks
    {
        AMP = 2
    };
    
    /*! The Envelope's segments */
    enum Segment
    {
        ATK = 2,
        SEG_A = 3,
        SEG_B = 4,
        SEG_C = 5,
        REL = 6
    };
    
    /*********************************************************************************************************************//*!
    *
    *  @brief       Constructs an Envelope object.
    *
    *  @details     The initial configurations for the Envelope is a 500ms long ATK segment
    *               from 0 to 0.8 amplitude and a 250ms long SEG_A from 0.8 down to 0.5.
    *
    *  @param       delayMillis An initial length value for the delay segment, in Milliseconds.
    *
    *  @param       sustainEnabled Whether or not tick the last SEG_C value until noteOff.
    *
    *************************************************************************************************************************/
    
    Envelope(unsigned int delayMillis = 0, bool sustainEnabled = true);
    
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
    *  @brief       Sets the length of the delay segment.
    *
    *  @param       millis The new length of the delay segment, in milliseconds.
    *
    ********************************************************************************************/
    
    void setDelay(unsigned int millis);
    
    /*****************************************************************************************//*!
    *
    *  @brief       Returns the length of the delay segment.
    *
    *  @return      The length of the delay segment, in milliseconds.
    *
    ********************************************************************************************/
    
    unsigned int getDelay() const;
    
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
    
    /*! @copydoc GenUnit::setAmp() */
    void setAmp(double amp);
    
private:
    
    /*! Connects the end and starting poseg_ts of the loop
        to avoid harsh transitions */
    enum { CONNECTOR, DEL };
    
    /*! Changes the current segment in the sequence */
    void _changeSeg(segItr itr);
    
    /*! Does the actual work for modulate() */
    double _tick();
    
    /*! Resets the loop segments and changes to hidden connector segment */
    void _resetLoop();
    
    /*! Whether or not to return the last tick forever after SEG_C finished */
    bool _sustainEnabled;
    
    /*! Current segment number in the sequence */
    seg_t _currSegNum;
};

#endif /* defined(__Anthem__Envelope__) */
