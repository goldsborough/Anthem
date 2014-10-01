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

SampleBuffer DirectOutput::_dataBuffer;

DirectOutput::DirectOutput(unsigned char ltncy)
{
    PaError err = Pa_Initialize();
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    Sample * data;
    
    err = Pa_OpenDefaultStream(
                                &_stream,
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
    _dataBuffer.buffer.push_back(sample);
}

Sample DirectOutput::_getSampleFromQueue()
{
    if (! _dataBuffer.buffer.empty())
    { return _dataBuffer.getpop(); }
    
    return Sample();
}

void DirectOutput::play()
{
    PaError err = Pa_StartStream( _stream );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
}

void DirectOutput::stop()
{
    PaError err = Pa_StopStream( _stream );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    // Clear data buffer
    _dataBuffer.buffer.clear();
}

int DirectOutput::paCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
{
    
    Sample * outSample = ( Sample * ) userData;
    float * out = (float*)outputBuffer;
    
    while (_dataBuffer.buffer.size() < framesPerBuffer);
    
    for( unsigned int n = 0; n < framesPerBuffer; n++ )
    {
        Sample sample = _getSampleFromQueue();
        
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
    
    if (Pa_IsStreamActive (_stream))
    {
        err = Pa_AbortStream(_stream);
        
        if( err != paNoError )
        { throw std::runtime_error(Pa_GetErrorText( err )); }
    }
    
    err = Pa_CloseStream( _stream );
    
    if( err != paNoError )
    { throw std::runtime_error(Pa_GetErrorText( err )); }
    
    err = Pa_Terminate();
}