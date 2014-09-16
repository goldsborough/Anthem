//
//  Wavefile.h
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Wavefile__
#define __Anthem__Wavefile__

#include <string>

class SampleBuffer;

struct Wavefile
{
    
public:
    
    void writeWav(const SampleBuffer& sampleBuffer,
                  std::string fname = std::string(),
                  unsigned short channels = 2) const;
    
    double* readWav(const std::string& fname);
    
private:
    
    struct WaveHeader
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
        
    };
    
    WaveHeader* _getHeader(unsigned short channels) const;
    
    /*! Checks filename for validity and manipulates string accordingly */
    void _checkFilename(std::string& fname) const;
};

#endif