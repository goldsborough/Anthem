//
//  DirectOutput.cpp
//  Synth
//
//  Created by Peter Goldsborough on 11/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "DirectOutput.h"
#include "Global.h"
#include "SampleData.h"

SampleBuffer DirectOutput::_dataBuffer;

DirectOutput::DirectOutput(unsigned char ltncy)
{
    PaError err = Pa_Initialize();
    
    SampleData * data;
    
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

void DirectOutput::processTick(const SampleData& smplD)
{
    //long x = _dataBuffer.size();
    
    _dataBuffer.push(smplD);
}

void DirectOutput::processTick(const double &value)
{
    SampleData smplD(value);
    
    _dataBuffer.push(smplD);
}


SampleData DirectOutput::_getSampleDataFromQueue()
{
    SampleData smplD;
    
    if (! _dataBuffer.empty())
        smplD =  _dataBuffer.getpop();
    
    return smplD;
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
    
    SampleData * outSampleData = ( SampleData * ) userData;
    float * out = (float*)outputBuffer;
    
    while (_dataBuffer.size() < framesPerBuffer);
    
    for( unsigned int n = 0; n < framesPerBuffer; n++ )
    {
        SampleData smplD = _getSampleDataFromQueue();
        
        outSampleData->left = smplD.left;
        outSampleData->right = smplD.right;
        
        *out++ = outSampleData->left;
        *out++ = outSampleData->right;
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