#include "Wavefile.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "Sample.hpp"

#include <stdint.h>
#include <stdexcept>

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
    
    open(fname);
}

Wavefile::Wavefile(const Wavefile& other)
: fname_(other.fname_),
  file_(other.fname_, std::ios::out | std::ios::binary | std::ios::trunc),
  header_(other.header_)
{
    // Copy buffer data
    for(std::deque<std::unique_ptr<Sample>>::const_iterator itr = other.buffer_.begin(),
        end = other.buffer_.begin();
        itr != end;
        ++itr)
    {
        buffer_.push_back(std::unique_ptr<Sample>(new Sample(*(*itr))));
    }
}

Wavefile& Wavefile::operator= (const Wavefile& other)
{
    if (this != &other)
    {
        fname_ = other.fname_;
        
        file_.close();
        
        file_.open(fname_, std::ios::out | std::ios::binary | std::ios::trunc);
        
        header_ = other.header_;
        
        // Copy buffer data
        for(std::deque<std::unique_ptr<Sample>>::const_iterator itr = other.buffer_.begin(),
            end = other.buffer_.begin();
            itr != end;
            ++itr)
        {
            buffer_.push_back(std::unique_ptr<Sample>(new Sample(*(*itr))));
        }
    }
    
    return *this;
}

void Wavefile::setChannels(unsigned short channels)
{ header_.channels = channels; }

void Wavefile::open(const std::string& fname)
{
    fname_ = Util::checkFileName(fname, ".wav");
    
    file_.open(fname_, std::ios::out | std::ios::binary | std::ios::trunc);
    
    if (! file_)
    { throw std::invalid_argument("Error opening file!"); }
}

void Wavefile::close()
{
    file_.close();
}

void Wavefile::process(const Sample &sample)
{
    buffer_.push_back(std::unique_ptr<Sample>(new Sample(sample)));
}

void Wavefile::flush()
{
    buffer_.clear();
}

void Wavefile::write()
{
    unsigned int totalSamples = static_cast<unsigned int>(buffer_.size());
    
    header_.waveSize = totalSamples * header_.align;
    
    header_.riffSize = header_.waveSize + sizeof(header_) - 8;
    
    unsigned long twoTotalSamples = totalSamples * 2;
    
    // because the current buffer is of type double
    int16_t* outBuffer = new int16_t [twoTotalSamples];
    
    for (unsigned long n = 0; n < twoTotalSamples;)
    {
        // Convert to 16 bit integer
        (*buffer_.front()) *= 32767;
        
        // Write to both channels (first channel 1, then channel 2)
        outBuffer[n++] = buffer_.front()->left;
        outBuffer[n++] = buffer_.front()->right;
        
        buffer_.pop_front();
    }

    // Try writing the header_ and the data to file
    if (! file_.write(reinterpret_cast<char*>(&header_), sizeof(header_)) ||
        ! file_.write(reinterpret_cast<char*>(outBuffer), header_.waveSize))
    { throw std::runtime_error("Error writing to file"); }
}