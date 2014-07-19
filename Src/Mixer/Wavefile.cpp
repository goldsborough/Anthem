//
//  Wavefile.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Wavefile.h"
#include "Global.h"
#include "Utils.h"
#include "SampleData.h"

Wavefile::Wavefile(unsigned char ch)
{
    channels = ch;
    
    memcpy(wh.riff_id, "RIFF", 4*sizeof(char));

    memcpy(wh.wavetype, "WAVE", 4*sizeof(char));
    
	memcpy(wh.fmt_id, "fmt ", 4*sizeof(char));
    
	wh.fmt_size = 16;
    
	wh.fmt_code = 1;    // 1 = PCM
    
	wh.channels = channels;    // 1 = mono, 2 = stereo
    
	wh.samplerate = Global::samplerate;
    
    wh.bits = 16;
	wh.align = (channels * wh.bits) / 8;
    wh.byterate = wh.samplerate * wh.align;
    
	memcpy(wh.wave_id, "data", 4*sizeof(char));
}

void Wavefile::check_fname(std::string& fname)
{
    if (fname.empty())
        fname = std::string(getDate());
    
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
    
    fname.insert(0, "/Users/petergoldsborough/Documents/vibe/Wavefiles/");
}


void Wavefile::write_wav(SampleQueue& smplQ, std::string fname)
{
    
    unsigned long total_samples = (unsigned long) smplQ.size();
    
    unsigned long byte_total = total_samples * wh.align;
    
    wh.riff_size = (unsigned int) byte_total + sizeof(wh) - 8;
    
    wh.wave_size = (unsigned int) byte_total;
    
    check_fname(fname);
    
    // because the current buffer is of type double
    end_buff = new int16_t [total_samples * 2];
    
    for (unsigned long n = 0; n < total_samples*2;)
    {
        const SampleData& SD = smplQ.getpop();
        
        end_buff[n++] = SD.left * 32767;
        end_buff[n++] = SD.right * 32767;
    }
    
    f.open(fname, std::ios::binary | std::ios::trunc);

    if (! f.write(reinterpret_cast<char*>(&wh), sizeof(wh)) ||
        ! f.write(reinterpret_cast<char*>(end_buff), byte_total))
        
        throw std::runtime_error("Error writing to file");
}