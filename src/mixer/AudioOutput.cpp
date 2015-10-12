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

Anthem* AudioOutput::_anthem = 0;

AudioOutput::AudioOutput()
{
    try
    {
        if (getNumberOfDevices())
        {
            _id = _audio.getDefaultOutputDevice();
            
            open(_id);
        }
        
        _api = _audio.getCurrentApi();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    _apiName = getApiName(_api);
}

void AudioOutput::init(Anthem *anthem)
{
    _anthem = anthem;
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
        return _audio.isStreamRunning();
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
        return _audio.isStreamRunning();
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return false;
}

int AudioOutput::_callback(void *output,
                           void *input,
                           unsigned int numberOfFrames,
                           double streamTime,
                           RtAudioStreamStatus status,
                           void *userData)
{
    double* outputBuffer = static_cast<double*>(output);
    
    for (unsigned int n = 0; n < numberOfFrames; ++n)
    {
        Sample sample = _anthem->_tick();
        
        *outputBuffer++ = sample.left;
        *outputBuffer++ = sample.right;
        
        _anthem->_update();
    }
    
    return 0;
}

std::string AudioOutput::getApi() const
{
    return _apiName;
}

void AudioOutput::open(id_t id, id_t channels, id_t frames)
{
    RtAudio::StreamParameters params;
    
    params.nChannels = channels;
    
    params.deviceId = id;
    
    _audio.openStream(&params,
                      NULL,
                      RTAUDIO_FLOAT64,
                      Global::samplerate,
                      &frames,
                      &_callback);
    
    _id = id;
    
    _device = getDevice(_id);
}

void AudioOutput::close()
{
    try
    {
        _audio.abortStream();
        _audio.closeStream();
        
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
        _audio.startStream();
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
        _audio.stopStream();
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
        return _audio.getDeviceCount();
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
        return _audio.getDeviceInfo(id);
    }
    
    catch(RtAudioError& error)
    {
        error.printMessage();
    }
    
    return Device();
}

AudioOutput::id_t AudioOutput::getCurrentID() const
{
    return _id;
}

AudioOutput::Device AudioOutput::getCurrentDevice() const
{
    return _device;
}