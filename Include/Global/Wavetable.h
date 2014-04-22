//
//  Wavetable.h
//  Synth
//
//  Created by Peter Goldsborough on 22/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Waveforms__
#define __Synth__Waveforms__

#include <vector>
#include <string>

/*****************************************************************************//*!
 *
 *  Waveforms can be generated either purely mathematically through
 *  various formulas or, in case of a square wave, simply by setting
 *  half the wavetable to 1 and the other half to 0, thought this will
 *  never come near to the complex waveforms found naturally. Of course
 *  those can actually never be formed computationally, since most wave-
 *  _tables that occur naturally are made up of an uncountable number of
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

class Wavetable
{
    
public:
    
    struct Partial
    {
        Partial(unsigned short number, double ampl,
                double phsOffs = 0)
        : num(number), amp(ampl), phaseOffs(phsOffs)
        { }
    
        const unsigned short num;
        double amp;
        
        double phaseOffs;
    };
    
    typedef std::vector<Partial> partVec;
    
    enum Modes
    {
        NONE = -1,
        
        SINE_2,
        SINE_4, // The number is the number of bits, not partials
        SINE_8,
        SINE,
        
        // Square waves, 2 - 64 partials
        SQUARE_2,
        SQUARE_4,
        SQUARE_8,
        SQUARE_16,
        SQUARE_32,
        SQUARE_64,
        SQUARE, // with sigma
        
        SAW_2,
        SAW_4,
        SAW_8,
        SAW_16,
        SAW_32,
        SAW_64,
        SAW, // with sigma
        
        TRIANGLE,
        RAMP,
        
        DIRECT_TRI,
        DIRECT_SQUARE,
        DIRECT_SAW,
        
        SMOOTH_SQUARE,
        SMOOTH_SAW,
    };
    
    void Init(unsigned int wtLen);
    
    ~Wavetable();
    
    double * getWaveform(const int mode);
    
    double * genWave(const partVec& partials,
                     double masterAmp = 1.0, bool sigmaAprox = false,
                     unsigned int bitWidth = 16);
    
private:
    
    // Rounds values within a certain bitWidth
    // to the closest value
    
    void round(double& val, unsigned int bitWidth);
    
    // Generate the wavetables
    
    /*!
     @brief      Generates a waveform through Fourier Synthesis
     @discussion
     @param      wavID needed for certain behaviour for some waveforms,
                 0 = SINE, 1 = SQUARE, 2 = SAW, 3 = RAMP, 4 = TRIANGLE
     @param      partialNumber Number of partials to add
     @param      ampFactor The factor to mulitply the amplitude values with
     @param      bitWidth BitWidth for sine waves
     @return     The wavetable
     */
    
    double* directSaw();
    double* directSquare();
    double* directTriangle();
    
    double* smoothSaw();
    double* smoothSquare();
    
    /*!
     @brief      Generates a noise wave
     @discussion Generates a noise wave by generating random samples
     @return     The noise wavetable
    */
    
    double* genNoise();
    
    double* _readWavetable(const int waveNum);
        
   /************************************************************************************//*!
   * The wtLength was a tricky one to find and reason upon. A compromise must be foun
   * between a reasonable length or size in KB for it to fit into at least L2 cache but
   * still be able to give a good resolution for the "compressed" samples. Samples will
   * later be retrieved via linear interpolation.
   *
   * A WT size of 2^12, or 4096 (opimizing the range by using a power of two), is
   * calculated as follows:
   *
   * WaveTableLength * 8 bytes of precision (double) / 1024 = 32KB
   ****************************************************************************************/
        
    unsigned int _wtLength;

    double _fundIncr; // the fundamental index increment

    std::vector<double*> _tables;
        
    std::vector<std::string> _wtNames;
};

extern Wavetable wavetable;

#endif /* defined(__Synth__Waveforms__) */
