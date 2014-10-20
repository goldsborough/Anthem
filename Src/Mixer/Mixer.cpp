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
#include "ModDock.h"

Mixer::Mixer(bool directOut, bool waveOut, double amp)

: Unit(2), _directOutputEnabled(directOut), _wavefileEnabled(waveOut),
  _masterAmp(amp), _stopped(true)

{
    _pan = new CrossfadeUnit(CrossfadeTypes::SINE);
    
    _sampleDataBuffer = new SampleBuffer;
    
    _directOut = new DirectOutput;
    
    _waveOut = new Wavefile;
    
    // Initialize ModDocks
    mods_[AMP]->setHigherBoundary(1);
    mods_[AMP]->setLowerBoundary(0);
    mods_[AMP]->setBaseValue(amp);
    
    mods_[PAN]->setHigherBoundary(100);
    mods_[PAN]->setLowerBoundary(-100);
    mods_[PAN]->setBaseValue(0);
}

void Mixer::process(Sample sample)
{
    // Modulate panning value
    if (mods_[PAN]->inUse())
    {
        _pan->setValue(mods_[PAN]->tick());
    }
    
    // Attenuate samples with panning
    sample.left *= _pan->left();
    sample.right *= _pan->right();
    
    // Modulate amplitude
    double amp = _masterAmp;
    
    if (mods_[AMP]->inUse())
    {
        amp = mods_[AMP]->tick();
    }
    
    sample *= amp;
    
    // Store in wavefile storage buffer
    if (_wavefileEnabled)
    { _sampleDataBuffer->buffer.push_back(sample); }
    
    // Send to direct audio output
    if (_directOutputEnabled)
    { _directOut->processTick(sample); }
}

void Mixer::setMasterAmp(double amp)
{
    if (amp > 1 || amp < 0)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    mods_[AMP]->setBaseValue(amp);
    
    _masterAmp = amp;
}

double Mixer::getMasterAmp() const
{
    return _masterAmp;
}

void Mixer::setPanValue(short pan)
{
    // Set value to pan object and
    // do boundary checking there
    _pan->setValue(pan);
    
    mods_[PAN]->setBaseValue(pan);
}

short Mixer::getPanValue() const
{
    return _pan->getValue();
}

void Mixer::setPanType(unsigned short type)
{
    _pan->setType(type);
}

unsigned short Mixer::getPanType() const
{
    return _pan->getType();
}

void Mixer::setWavefileEnabled(bool state)
{
    _wavefileEnabled = state;
}

bool Mixer::wavefileEnabled() const
{
    return _wavefileEnabled;
}

void Mixer::setDirectOutputEnabled(bool state)
{
    _directOutputEnabled = state;
}

bool Mixer::directOutputEnabled() const
{
    return _directOutputEnabled;
}

void Mixer::play()
{
    if (_stopped)
    {
        if (_directOutputEnabled)
        { _directOut->play(); }
        
        _stopped = false;
    }
}

void Mixer::stop()
{
    if (! _stopped)
    {
        _stopped = true;
        
        if (_directOutputEnabled)
        { _directOut->stop(); }
        
        if (_wavefileEnabled)
        { _waveOut->write(*_sampleDataBuffer); }
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