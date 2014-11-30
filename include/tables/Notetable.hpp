/*********************************************************************************************//*!
*
*  @file        Notetable.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Notetable for piano key to frequency conversions.
*
*************************************************************************************************/

#ifndef Anthem_Notes_hpp
#define Anthem_Notes_hpp

#include "LookupTable.hpp"

/*********************************************************************************************//*!
*
*  @brief       Table with piano key to frequency conversions for lookup.
*
*  @details     The Notetable holds frequency values for the 128 MIDI notes.
*               The note number is the index where the frequency is found ([48] = 440Hz).
*
*************************************************************************************************/

struct Notetable : public LookupTable<double>
{
    /*! Reads the Notetable data from disk. */
    void init();
};

extern Notetable notetable;

#endif
