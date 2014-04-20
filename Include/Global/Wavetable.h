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

class TextParser;

class Wavetable
{
    
public:
    
    struct Partial
    {
        
        int number;
        double amp;
        
        double phase = 0.0;
        double phase_incr;
    };
    
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
        SQUARE,
        
        SAW_2,
        SAW_4,
        SAW_8,
        SAW_16,
        SAW_32,
        SAW,
        
        TRIANGLE,
        RAMP,
        
        DIRECT_TRI,
        DIRECT_SQUARE,
        DIRECT_SAW,
        
        SMOOTH_SQUARE,
        SMOOTH_SAW,
        
        USER
    };
    
    void Init(unsigned int wtLen);
    
    ~Wavetable();
    
    double * getWaveform(const int mode);
    
    double * getNoise() { return 0;}//WT_NOISE; }
    
    double * genericWave(std::vector<Wavetable::Partial>& partials);
    
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
    
    double* genWave(unsigned char wavID,
                    unsigned char partialNumber = 64,
                    double ampFactor = 1,
                    unsigned char bitWidth = 16);
    
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
    
    TextParser * _textParser;
};

extern Wavetable wavetable;

#endif /* defined(__Synth__Waveforms__) */
