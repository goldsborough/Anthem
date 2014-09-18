/*********************************************************************************************//*!
*
*  @file        Wavefile.h
*
*  @author      Peter Goldsborough
*
*  @date        18/10/2014
*
*  @brief       Defines the Wavefile class.
*
*************************************************************************************************/

#ifndef __Anthem__Wavefile__
#define __Anthem__Wavefile__

#include <string>

class SampleBuffer;

/*********************************************************************************************//*!
*
*  @brief       Class for storing sample data in wavefiles.
*
*************************************************************************************************/

class Wavefile
{
    
public:
    
    /*********************************************************************************************//*!
    *
    *  @brief       Constructs a Wavefile object.
    *
    *  @param       fname The name of the wavefile.
    *
    *  @param       channels The number of channels for the wavefile, defaults to stereo (2).
    *
    *************************************************************************************************/
    
    Wavefile(const std::string& fname = std::string(),
             unsigned short channels = 2);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the number of channels for the wavefile.
    *
    *  @param       channels The number of channels, usually mono (1) or stereo (2).
    *
    *************************************************************************************************/
    void setChannels(unsigned short channels);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Sets the wavefile's file name.
    *
    *  @param       fname The new file name.
    *
    *************************************************************************************************/
    void setFileName(const std::string& fname);
    
    
    /*********************************************************************************************//*!
    *
    *  @brief       Writes a sample buffer to a wavefile.
    *
    *  @param       sampleBuffer The SampleBuffer to write to a wavefile.
    *
    *************************************************************************************************/
    void write(const SampleBuffer& sampleBuffer);
    
private:
    
    /*! Wavefile header */
    struct
    {
        uint8_t riffId[4]; // 'R' 'I' 'F' 'F'
        
        uint32_t riffSize; // chunks size in bytes
        
        uint8_t wavetype[4]; // 'W' 'A' 'V' 'E'
        
        uint8_t fmtId[4]; // 'f' 'm' 't' ''
        
        uint32_t fmtSize;
        
        uint16_t fmtCode; // 1 = pulse code modulation
        
        uint16_t channels;
        
        uint32_t samplerate;
        
        uint32_t byterate; // bytes per second
        
        uint16_t align; // bytes per sample * channel
        
        uint16_t bits; // one byte per channel, so 16 bits per sample
        
        uint8_t waveId[4]; // 'd' 'a' 't' 'a'
        
        uint32_t waveSize; // byte total
        
    } _header;
    
    // The file name 
    std::string _fname;
};

#endif