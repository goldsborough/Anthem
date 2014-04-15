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

Mixer::Mixer(bool directOut,
             bool waveOut,
             const Priority& priort)

            : _sendToDirectOutput(directOut),
            _priority(priort),
            _sendToWaveFile(waveOut),
            _masterAmp(1)

            {
                _pan = new XFadeSine;
                
                _pan->init();
                
                _sampleDataBuffer = new SampleQueue;
                
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
    
    if (_sendToWaveFile)
        _sampleDataBuffer->push(smplD);
    
    if (_sendToDirectOutput)
        _directOut->processTick(smplD);
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