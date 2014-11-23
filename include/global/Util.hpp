/*********************************************************************************************//*!
*
*  @file        Util.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Various utility functions for Anthem.
*
*************************************************************************************************/

#ifndef __Anthem__Util__
#define __Anthem__Util__

#include <string>

namespace Util
{
    /*! Returns an std::string with the current date in the format DD_MM_YYYY */
    extern std::string getDate();
    
    extern double noteToFreq(unsigned short note);

    extern void round(double& val, unsigned int bitWidth);
    
    extern unsigned short freqToNote(double freq);

    extern double semitonesToFreq(double baseFreq, double semitoneOffset);
    
    extern double freqToSemitones(double baseFreq, double newFreq);

    extern float getPassedTime(unsigned long start);
    
    extern double dbToAmp(double baseAmp, double dB);
    
    extern std::string checkFileName(std::string fname, const std::string& fileEnding);
}


#endif /* defined(__Anthem__Util__) */
