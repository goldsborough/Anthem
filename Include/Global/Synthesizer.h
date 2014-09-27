/*********************************************************************************************//*!
*
*  @file        Synthesizer.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Main initialization class.
*
*  @details     Will be changed at some point ... 
*
*************************************************************************************************/

#ifndef __Anthem__Synthesizer__
#define __Anthem__Synthesizer__

class Synthesizer
{
    
public:
    
    // Types of polyphony
    
    enum Priority
    {
        LAST,      // first voices are replaced by new ones
        FIRST,     // a key must be released to add a new one
        HIGHEST,   // lower pitches are released first
        LOWEST     // higher pitches are released first
    };
    
    Synthesizer(unsigned int smplr = 44100, unsigned short wtLen = 4095);
    
};

#endif /* defined(__Anthem__Synthesizer__) */
