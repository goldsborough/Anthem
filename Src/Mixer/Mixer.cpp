//
//  Mixer.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 28/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Mixer.h"
#include "Global.h"
#include "Crossfader.h"
#include "DirectOutput.h"
#include "Wavefile.h"
#include "Sample.h"

#include <iostream>

Mixer::Mixer(bool directOut, bool waveOut)

: _sendToDirectOutput(directOut), _sendToWaveFile(waveOut), _masterAmp(0.5)
{
    _pan = new CrossfadeUnit(CrossfadeTypes::SINE);
    
    _sampleDataBuffer = new SampleBuffer;
    
    _directOut = new DirectOutput;
    
    _waveOut = new Wavefile;
}

void Mixer::process(Sample sample)
{
    sample.left *= _pan->left();
    sample.right *= _pan->right();
    
    sample *= _masterAmp;
    
#ifdef AMP_DEBUG
    
    if (sample.left > 1 || sample.right < -1
        || sample.right > 1 || sample.right < -1)
    {
        std::cerr << "WARNING: Amplitude overflow!" << std::endl;
    }
    
#endif
    
    if (_sendToWaveFile)
    { _sampleDataBuffer->buffer.push_back(sample); }
    
    if (_sendToDirectOutput)
    { _directOut->processTick(sample); }
}

void Mixer::setPanning(const char pan)
{
    _pan->setValue(pan);
}

void Mixer::play()
{
    if (_stopped)
    {
        if (_sendToDirectOutput)
        { _directOut->play(); }
        
        _stopped = false;
    }
};

void Mixer::stop()
{
    if (! _stopped)
    {
        _stopped = true;
        
        if (_sendToDirectOutput)
        { _directOut->stop(); }
        
        if (_sendToWaveFile)
        { _waveOut->writeWav(*_sampleDataBuffer); }
    }
}

Mixer::~Mixer()
{
    stop();
    
    delete _pan;
    delete _waveOut;
    delete _directOut;
    delete _sampleDataBuffer;
}