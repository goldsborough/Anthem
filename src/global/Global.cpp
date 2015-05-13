/********************************************************************************************//*!
*
*  @file        Global.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Global.hpp"
#include "Wavetable.hpp"
#include "Pantable.hpp"
#include "Notetable.hpp"

// Definition of variables declared 'extern' in their header files
WavetableDatabase wavetableDatabase;
PantableDatabase pantableDatabase;
Notetable notetable;

namespace Global
{
    unsigned int samplerate = 0;
    
    unsigned int nyquistLimit = 0;
    
    unsigned short wavetableLength = 0;
    
    double tableIncr = 0;
    
    void init(const unsigned int smplr, const unsigned int wavetableLen)
    {
        samplerate = smplr;
        nyquistLimit = smplr / 2;
        
        wavetableLength = wavetableLen;
        
        tableIncr = static_cast<double>(wavetableLength) / smplr;
        
        wavetableDatabase.init();
    }
}