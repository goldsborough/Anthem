//
//  DirectOutput.h
//  Synth
//
//  Created by Peter Goldsborough on 08/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__DirectOutput__
#define __Synth__DirectOutput__

#include "portaudio.h"

class SampleData;
class SampleQueue;

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
    
    void processTick(const SampleData& smplD);
    
    void processTick(const double& value);
    
    void play();
    
    void stop();
    
private:
    
    PaStream * stream;
    
    static SampleData getSampleDataFromQueue();
    
    static SampleQueue _dataBuffer;
    
};

#endif /* defined(__Synth__DirectOutput__) */
