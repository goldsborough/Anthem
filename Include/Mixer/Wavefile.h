//
//  Wavefile.h
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Wavefile__
#define __Anthem__Wavefile__

#include <stdint.h>
#include <fstream>

class SampleBuffer;

class Wavefile
{
    
public:
    
    /*!
     @brief   Populates the waveheader with everything that doesn't depend on the data buffer
     */
    
    Wavefile(unsigned char ch = 2);
    
    ~Wavefile(){ f.close(); delete end_buff; };
    
    void write_wav(SampleBuffer& smplQ, std::string fname = "");
    
    double * read_wav(const std::string& fname);
    
private:
    
    struct waveheader
    {
        uint8_t riff_id[4]; // 'R' 'I' 'F' 'F'
        
        uint32_t riff_size; // chunks size in bytes
        
        uint8_t wavetype[4]; // 'W' 'A' 'V' 'E'
        
        uint8_t fmt_id[4]; // 'f' 'm' 't' ''
        
        uint32_t fmt_size;
        
        uint16_t fmt_code; // 1 = pulse code modulation
        
        uint16_t channels;
        
        uint32_t samplerate;
        
        uint32_t byterate; // bytes per second
        
        uint16_t align; // bytes per sample * channel
        
        uint16_t bits; // one byte per channel, so 16 bits per sample
        
        uint8_t wave_id[4]; // 'd' 'a' 't' 'a'
        
        uint32_t wave_size; // byte total
    };
    
    waveheader wh; // the waveheader
    
    int16_t * end_buff; // the final buffer
    unsigned char channels;
    
    // the filestream
    std::ofstream f;
    
    /*!
        @brief checks filename for validity and manipulates string accordingly
    */
    void check_fname(std::string& fname);
};

#endif