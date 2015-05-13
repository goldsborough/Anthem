/********************************************************************************************//*!
*
*  @file        Mixer.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Mixer.hpp"
#include "Global.hpp"
#include "Crossfader.hpp"
#include "Sample.hpp"
#include "ModDock.hpp"

Mixer::Mixer(double amp)

: Unit(2),
  masterAmp_(amp), recording_(false),
  pan_(new CrossfadeUnit)

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
  masterAmp_(other.masterAmp_),
  recording_(other.recording_),
  pan_(new CrossfadeUnit(*other.pan_)),
  wavefile_(other.wavefile_)
{ }

Mixer& Mixer::operator= (const Mixer& other)
{
    if (this != &other)
    {
        Unit::operator=(other);
        
        masterAmp_ = other.masterAmp_;
        
        recording_ = other.recording_;
        
        *pan_ = *other.pan_;
        
        wavefile_ = other.wavefile_;
    }
    
    return *this;
}

Sample Mixer::process(Sample sample)
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
    
    // Apply master amplitude
    sample *= masterAmp_;
    
    // Send to wavefile if recording
    if (recording_)
    {
        wavefile_.process(sample);
    }
    
    return sample;
}

void Mixer::setMasterAmp(double amp)
{
    if (amp > 1 || amp < 0)
    {
        throw std::invalid_argument("Amplitude must be between 0 and 1!");
    }
    
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

void Mixer::setPanValue(double pan)
{
    // Set value to pan object and
    // do boundary checking there
    pan_->setValue(pan);

    mods_[PAN].setBaseValue(pan);
}

double Mixer::getPanValue() const
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

bool Mixer::isRecording() const
{
    return recording_;
}

void Mixer::startRecording()
{
    recording_ = true;
}

void Mixer::pauseRecording()
{
    recording_ = false;
}

void Mixer::stopRecording()
{
    recording_ = false;
    
    wavefile_.flush();
}

void Mixer::saveRecording()
{
    recording_ = false;
    
    wavefile_.write();
}