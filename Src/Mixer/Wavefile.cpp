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


Wavefile::WaveHeader* Wavefile::_getHeader(unsigned short channels) const
{
    WaveHeader* header = new WaveHeader;
    
    memcpy(header->riffId, "RIFF", 4*sizeof(char));
    
    memcpy(header->wavetype, "WAVE", 4*sizeof(char));
    
	memcpy(header->fmtId, "fmt ", 4*sizeof(char));
    
	header->fmtSize = 16;
    
	header->fmtCode = 1;    // 1 = PCM
    
	header->channels = channels;    // 1 = mono, 2 = stereo
    
	header->samplerate = Global::samplerate;
    
    header->bits = 16;
    
	header->align = (channels * header->bits) / 8;
    
    header->byterate = header->samplerate * header->align;
    
	memcpy(header->waveId, "data", 4*sizeof(char));
    
    return header;
}

void Wavefile::_checkFilename(std::string& fname) const
{
    if (fname.empty())
        fname = std::string(Util::getDate());
    
    typedef std::string::size_type szt;
    
    szt ext_ind = fname.find('.'); // extension index
    
    szt end = fname.size();
    
    if (ext_ind != std::string::npos && fname.substr(ext_ind,4) == ".wav")
        end = ext_ind;
    
    else fname += ".wav"; // if no extension, add .wav
    
    
    for (szt n = 0; n < end; n++)
    {
        char c = fname[n];
        
        if (! isalnum(c))
            fname.replace(n, 1, "_");
    }
    
    fname.insert(0, "/Users/petergoldsborough/Documents/Anthem/Wavefiles/");
}


void Wavefile::writeWav(const SampleBuffer& sampleBuffer,
                        std::string fname,
                        unsigned short channels) const
{
    
    std::unique_ptr<WaveHeader> header(_getHeader(channels));
    
    unsigned int totalSamples = (unsigned) sampleBuffer.buffer.size();
    
    header.get()->waveSize = totalSamples * header.get()->align;
    
    header.get()->riffSize = header.get()->waveSize + sizeof(*header.get()) - 8;
    
    _checkFilename(fname);
    
    // because the current buffer is of type double
    int16_t* buffer = new int16_t [totalSamples * 2];
    
    for (unsigned long n = 0, end = totalSamples * 2; n < end;)
    {
        Sample sample(sampleBuffer.buffer[n/2]);
        
        sample *= 32767;
        
        buffer[n++] = sample.left;
        buffer[n++] = sample.right;
    }
    
    std::ofstream file(fname, std::ios::binary | std::ios::trunc);

    if (! file.write(reinterpret_cast<char*>(header.get()), sizeof(*header.get())) ||
        ! file.write(reinterpret_cast<char*>(buffer), header.get()->waveSize))
        
    { throw std::runtime_error("Error writing to file"); }
}