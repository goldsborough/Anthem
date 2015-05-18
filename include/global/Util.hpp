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
    
    /*! Converts a MIDI note (0-127) to a frequency in Hertz. */
    extern double noteToFreq(unsigned short note);

    /*! Rounds a value to the nearest bin. */
    extern void round(double& val, unsigned int bins);
    
    /*! Converts a frequency to the nearest MIDI note (0-127). */
    extern unsigned short freqToNote(double freq);

    /*! Converts a semitone offset for a base frequency into a new frequency. */
    extern double semitonesToFreq(double baseFreq, double semitoneOffset);
    
    /*! Computes the semitone difference between two frequencies. */
    extern double freqToSemitones(double baseFreq, double newFreq);

    /*! Utility function for time measurement. Pass a clock_t. */
    extern float getPassedTime(unsigned long start);
    
    /*! Computes a new amplitude given a base amplitude and a decibel value. */
    extern double dbToAmp(double baseAmp, double dB);
    
    /*! Utility function for file names. */
    extern std::string checkFileName(std::string fname, const std::string& fileEnding);
}


#endif /* defined(__Anthem__Util__) */
