#include "AudioOutput.hpp"
#include "Global.hpp"
#include "Sample.hpp"



#include "Operator.hpp"


std::deque<std::unique_ptr<Sample>> AudioOutput::buffer_ = std::deque<std::unique_ptr<Sample>>();

Operator* AudioOutput::osc = 0;



AudioOutput::AudioOutput()
{
    try
    {
        if (getNumberOfDevices())
        {
            id_ = audio_.getDefaultOutputDevice();
        
            open(id_);
        }
        
        api_ = audio_.getCurrentApi();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    apiName_ = getApiName(api_);
}

std::string AudioOutput::getApiName(const RtAudio::Api &api)
{
    switch (api)
    {
        case RtAudio::LINUX_ALSA:
            return "ALSA";
            
        case RtAudio::LINUX_OSS:
            return "Open Sound System";
            
        case RtAudio::LINUX_PULSE:
            return "PulseAudio";
            
        case RtAudio::UNIX_JACK:
            return "Jack";
            
        case RtAudio::MACOSX_CORE:
            return "Core Audio";
            
        case RtAudio::WINDOWS_ASIO:
            return "ASIO";
            
        case RtAudio::WINDOWS_DS:
            return "Direct Sound";
            
        case RtAudio::WINDOWS_WASAPI:
            return "WASAPI";
            
        default:
            return "Unspecified";
    }
}

bool AudioOutput::isStreaming()
{
    try
    {
        return audio_.isStreamRunning();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return false;
}

bool AudioOutput::isOpen()
{
    try
    {
        return audio_.isStreamRunning();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return false;
}

int AudioOutput::callback_(void *output,
                           void *input,
                           unsigned int numberOfFrames,
                           double streamTime,
                           RtAudioStreamStatus status,
                           void *userData)
{
    double* outputBuffer = static_cast<double*>(output);
    
    //std::cout << streamTime << "\t" << count++ << "\t" << buffer_.size() << "\t" << std::endl;
    
    for (unsigned int n = 0; n < numberOfFrames; ++n)
    {
        //double left = buffer_.front()->left;
        //double right = buffer_.front()->right;
        
        double tick = osc->tick() / 2;
        
        osc->increment();
        
        *outputBuffer++ = tick;
        *outputBuffer++ = tick;
        
        //buffer_.pop_front();
    }
    
    return 0;
}

std::string AudioOutput::getApi() const
{
    return apiName_;
}

void AudioOutput::process(const Sample& sample)
{
    buffer_.push_back(std::unique_ptr<Sample>(new Sample(sample)));
}

void AudioOutput::open(id_t id, id_t channels, id_t frames)
{
    RtAudio::StreamParameters params;
    
    params.nChannels = channels;
    
    params.deviceId = id;
    
    audio_.openStream(&params,
                      NULL,
                      RTAUDIO_FLOAT64,
                      Global::samplerate,
                      &frames,
                      &callback_);
    
    id_ = id;
    
    device_ = getDevice(id_);
}

void AudioOutput::close()
{
    try
    {
        audio_.abortStream();
        audio_.closeStream();
        
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
}

void AudioOutput::start()
{
    try
    {
        audio_.startStream();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
}

void AudioOutput::stop()
{
    try
    {
        audio_.stopStream();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
}

AudioOutput::id_t AudioOutput::getNumberOfDevices()
{
    try
    {
        return audio_.getDeviceCount();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return 0;
}

AudioOutput::Device AudioOutput::getDevice(id_t id)
{
    try
    {
        return audio_.getDeviceInfo(id);
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return Device();
}

AudioOutput::id_t AudioOutput::getCurrentID() const
{
    return id_;
}

AudioOutput::Device AudioOutput::getCurrentDevice() const
{
    return device_;
}