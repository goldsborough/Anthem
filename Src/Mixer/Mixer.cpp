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

: Unit(2), directOutputEnabled_(directOut), wavefileEnabled_(waveOut),
  masterAmp_(amp), stopped_(true)

{
    pan_ = new CrossfadeUnit(CrossfadeTypes::SINE);
    
    sampleDataBuffer_ = new SampleBuffer;
    
    directOut_ = new DirectOutput;
    
    waveOut_ = new Wavefile;
    
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
        pan_->setValue(mods_[PAN]->tick());
    }
    
    // Attenuate samples with panning
    sample.left *= pan_->left();
    sample.right *= pan_->right();
    
    // Modulate amplitude
    double amp = masterAmp_;
    
    if (mods_[AMP]->inUse())
    {
        amp = mods_[AMP]->tick();
    }
    
    sample *= amp;
    
    // Store in wavefile storage buffer
    if (wavefileEnabled_)
    { sampleDataBuffer_->buffer.push_back(sample); }
    
    // Send to direct audio output
    if (directOutputEnabled_)
    { directOut_->processTick(sample); }
}

void Mixer::setMasterAmp(double amp)
{
    if (amp > 1 || amp < 0)
    { throw std::invalid_argument("Amplitude must be between 0 and 1!"); }
    
    mods_[AMP]->setBaseValue(amp);
    
    masterAmp_ = amp;
}

double Mixer::getMasterAmp() const
{
    return masterAmp_;
}

void Mixer::setPanValue(short pan)
{
    // Set value to pan object and
    // do boundary checking there
    pan_->setValue(pan);
    
    mods_[PAN]->setBaseValue(pan);
}

short Mixer::getPanValue() const
{
    return pan_->getValue();
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

void Mixer::setDirectOutputEnabled(bool state)
{
    directOutputEnabled_ = state;
}

bool Mixer::directOutputEnabled() const
{
    return directOutputEnabled_;
}

void Mixer::play()
{
    if (stopped_)
    {
        if (directOutputEnabled_)
        { directOut_->play(); }
        
        stopped_ = false;
    }
}

void Mixer::stop()
{
    if (! stopped_)
    {
        stopped_ = true;
        
        if (directOutputEnabled_)
        { directOut_->stop(); }
        
        if (wavefileEnabled_)
        { waveOut_->write(*sampleDataBuffer_); }
    }
}

Mixer::~Mixer()
{
    stop();
    
    delete pan_;
    delete waveOut_;
    delete directOut_;
    delete sampleDataBuffer_;
}