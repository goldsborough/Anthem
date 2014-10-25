//
//  DirectOutput.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 11/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "DirectOutput.h"
#include "Global.h"
#include "Sample.h"

SampleBuffer DirectOutput::dataBuffer_;

DirectOutput::DirectOutput(unsigned char ltncy)
{
    PaError err = Pa_Initialize();
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    Sample * data;
    
    err = Pa_OpenDefaultStream(
                                &stream_,
                                NULL, // no input
                                2,
                                paFloat32,
                                Global::samplerate,
                                paFramesPerBufferUnspecified,
                                &paCallback,
                                &data
                                );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
}

void DirectOutput::processTick(const Sample& sample)
{
    dataBuffer_.buffer.push_back(sample);
}

Sample DirectOutput::getSampleFromQueue_()
{
    if (! dataBuffer_.buffer.empty())
    { return dataBuffer_.getpop(); }
    
    return Sample();
}

void DirectOutput::play()
{
    PaError err = Pa_StartStream( stream_ );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
}

void DirectOutput::stop()
{
    PaError err = Pa_StopStream( stream_ );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    // Clear data buffer
    dataBuffer_.buffer.clear();
}

int DirectOutput::paCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
{
    
    Sample * outSample = ( Sample * ) userData;
    float * out = (float*)outputBuffer;
    
    while (dataBuffer_.buffer.size() < framesPerBuffer);
    
    for( unsigned int n = 0; n < framesPerBuffer; n++ )
    {
        Sample sample = getSampleFromQueue_();
        
        outSample->left = sample.left;
        outSample->right = sample.right;
        
        *out++ = outSample->left;
        *out++ = outSample->right;
    }
    
    return 0;
}

DirectOutput::~DirectOutput()
{
    PaError err;
    
    if (Pa_IsStreamActive (stream_))
    {
        err = Pa_AbortStream(stream_);
        
        if( err != paNoError )
        { throw std::runtime_error(Pa_GetErrorText( err )); }
    }
    
    err = Pa_CloseStream( stream_ );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    err = Pa_Terminate();
}