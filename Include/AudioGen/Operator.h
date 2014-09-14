/*********************************************************************************************//*!
*
*  @file        Operator.h
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       Defines the Operator class.
*
*************************************************************************************************/

#ifndef __Anthem__Operator__
#define __Anthem__Operator__

#include "Units.h"

class Oscillator;

/*************************************************************************************************//*!
*
*  @brief       The Operator class.
*
*  @details     An Operator is an interfacing class for playing notes (i.e. pitches/frequencies).
*               For each note, an oscillator of that frequency is created and added to the other
*               notes to create a final signal. These operator signals are then synthesized via
*               Frequency Modulation.
*
*****************************************************************************************************/

class Operator : public GenUnit
{
    
public:
    
    /*! The available ModDocks/parameters to
        modulate via other GenUnits */
    enum DOCKS
    {
        AMP,
        FREQ_SEMI,
        FREQ_CENT
    };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs an Operator.
    *
    *  @param       wt The id of the wavetable to play. Usually a member of WavetableDB::Wavetables.
    *                  Defaults to WavetableDB::Wavetables::SINE, or 0.
    *
    *  @param       amp The initial amplitude value between 0 and 1. Defaults to 1.
    *
    *****************************************************************************************************/
    
    Operator(short wt = 0, double amp = 1);
    
    ~Operator();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a sample.
    *
    *  @return      The generated sample.
    *
    *****************************************************************************************************/
    
    double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets a semitone offset to all currently available oscillators.
    *
    *  @param       semis The semitone offset, between -48 and +48.
    *
    *  @param       permanent Wether or not to set the new value as the Operator's base value.
    *
    *  @see         setCents()
    *
    *****************************************************************************************************/
    
    void setSemis(double semis, bool permanent = true);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets a cent offset to all currently available oscillators.
    *
    *  @param       cents The cent offset, between 0 and 100.
    *
    *  @param       permanent Wether or not to set the new value as the Operator's base value.
    *
    *  @see         setSemis()
    *
    *****************************************************************************************************/
    
    void setCents(double cents, bool permanent = true);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the operator's wavetable.
    *
    *  @details     Behind the scenes the operator just changes the wavetable of its oscillators.
    *
    *  @param       wt The id of the new wavetable. Usually a member of WavetableDB::Wavetables.
    *
    *****************************************************************************************************/
    
    void setWavetable(short wt);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Adds a note to the operator.
    *
    *  @details     A note is in this case nothing else than a frequency. The operator creates a new
    *               oscillator and initializes it with this pitch.
    *
    *  @param       frq The frequency of the note to add.
    *
    *****************************************************************************************************/
    
    void addNote(double frq);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Releases a note from the operator.
    *
    *  @details     This form of relNote() looks for an oscillator within the operator's vector of 
    *               oscillators that has the passed frequency and deletes it, if found.
    *
    *  @param       frq The frequency of the note to release.
    *
    *****************************************************************************************************/
    
    void relNote(double frq);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Releases a note from the operator.
    *
    *  @details     This form of relNote() releases an oscillator from the operator's vector of
    *               oscillators, given its index (in that vector).
    *
    *  @param       ind The index of the oscillator to release.
    *
    *****************************************************************************************************/
    
    void relNote(unsigned short ind);
    
private:
    
    typedef std::vector<Oscillator*> oscVec;
    
    /*! ModDock initialization */
    void _initModDocks();
    
    /*! The std::vector of pointers to Oscillators that holds current notes */
    oscVec _oscs;
    
    /*! The current semitone offset value */
    unsigned short _semis;
    
    /*! The current cent offset value */
    unsigned short _cents;
    
    /*! The id of the current wavetable */
    unsigned int _wavetableId;
};

#endif /* defined(__Anthem__Operator__) */
