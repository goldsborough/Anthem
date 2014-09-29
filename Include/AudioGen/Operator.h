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
    enum Docks
    {
        AMP,
        SEMI_OFFSET,
        CENT_OFFSET
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
    *  @param       semitones The semitone offset, between -48 and +48.
    *
    *  @see         setCents()
    *
    *****************************************************************************************************/
    
    void setSemitoneOffset(short semitoneOffset);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Operator's current semitone offset, if any.
    *
    *  @return      The semitone offset.
    *
    *****************************************************************************************************/
    
    short getSemitoneOffset() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets a cent offset to all currently available oscillators.
    *
    *  @param       cents The cent offset, between 0 and 100.
    *
    *  @see         setSemis()
    *
    *****************************************************************************************************/
    
    void setCentOffset(short centOffset);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Operator's current cent offset, if any.
    *
    *  @return      The cent offset.
    *
    *****************************************************************************************************/
    
    short getCentOffset() const;
    
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
    *  @brief       Returns the operator's wavetable's ID.
    *
    *  @return      The wavetable's ID.
    *
    *****************************************************************************************************/
    
    short getWavetableID() const;
    
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
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the frequency of a note.
    *
    *  @param       ind The index of the note.
    *
    *  @return      The note's frequency value in Hertz.
    *
    *****************************************************************************************************/
    
    double getNoteFreq(unsigned short ind);
    
    /*! @copydoc GenUnit::setAmp() */
    void setAmp(double amp);
    
private:
    
    typedef std::vector<Oscillator*> noteVec;
    
    /*! The std::vector of pointers to Oscillators that holds current notes */
    noteVec _notes;
    
    /*! The current semitone offset value */
    unsigned short _semitoneOffset;
    
    /*! The current cent offset value */
    unsigned short _centOffset;
    
    /*! The id of the current wavetable */
    short _wavetableID;
};

#endif /* defined(__Anthem__Operator__) */
