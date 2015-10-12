/********************************************************************************************//*!
*
*  @file        Wavefile.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Wavefile.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "Sample.hpp"

#include <stdint.h>
#include <stdexcept>

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
    
    open(fname);
}

Wavefile::Wavefile(const Wavefile& other)
: _fname(other._fname),
  _file(other._fname, std::ios::out | std::ios::binary | std::ios::trunc),
  _header(other._header)
{
    // Copy buffer data
    for(std::deque<std::unique_ptr<Sample>>::const_iterator itr = other._buffer.begin(),
        end = other._buffer.begin();
        itr != end;
        ++itr)
    {
        _buffer.push_back(std::unique_ptr<Sample>(new Sample(*(*itr))));
    }
}

Wavefile& Wavefile::operator= (const Wavefile& other)
{
    if (this != &other)
    {
        _fname = other._fname;
        
        _file.close();
        
        _file.open(_fname, std::ios::out | std::ios::binary | std::ios::trunc);
        
        _header = other._header;
        
        // Copy buffer data
        for(std::deque<std::unique_ptr<Sample>>::const_iterator itr = other._buffer.begin(),
            end = other._buffer.begin();
            itr != end;
            ++itr)
        {
            _buffer.push_back(std::unique_ptr<Sample>(new Sample(*(*itr))));
        }
    }
    
    return *this;
}

void Wavefile::setChannels(unsigned short channels)
{ _header.channels = channels; }

void Wavefile::open(const std::string& fname)
{
    _fname = Util::checkFileName(fname, ".wav");
    
    _file.open(_fname, std::ios::out | std::ios::binary | std::ios::trunc);
    
    if (! _file)
    { throw std::invalid_argument("Error opening file!"); }
}

void Wavefile::close()
{
    _file.close();
}

void Wavefile::process(const Sample &sample)
{
    _buffer.push_back(std::unique_ptr<Sample>(new Sample(sample)));
}

void Wavefile::flush()
{
    _buffer.clear();
}

void Wavefile::write()
{
    unsigned int totalSamples = static_cast<unsigned int>(_buffer.size());
    
    _header.waveSize = totalSamples * _header.align;
    
    _header.riffSize = _header.waveSize + sizeof(_header) - 8;
    
    unsigned long twoTotalSamples = totalSamples * 2;
    
    // because the current buffer is of type double
    int16_t* outBuffer = new int16_t [twoTotalSamples];
    
    for (unsigned long n = 0; n < twoTotalSamples;)
    {
        // Convert to 16 bit integer
        (*_buffer.front()) *= 32767;
        
        // Write to both channels (first channel 1, then channel 2)
        outBuffer[n++] = _buffer.front()->left;
        outBuffer[n++] = _buffer.front()->right;
        
        _buffer.pop_front();
    }

    // Try writing the _header and the data to file
    if (! _file.write(reinterpret_cast<char*>(&_header), sizeof(_header)) ||
        ! _file.write(reinterpret_cast<char*>(outBuffer), _header.waveSize))
    { throw std::runtime_error("Error writing to file"); }
}