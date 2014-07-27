//
//  Mixer.cpp
//  Synth
//
//  Created by Peter Goldsborough on 28/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Mixer.h"
#include "Global.h"
#include "Crossfader.h"
#include "DirectOutput.h"
#include "Wavefile.h"
#include "SampleData.h"

#include <iostream>

Mixer::Mixer(bool directOut, bool waveOut)

: _sendToDirectOutput(directOut), _sendToWaveFile(waveOut), _masterAmp(0.5)
{
    _pan = new XFadeSine;
    
    _sampleDataBuffer = new SampleBuffer;
    
    _directOut = new DirectOutput;
    
    _waveOut = new Wavefile;
}

void Mixer::processTick(const double smpl)
{
    _sample = smpl;
    
    SampleData smplD(smpl,smpl);
    
    smplD.left *= _pan->left();
    smplD.right *= _pan->right();
    
    smplD *= _masterAmp;
    
#ifdef VIBE_DEBUG
    
    if (smplD.left > 1 || smplD.right < -1
        || smplD.right > 1 || smplD.right < -1)
    {
        std::cerr << "WARNING: Amplitude overflow!" << std::endl;
    }
    
#endif
    
    if (_sendToWaveFile)
    { _sampleDataBuffer->push(smplD); }
    
    if (_sendToDirectOutput)
    { _directOut->processTick(smplD); }
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
            _directOut->play();
        
        _stopped = false;
    }
};

void Mixer::stop()
{
    if (! _stopped)
    {
        _stopped = true;
        
        if (_sendToDirectOutput)
            _directOut->stop();
        
        if (_sendToWaveFile)
            _waveOut->write_wav(*_sampleDataBuffer);
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