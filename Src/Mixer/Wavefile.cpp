//
//  Wavefile.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include <stdint.h>
#include <fstream>

#include "Wavefile.h"
#include "Global.h"
#include "Util.h"
#include "Sample.h"

Wavefile::Wavefile(const std::string& fname, unsigned short channels)
{
    memcpy(_header.riffId, "RIFF", 4*sizeof(char));
    
    memcpy(_header.wavetype, "WAVE", 4*sizeof(char));
    
	memcpy(_header.fmtId, "fmt ", 4*sizeof(char));
    
	_header.fmtSize = 16;
    
	_header.fmtCode = 1;    // 1 = PCM
    
	_header.channels = channels;    // 1 = mono, 2 = stereo
    
	_header.samplerate = Global::samplerate;
    
    _header.bits = 16;
    
	_header.align = (channels * _header.bits) / 8;
    
    _header.byterate = _header.samplerate * _header.align;
    
	memcpy(_header.waveId, "data", 4*sizeof(char));
    
    setFileName(fname);
}

void Wavefile::setChannels(unsigned short channels)
{ _header.channels = channels; }

void Wavefile::setFileName(const std::string& fname)
{
    _fname = Util::checkFileName(fname, ".wav");
}

void Wavefile::write(const SampleBuffer& sampleBuffer)
{
    unsigned int totalSamples = (unsigned) sampleBuffer.buffer.size();
    
    _header.waveSize = totalSamples * _header.align;
    
    _header.riffSize = _header.waveSize + sizeof(_header) - 8;
    
    // because the current buffer is of type double
    int16_t* buffer = new int16_t [totalSamples * 2];
    
    for (unsigned long n = 0, end = totalSamples * 2; n < end;)
    {
        Sample sample(sampleBuffer.buffer[n/2]);
        
        sample *= 32767;
        
        buffer[n++] = sample.left;
        buffer[n++] = sample.right;
    }
    
    std::ofstream file(_fname, std::ios::binary | std::ios::trunc);

    // Try writing the _header and the data to file
    if (! file.write(reinterpret_cast<char*>(&_header), sizeof(_header)) ||
        ! file.write(reinterpret_cast<char*>(buffer), _header.waveSize))
    { throw std::runtime_error("Error writing to file"); }
}