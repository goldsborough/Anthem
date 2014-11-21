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
#include "AudioOutput.h"
#include "Wavefile.h"
#include "Sample.h"
#include "ModDock.h"

Mixer::Mixer(bool audioOut, bool waveOut, double amp)

: Unit(2), audioOutputEnabled_(audioOut),
  wavefileEnabled_(waveOut),
  masterAmp_(amp), stopped_(true),
  pan_(new CrossfadeUnit(CrossfadeTypes::SINE)),
  audioOutput_(new AudioOutput),
  waveOutput_(new Wavefile)

{
    // Initialize ModDocks
    mods_[MASTER_AMP].setHigherBoundary(1);
    mods_[MASTER_AMP].setLowerBoundary(0);
    mods_[MASTER_AMP].setBaseValue(amp);
    
    mods_[PAN].setHigherBoundary(100);
    mods_[PAN].setLowerBoundary(-100);
    mods_[PAN].setBaseValue(0);
}

Mixer::Mixer(const Mixer& other)
: Unit(other),
  audioOutputEnabled_(other.audioOutputEnabled_),
  wavefileEnabled_(other.wavefileEnabled_),
  masterAmp_(other.masterAmp_),
  stopped_(other.stopped_),
  pan_(new CrossfadeUnit(*other.pan_)),
  audioOutput_(new AudioOutput(*other.audioOutput_)),
  waveOutput_(new Wavefile(*other.waveOutput_))
{ }

Mixer& Mixer::operator= (const Mixer& other)
{
    if (this != &other)
    {
        Unit::operator=(other);
        
        audioOutputEnabled_ = other.audioOutputEnabled_;
        
        wavefileEnabled_ = other.wavefileEnabled_;
        
        masterAmp_ = other.masterAmp_;
        
        stopped_ = other.stopped_;
        
        *pan_ = *other.pan_;
        
        *audioOutput_ = *other.audioOutput_;
        
        *waveOutput_ = *other.waveOutput_;
    }
    
    return *this;
}

void Mixer::process(Sample sample)
{
    // Modulate panning value
    if (mods_[PAN].inUse())
    {
        pan_->setValue(mods_[PAN].tick());
    }
    
    if (mods_[MASTER_AMP].inUse())
    {
        masterAmp_ = mods_[MASTER_AMP].tick();
    }
    
    // Attenuate samples with panning
    sample.left *= pan_->left();
    sample.right *= pan_->right();
    
    sample *= masterAmp_;
    
    // Store in wavefile storage buffer
    if (wavefileEnabled_)
    { waveOutput_->process(sample); }
    
    // Send to direct audio output
    if (audioOutputEnabled_)
    { audioOutput_->process(sample); }
}

void Mixer::setMasterAmp(double amp)
{
    if (amp > 1 || amp < 0)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    mods_[MASTER_AMP].setBaseValue(amp);
    
    masterAmp_ = amp;
}

double Mixer::getMasterAmp() const
{
    if (mods_[MASTER_AMP].inUse())
    {
        return mods_[MASTER_AMP].getBaseValue();
    }
    
    else return masterAmp_;
}

void Mixer::setPanValue(short pan)
{
    // Set value to pan object and
    // do boundary checking there
    pan_->setValue(pan);

    mods_[PAN].setBaseValue(pan);
}

short Mixer::getPanValue() const
{
    if (mods_[PAN].inUse())
    {
        return mods_[PAN].getBaseValue();
    }
    
    else return pan_->getValue();
}

void Mixer::setPanType(unsigned short type)
{
    pan_->setType(type);
}

unsigned short Mixer::getPanType() const
{
    return pan_->getType();
}

void Mixer::setWavefileEnabled(bool state)
{
    wavefileEnabled_ = state;
}

bool Mixer::wavefileEnabled() const
{
    return wavefileEnabled_;
}

void Mixer::setAudioOutputEnabled(bool state)
{
    audioOutputEnabled_ = state;
}

bool Mixer::isAudioOutputEnabled() const
{
    return audioOutputEnabled_;
}

void Mixer::start()
{
    if (stopped_)
    {
        if (audioOutputEnabled_)
        { audioOutput_->start(); }
        
        stopped_ = false;
    }
}

void Mixer::stop()
{
    if (! stopped_)
    {
        stopped_ = true;
        
        if (audioOutputEnabled_)
        { audioOutput_->stop(); }
        
        if (wavefileEnabled_)
        { waveOutput_->write(); }
    }
}

Mixer::~Mixer()
{ }