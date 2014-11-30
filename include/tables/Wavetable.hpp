/*********************************************************************************************//*!
*
*  @file        Wavetable.hpp
*
*  @author      Peter Goldsborough
*
*  @date        25/09/2014
*
*  @brief       Wavetable management classes.
*
*  @details     This file defines the Wavetable class and the WavetableDB class.
*
*************************************************************************************************/

#ifndef __Anthem__Wavetable__
#define __Anthem__Wavetable__

#include "LookupTable.hpp"

#include <string>
#include <vector>

/*****************************************************************************//*!
*
*  Waveforms can be generated either purely mathematically through
*  various formulas or, in case of a square wave, simply by setting
*  half the wavetable to 1 and the other half to 0, thought this will
*  never come near to the complex waveforms found naturally. Of course
*  those can actually never be formed computationally, since most wave-
*  tables that occur naturally are made up of an uncountable number of
*  partials. However, we can get very close through additive approaches,
*  so by adding integer mutliples of the fundamental pitch in a variety
*  of ways:
*
*  Square wave: 2 - 64 partials
*
*                            _____       _____
*                           |     |     |     |
*                      _____|     |_____|     |_____
*
*  Square waves are generated by adding all the odd partials, so 3, 5, 7
*  etc. times the fundamental pitch. The amplitude is indirectly
*  proportinal to the partial number, so the third partial will be 1/3 the
*  amplitude, the 5th partial 1/5th and so on. 64 partials is the maximum
*  amount of partials we will compute here and is really more than enough
*  to get near a natural sound.
*
*  Sawtooth wave: 2 - 64 partials
*
*                             |\  |\  |\
*                             | \ | \ | \
*                             |  \|  \|  \
*
*  Sawtooth waves descend from maximum amplitude down to 0 linearly and then
*  jump right back. This is computed by adding all partials with indirectly
*  proportional amplitudes.
*
*  Ramp wave: 64 partials
*
*                                /|  /|  /|
*                               / | / | / |
*                              /  |/  |/  |
*
*  Ramp waves are somewhat the inverse of sawtooth waves in the way that
*  they ascend to maximum amplitude from 0, linearly again, and then jump
*  right back down. They are accomplished by using the same approach as
*  for saw waves but with all negative amplitude values.
*
*  Triangle wave: 64 Partials
*
*                              /\    /\    /\
*                             /  \  /  \  /  \
*                            /    \/    \/    \
*
*  Triangle waves can be seen as linear sine waves, or a combination of
*  ramp and saw waves. They ascend from 0 to 2 linearly and then descend
*  to 0 linearly again. They are created by adding all odd partials with
*  alternating sign for the amplitude.
*
*
*  More on this here: http://bit.ly/1nkyXfW
*
*
*  The wavetable must be initalized as _wtLength + 1, as we are using
*  linear interpolation. Say the current sample is sample 12345, at a table
*  length of 4096, the index increment per sample in the wavetable is
*  4096/44100 = 0.0928..., so the index for sample 12345 would be 1146.60....
*  This means that we must interpolate between index 1146 and index 1147.
*  Now, the table index for sample 44100 is 4096 of course, when doing
*  interpolation we would need to access item 4067. If we thus do not append
*  the beginning of the wave to the end, we get an index error.
*
*****************************************************************************/

/*********************************************************************************************//*!
*
*  @brief       A partial/harmonic/overtone.
*
*  @details     The Partial struct represents a single Partial for generating waveforms through
*               Fourier/Additive synthesis. A partial can be expressed through its number, an
*               integer multiple of the fundamental frequency, its amplitude and an optional
*               phase offset.
*
*************************************************************************************************/

struct Partial
{
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a Partial object.
    *
    *  @param       number The Partial's number e.g. 2 means it has twice the frequency of the fundamental.
    *
    *  @param       ampl The initial amplitude value.
    *
    *  @param       phsOffs An optional initial phase offset in degrees, defaults to 0 (no offset).
    *
    *****************************************************************************************************/
    
    Partial(unsigned short number, double ampl, double phsOffs = 0)
    : num(number), amp(ampl), phaseOffs(phsOffs)
    { }
    
    /*! The Partial's number, stays const. */
    const unsigned short num;
    
    /*! The amplitude value. */
    double amp;
    
    /*! A phase offset */
    double phaseOffs;
};

/*********************************************************************************************//*!
*
*  @brief       Class for storing waveform lookup tables.
*
*  @details     This is the main class in Anthem for storing and looking up values from waveforms
*               (e.g. sine, saw, square wave). A wavetable can be constructed either by passing
*               it a pointer to waveform values directly or through Additive Synthesis, in
*               combination with the Partial struct. The Wavetable class implements reference
*               counting.
*
*************************************************************************************************/

class Wavetable : public LookupTable<double>
{
    
public:
    
    enum class MathematicalWaveform
    {
        DIRECT_TRIANGLE,
        DIRECT_SQUARE,
        DIRECT_SAW,
        
