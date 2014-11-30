/********************************************************************************************//*!
*
*  @file        AudioOutput.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "AudioOutput.hpp"
#include "Global.hpp"
#include "Sample.hpp"
#include "Anthem.hpp"

Anthem* AudioOutput::anthem_ = 0;

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

void AudioOutput::init(Anthem *anthem)
{
    anthem_ = anthem;
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
    
    for (unsigned int n = 0; n < numberOfFrames; ++n)
    {
        Sample sample = anthem_->tick_();
        
        *outputBuffer++ = sample.left;
        *outputBuffer++ = sample.right;
        
        anthem_->increment_();
    }
    
    return 0;
}

std::string AudioOutput::getApi() const
{
    return apiName_;
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