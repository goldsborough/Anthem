/*********************************************************************************************//*!
*
*  @file        AudioOutput.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Holds the DirectOutput class for real-time audio output.
*
*************************************************************************************************/

#ifndef __Anthem__AudioOutput__
#define __Anthem__AudioOutput__

#include <RtAudio.h>

#include <deque>
#include <memory>


class Operator;



class Sample;

class AudioOutput
{
    
public:
    
    typedef unsigned int id_t;
    
    typedef RtAudio::DeviceInfo Device;
    
    AudioOutput();
    
    static void process(const Sample& sample);
    
    void open(id_t id, id_t channels = 2, id_t frames = 256);
    
    void close();
    
    void start();
    
    void stop();
    
    bool isStreaming();
    
    bool isOpen();
    
    id_t getCurrentID() const;
    
    Device getCurrentDevice() const;
    
    id_t getNumberOfDevices();
    
    Device getDevice(id_t id);
    
    std::string getApi() const;
    
    static Operator* osc;
    
private:
    
    static int callback_(void* output,
                         void* input,
                         unsigned int numberOfFrames,
                         double streamTime,
                         RtAudioStreamStatus status,
                         void* userData);
    
    Sample getSample_();
    
    std::string getApiName(const RtAudio::Api& api);
    
    static std::deque<std::unique_ptr<Sample>> buffer_;
    
    Device device_;
    
    id_t id_;
    
    RtAudio::Api api_;
    
    std::string apiName_;
    
    RtAudio audio_;
};

#endif /* defined(__Anthem__AudioOutput__) */