        SMOOTH_SQUARE,
        SMOOTH_SAW
    };
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a Wavetable additively.
    *
    *  @details     An additive wavetable is constructed by adding a sequence of Partial objects
    *               together. Optionally they can smoothed out with sigma/lanczos approximation which
    *               also reduces the Gibbs effect.
    *
    *  @param       start An iterator to the start of a sequence of Partial objects (e.g. std::vector<Partial>).
    *
    *  @param       end An iterator to the end of a sequence of Partial objects.
    *
    *  @param       wtLen The length of the wavetable to construct, 4095 is used throughout Anthem.
    *
    *  @param       masterAmp Attenuation value for all values in the table.
    *
    *  @param       sigmaAprox Boolean whether or not to use sigma approximation to smooth out the values.
    *
    *  @param       bitWidth Number of bits to scale to, defaults to 16 (bits).
    *
    *  @param       id The wavetable's id, defaults to -1.
    *
    *****************************************************************************************************/
    
    template <class PartItr>
    Wavetable(PartItr start, PartItr end,
              index_t wtLength,
              double masterAmp = 1,
              bool sigmaAprox = false,
              unsigned int bitWidth = 16,
              index_t id = 0,
              const std::string& name = std::string());
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a Wavetable object from a pointer.
    *
    *  @param       ptr The pointer to waveform values to store and manage.
    *
    *  @param       wtLength The length of the wavetable (and the array of values pointed to by ptr).
    *
    *  @param       id The wavetable's id, defaults to -1.
    *
    *****************************************************************************************************/
    
    Wavetable(double * ptr = 0,
              index_t wtLength = 0,
              index_t id = 0,
              const std::string& name = std::string());
    
    /*************************************************************************************************//*!
    *
    *  @brief       Constructs a Wavetable object with a directly calculated, mathematical waveform.
    *
    *  @param       waveform A MathematicalWaveform member to generate.
    *
    *  @param       wtLength The length of the wavetable (and the array of values pointed to by ptr).
    *
    *  @param       id The wavetable's id, defaults to -1.
    *
    *****************************************************************************************************/
    
    Wavetable(MathematicalWaveform waveform,
              index_t wtLength,
              index_t id = 0,
              const std::string& name = std::string());
    
private:
    
    /*! Generates a sawtooth wave directly/mathematically */
    double* directSaw_() const;
    
    /*! Generates a square wave directly/mathematically */
    double* directSquare_() const;
    
    /*! Generates a triangle wave directly/mathematically */
    double* directTriangle_() const;
    
    /*! Generates a smoothed sawtooth wave directly/mathematically */
    double* smoothSaw_() const;
    
    /*! Generates a smoothed square wave directly/mathematically */
    double* smoothSquare_() const;
};

/*********************************************************************************************//*!
*
*  @brief       Stores Anthem's wavetables.
*
*  @details     The WavetableDatabase class manages all of Anthem's wavetables
*               and is responsible for providing Oscillators with Wavetables.
*
*************************************************************************************************/

class WavetableDatabase
{
    
public:
    
    enum Wavetables
    {
        SINE,
        SINE_2,
        SINE_4, // The number is the number of bits, not partials
        SINE_8,
        
        SQUARE, // with sigma
        SQUARE_2,
        SQUARE_4,
        SQUARE_8,
        SQUARE_16,
        SQUARE_32,
        SQUARE_64,
        
        SAW, // with sigma
        SAW_2,
        SAW_4,
        SAW_8,
        SAW_16,
        SAW_32,
        SAW_64,
        
        TRIANGLE,
        RAMP,
        
        DIRECT_TRIANGLE,
        DIRECT_SQUARE,
        DIRECT_SAW,
        
        SMOOTH_SQUARE,
        SMOOTH_SAW
    };
    
    typedef unsigned long index_t;
    
    /*********************************************************************************************//*!
    *
    *  @brief       Initialzes the WavetableDatabase.
    *
    *  @details     The WavetableDatabase is initialized by reading all available wavetables from the
    *               wavetable folder.
    *
    *************************************************************************************************/
    
    void init();
    
    Wavetable& operator[] (index_t wt);
    
    const Wavetable& operator[] (index_t wt) const;
    
    /*************************************************************************//*!
    *
    *   @brief Writes Wavetable object to file.
    *
    *   @param fname The name of the file to write to.
    *
    *   @param wt The wavetable object to write to file. 
    *
    ****************************************************************************/
    
    void writeWavetable(const std::string& fname, const Wavetable& wt) const;
    
    /*! Returns the number of wavetables stored. */
    index_t size() const;
    
private:
    
    /*************************************************************************//*!
    *
    *   @brief Reads a wavetable and returns a double pointer.
    *
    *   @param fname The name of the file to read from.
    *
    ****************************************************************************/
    
    double* readWavetable(const std::string& fname) const;
    
    /*! Vector of Wavetable objects */
    std::vector<Wavetable> tables_;
};

extern WavetableDatabase wavetableDatabase;

#endif /* defined(__Anthem__Wavetable__) */
