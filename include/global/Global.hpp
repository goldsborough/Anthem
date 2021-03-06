/*********************************************************************************************//*!
*
*  @file        Global.hpp
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Global declarations namespace.
*
*  @details     This file holds global values in a namespace, such as pi or the samplerate.
*
*************************************************************************************************/

#ifndef __Anthem__Global__
#define __Anthem__Global__

// http://goo.gl/748HMW

namespace Global
{    
    /*! π */
    const double pi = 3.14159265358979;
    
    /*! 2π */
    const double twoPi = 6.28318530717958;
    
    /*! Square root of two. */
    const double sqrt2 = 1.41421356237309;
    
    /*! The samplerate used, usually 44100 Hz. */
    extern unsigned int samplerate;
    
    /*! The nyquist sampling limit, half the sampling rate. */
    extern unsigned int nyquistLimit;
    
    /*! The wavetable length. */
    extern unsigned short wavetableLength;
    
    /*! The fundamental table increment = wavetableLength/samplerate. */
    extern double tableIncrement;
    
    /*************************************************************************************************//*!
    *
    *  @brief       Initializes the namespace.
    *
    *  @param       smplr The samplerate.
    *
    *  @param       wavetableLength The wavetable length.
    *
    *****************************************************************************************************/
    extern void init(const unsigned int smplr = 48000,
                     const unsigned int wavetableLength = 4095);
};

#endif /* defined(__Anthem__Globals__) */