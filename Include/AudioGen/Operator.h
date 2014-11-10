/*********************************************************************************************//*!
*
*  @file        Operator.h
*
*  @author      Peter Goldsborough
*
*  @date        10/11/2014
*
*  @brief       Defines the Operator class.
*
*************************************************************************************************/

#ifndef __Anthem__Operator__
#define __Anthem__Operator__

#include "Oscillator.h"
#include "Units.h"


class Operator : public Oscillator, public GenUnit
{
    
public:
    
    typedef unsigned short note_t;
    
    /*! Available ModDocks for modulation */
    enum Docks
    {
        AMP
    };
    
    Operator(short wt = -1, double frq = 1,
             double amp = 1, short phaseOffset = 0,
             double ratio = 1, double freqOffset = 0);
    
    double tick();
    
    void increment();
    
    void modulateFrequency(double value);
    
    void setNoteFrequency(double frequency);
    
    double getNoteFrequency() const;
    
    double getModBaseFrequency() const;
    
    void setNote(note_t note);
    
    note_t getNote() const;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the Oscillator's frequency offset.
    *
    *  @param       Hz The new frequency offset, in Hertz.
    *
    *****************************************************************************************************/
    
    void setFrequencyOffset(double Hz);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the Oscillator's frequency offset.
    *
    *  @return      The current frequency offset, in Hertz.
    *
    *****************************************************************************************************/
    
    double getFrequencyOffset() const;
    
    void setSemitoneOffset(double semitones);
    
    double getSemitoneOffset() const;
    
    void setRatio(double ratio);
    
    double getRatio() const;
    
    void setAmp(double amp);
    
    double getAmp() const;
    
private:
    
    /*! The frequency ratio of the Operator
        relative to the current note */
    double ratio_;
    
    /*! Current frequency offset value in table index increment */
    double indexOffset_;
    
    /*! Current frequency offset value in Hertz */
    double freqOffset_;
    
    /*! Current frequency offset value in semitones */
    double semitoneOffset_;
    
    /*! The frequency of the original note, without any ratio */
    double noteFreq_;
    
    /*! The note that was originally input */
    note_t note_;
    
    /*! The base frequency for modulation in Hertz */
    double modBaseFreq_;
    
    /*! The base frequency for modulation as a note */
    note_t modBaseNote_;
};

#endif /* defined(__Anthem__Operator__) */
