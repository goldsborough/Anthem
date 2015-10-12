/********************************************************************************************//*!
*
*  @file        Notetable.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Notetable.hpp"

#include <fstream>

Notetable::Notetable()
: LookupTable<double>(128, "Notes")
{
    std::ifstream file("/Users/petergoldsborough/Documents/Anthem/rsc/notes.table");
    
    // 128 MIDI notes. Number hasn't changed in the
    // last 30 years, probably wont't too soon.
    _data.resize(128);
    
    for (auto& note : _data)
    {
        file >> note;
    }
}
