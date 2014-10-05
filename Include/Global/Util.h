/*********************************************************************************************//*!
*
*  @file        Util.h
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
    
    extern double noteToFreq(unsigned short n);

    extern unsigned short freqToNote(double freq);

    extern double centToFreq(double baseFreq, int centOffset);

    extern double semiToFreq(double baseFreq, int semiToneOffset);

    extern float getPassedTime(unsigned long start);
    
    extern double dbToAmp(double baseAmp, double dB);
    
    extern std::string checkFileName(std::string fname, const std::string& fileEnding);
}


#endif /* defined(__Anthem__Util__) */
