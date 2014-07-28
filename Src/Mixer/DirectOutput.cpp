//
//  DirectOutput.cpp
//  Synth
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
    
    Sample * data;
    
    err = Pa_OpenDefaultStream(
                                &_stream,
                                0, /* no input */
                                2,
                                paFloat32,
                                Global::samplerate,
                                256,
                                &paCallback,
                                &data
                                );
    
    if( err != paNoError )
        throw std::runtime_error(Pa_GetErrorText( err ));
}

void DirectOutput::processTick(const Sample& sample)
{
    //long x = _dataBuffer.size();
    
    _dataBuffer.push(sample);
}

void DirectOutput::processTick(const double &value)
{
    Sample sample(value);
    
    _dataBuffer.push(sample);
}


Sample DirectOutput::_getSampleFromQueue()
{
    Sample sample;
    
    if (! _dataBuffer.empty())
        sample =  _dataBuffer.getpop();
    
    return sample;
}

void DirectOutput::play()
{
    PaError err = Pa_StartStream( _stream );
    
    if( err != paNoError )
        throw std::runtime_error(Pa_GetErrorText( err ));
}

void DirectOutput::stop()
{
    PaError err = Pa_StopStream( _stream );
    
    if( err != paNoError )
        throw std::runtime_error(Pa_GetErrorText( err ));
}

int DirectOutput::paCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
{
    
    Sample * outSample = ( Sample * ) userData;
    float * out = (float*)outputBuffer;
    
    while (_dataBuffer.size() < framesPerBuffer);
    
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
            throw std::runtime_error(Pa_GetErrorText( err ));
    }
    
    err = Pa_CloseStream( _stream );
    
    if( err != paNoError )
        throw std::runtime_error(Pa_GetErrorText( err ));
    
    err = Pa_Terminate();
    
    if( err != paNoError )
        throw std::runtime_error(Pa_GetErrorText( err ));
}