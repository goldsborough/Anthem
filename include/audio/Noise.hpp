/*********************************************************************************************//*!
*
*  @file        Noise.hpp
*
*  @author      Peter Goldsborough
*
*  @date        11/10/2014
*
*  @brief       Defines the Noise oscillator class.
*
*************************************************************************************************/

#ifndef __Anthem__Noise__
#define __Anthem__Noise__

#include "Units.hpp"

#include <random>

class Filter;

/*********************************************************************************************//*!
*
*  @brief       A noise oscillator.
*
*  @details     Noise cannot effectively be stored in wavetables, as it must be completely and
*               continuously random. Therefore, I needed to create a separate noise oscillator
*               which generates samples directly, as opposed to reading them from a wavetable.
                                                                                                
*               There are currently five different noise colors to choose from:
*
*               -# __White noise__ is a random signal in its purest and most un-filtered
*                  form. It has all frequencies distributed equally throughout the signal.
*
*               -# __Pink noise__ is white noise filtered with a 3dB/octave low pass filter.
*                  In contrast to white noise, which has equal energy *per frequency*,
*                  ink noise has equal energy *per octave*. This is a significant
*                  difference as humans hear on the basis of octaves (logarithmically),
*                  and not completely linearly, meaning that for the human ear the
*                  difference between 400 and 800 Hertz (one octave) sounds the same as
*                  the octave jump between 6 KHz and 11 Khz, even though the absolute
*                  difference between thoes two ranges is 4.6 Khz! To address this
*                  phenomenon, pink noise has more energy in the lower, narrower octave
*                  ranges and less energy in the wider, higher octave ranges. Therefore,
*                  the few high energy values in the low frequency bands add up to equal
*                  the sum of lower energy values in the wider, higher frequency octave
*                  bands.
*
*               -# __Red noise__, also called "Brown" or "Brownian" noise, has an energy decrease of
*                  6dB/Octave. In opposition to all other noise types available for the Noise class,
*                  red noise is not named after its visual representation (e.g. white light has the same
*                  frequency spectrum as white noise), but rather after it's resemblance to Brownian
*                  motion. Another method for generating red noise besides filtering white noise
*                  appropriately is integrating a white noise signal.
*
*               -# __Blue noise__ has an energy *increase* of 3dB/octave, making it sound like a very
*                  high-pitched version of white-noise.
*
*               -# __Violet noise__'s frequency spectrum shows a 6dB increase per octave. It can also
*                  be generated by differentiating white noise.
*
*               References:
*
*               + http://www.acousticfields.com/white-noise-definition-vs-pink-noise/
*
*               + http://en.wikipedia.org/wiki/Colors_of_noise#White_noise
*
*               + http://www.mediacollege.com/audio/noise/
*
*  @todo        Implement gray noise.
*
*************************************************************************************************/


class Noise : public GenUnit
{
    
public:
    
    /*! Available ModDock s*/
    enum Docks { AMP };
    
    /*! The available noise colors */
    enum Colors
    {
        WHITE,
        PINK,
        RED,
        BLUE,
        VIOLET
    };
    
    /**********************************************************************************************************************//*!
    *
    *  @brief       Constructs a Noise object.
    *
    *  @param       color The initial noise color, usually a member of the Noise::Colors enum. Defaults to 0 (white noise).
    *
    *  @param       amp The initial amplitude value between 0 and 1. Defaults to 1.
    *
    **************************************************************************************************************************/
    
    Noise(unsigned short color = 0, double amp = 1);
    
    Noise(const Noise& other);
    
    Noise& operator= (const Noise& other);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the latest noise sample.
    *
    *  @return      A noise sample.
    *
    *****************************************************************************************************/
    
    double tick();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Generates a new noise sample, to be retrieved via tick().
    *
    *****************************************************************************************************/
    
    void update();
    
    /*************************************************************************************************//*!
    *
    *  @brief       Sets the noise color.
    *
    *  @param       color The new noise color, usually a member of the Noise::Colors enum.
    *
    *****************************************************************************************************/
    
    void setColor(unsigned short color);
    
    /*************************************************************************************************//*!
    *
    *  @brief       Returns the current color value from the Colors enum.
    *
    *  @return      The Noise oscillator's current noise color value.
    *
    *****************************************************************************************************/
    
    unsigned short getColor() const;
    
    /*! @copydoc GenUnit::setAmp() */
    void setAmp(double amp);
    
    /*! @copydoc GenUnit::getAmp() */
    double getAmp() const;
    
private:
    
    /*! Filter that changes the noise color */
    std::unique_ptr<Filter> filter_;
    
    /*! Current noise color */
    unsigned short color_;
    
    /*! Current random value */
    double rval_;
    
    /*! Mersenne-twister random number generator */
    std::mt19937 rgen_;
    
    /*! Random number distribution (uniform) */
    std::uniform_real_distribution<double> dist_;
};

#endif /* defined(__Anthem__Noise__) */
