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
  _masterAmp(amp), _recording(false),
  _pan(new CrossfadeUnit)

{
    // Initialize ModDocks
    _mods[MASTER_AMP].setHigherBoundary(1);
    _mods[MASTER_AMP].setLowerBoundary(0);
    _mods[MASTER_AMP].setBaseValue(amp);
    
    _mods[PAN].setHigherBoundary(100);
    _mods[PAN].setLowerBoundary(-100);
    _mods[PAN].setBaseValue(0);
}

Mixer::Mixer(const Mixer& other)
: Unit(other),
  _masterAmp(other._masterAmp),
  _recording(other._recording),
  _pan(new CrossfadeUnit(*other._pan)),
  _wavefile(other._wavefile)
{ }

Mixer& Mixer::operator= (const Mixer& other)
{
    if (this != &other)
    {
        Unit::operator=(other);
        
        _masterAmp = other._masterAmp;
        
        _recording = other._recording;
        
        *_pan = *other._pan;
        
        _wavefile = other._wavefile;
    }
    
    return *this;
}

Sample Mixer::process(Sample sample)
{
    // Modulate panning value
    if (_mods[PAN].inUse())
    {
        _pan->setValue(_mods[PAN].tick());
    }
    
    if (_mods[MASTER_AMP].inUse())
    {
        _masterAmp = _mods[MASTER_AMP].tick();
    }
    
    // Attenuate samples with panning
    sample.left *= _pan->left();
    sample.right *= _pan->right();
    
    // Apply master amplitude
    sample *= _masterAmp;
    
    // Send to wavefile if recording
    if (_recording)
    {
        _wavefile.process(sample);
    }
    
    return sample;
}

void Mixer::setMasterAmp(double amp)
{
    if (amp > 1 || amp < 0)
    {
        throw std::invalid_argument("Amplitude must be between 0 and 1!");
    }
    
    _mods[MASTER_AMP].setBaseValue(amp);
    
    _masterAmp = amp;
}

double Mixer::getMasterAmp() const
{
    if (_mods[MASTER_AMP].inUse())
    {
        return _mods[MASTER_AMP].getBaseValue();
    }
    
    else return _masterAmp;
}

void Mixer::setPanValue(double pan)
{
    // Set value to pan object and
    // do boundary checking there
    _pan->setValue(pan);

    _mods[PAN].setBaseValue(pan);
}

double Mixer::getPanValue() const
{
    if (_mods[PAN].inUse())
    {
        return _mods[PAN].getBaseValue();
    }
    
    else return _pan->getValue();
}

void Mixer::setPanType(unsigned short type)
{
    _pan->setType(type);
}

unsigned short Mixer::getPanType() const
{
    return _pan->getType();
}

bool Mixer::isRecording() const
{
    return _recording;
}

void Mixer::startRecording()
{
    _recording = true;
}

void Mixer::pauseRecording()
{
    _recording = false;
}

void Mixer::stopRecording()
{
    _recording = false;
    
    _wavefile.flush();
}

void Mixer::saveRecording()
{
    _recording = false;
    
    _wavefile.write();
}