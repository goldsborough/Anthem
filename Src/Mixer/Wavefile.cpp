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
    memcpy(header_.riffId, "RIFF", 4*sizeof(char));
    
    memcpy(header_.wavetype, "WAVE", 4*sizeof(char));
    
	memcpy(header_.fmtId, "fmt ", 4*sizeof(char));
    
	header_.fmtSize = 16;
    
	header_.fmtCode = 1;    // 1 = PCM
    
	header_.channels = channels;    // 1 = mono, 2 = stereo
    
	header_.samplerate = Global::samplerate;
    
    header_.bits = 16;
    
	header_.align = (channels * header_.bits) / 8;
    
    header_.byterate = header_.samplerate * header_.align;
    
	memcpy(header_.waveId, "data", 4*sizeof(char));
    
    setFileName(fname);
}

void Wavefile::setChannels(unsigned short channels)
{ header_.channels = channels; }

void Wavefile::setFileName(const std::string& fname)
{
    fname_ = Util::checkFileName(fname, ".wav");
}

void Wavefile::write(const SampleBuffer& sampleBuffer)
{
    unsigned int totalSamples = (unsigned) sampleBuffer.buffer.size();
    
    header_.waveSize = totalSamples * header_.align;
    
    header_.riffSize = header_.waveSize + sizeof(header_) - 8;
    
    // because the current buffer is of type double
    int16_t* buffer = new int16_t [totalSamples * 2];
    
    for (unsigned long n = 0, end = totalSamples * 2; n < end;)
    {
        Sample sample(sampleBuffer.buffer[n/2]);
        
        sample *= 32767;
        
        buffer[n++] = sample.left;
        buffer[n++] = sample.right;
    }
    
    std::ofstream file(fname_, std::ios::binary | std::ios::trunc);

    // Try writing the header_ and the data to file
    if (! file.write(reinterpret_cast<char*>(&header_), sizeof(header_)) ||
        ! file.write(reinterpret_cast<char*>(buffer), header_.waveSize))
    { throw std::runtime_error("Error writing to file"); }
}