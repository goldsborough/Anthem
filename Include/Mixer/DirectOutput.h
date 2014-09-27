/*********************************************************************************************//*!
*
*  @file        DirectOutput.h
*
*  @author      Peter Goldsborough
*
*  @date        27/09/2014
*
*  @brief       Holds the DirectOutput class for real-time audio output.
*
*************************************************************************************************/

#ifndef __Anthem__DirectOutput__
#define __Anthem__DirectOutput__

#include <portaudio.h>

class Sample;
class SampleBuffer;

class DirectOutput
{
    
public:
    
    DirectOutput(unsigned char ltncy = 20);
    
    ~DirectOutput();
    
    static int paCallback(const void *input,
                          void *output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData ) ;
    
    void processTick(const Sample& sample);
    
    void play();
    
    void stop();
    
private:
    
    PaStream * _stream;
    
    static Sample _getSampleFromQueue();
    
    static SampleBuffer _dataBuffer;
    
};

#endif /* defined(__Anthem__DirectOutput__) */
