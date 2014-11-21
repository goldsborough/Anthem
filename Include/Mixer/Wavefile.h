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

#include <fstream>
#include <string>
#include <deque>
#include <memory>

class Sample;

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
    
    Wavefile(const Wavefile& other);
    
    Wavefile& operator= (const Wavefile& other);
    
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
    *  @brief       Opens a new wavefile.
    *
    *  @param       fname The new file name.
    *
    *************************************************************************************************/
    
    void open(const std::string& fname);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Closes the current wavefile. Samples must be written manually.
    *
    *  @see         write()
    *
    *************************************************************************************************/
    
    void close();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Pushes a sample into the wavefile's sample buffer.
    *
    *  @param       sample A Sample object with two channels.
    *
    *************************************************************************************************/
    
    void process(const Sample& sample);
    
    /*********************************************************************************************//*!
    *
    *  @brief       Writes a sample buffer to a wavefile.
    *
    *  @param       sampleBuffer The SampleBuffer to write to a wavefile.
    *
    *  @see         flush()
    *
    *************************************************************************************************/
    
    void write();
    
    /*********************************************************************************************//*!
    *
    *  @brief       Flushes the contents of the Wavefile object's sample buffer.
    *
    *  @see         write()
    *
    *************************************************************************************************/
    
    void flush();
    
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
        
    } header_;
    
    /*! The sample buffer */
    std::deque<std::unique_ptr<Sample>> buffer_;
    
    /*! The file name */
    std::string fname_;
    
    /*! Wavefile stream object */
    std::ofstream file_;
};

#endif