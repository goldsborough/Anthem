/********************************************************************************************//*!
*
*  @file        Operator.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Operator.hpp"
#include "Wavetable.hpp"
#include "ModDock.hpp"
#include "Global.hpp"
#include "Notetable.hpp"
#include "Util.hpp"

Operator::Operator(unsigned short wt,
                   double freqOffset,
                   double level,
                   Mode mode,
                   short phaseOffset,
                   double ratio)

: Oscillator(wt, 0, phaseOffset),
  ratio_(ratio),
  GenUnit(1),
  noteFreq_(0),
  note_(0),
  modOffset_(0),
  semitoneOffset_(0),
  freqOffset_(0),
  realFreq_(0),
  level_(0)
{
    setFrequencyOffset(freqOffset);
    
    // setMode only works if the modes are different
    mode_ = (mode == Mode::FM) ? Mode::ADDITIVE : Mode::FM;
    
    setMode(mode);
    
    mods_[LEVEL].setLowerBoundary(0);
    mods_[LEVEL].setBaseValue(level);
    
    setLevel(level);
}

void Operator::setMode(Mode mode)
{
    if (mode_ == mode) return;
    
    mode_ = mode;
    
    switch(mode)
    {
        case Mode::FM:
        {
            setLevel(level_ * 10);
            
            // Index of modulation, between 0 and 10
            boundary_ = 10;
            
            break;
        }
    
        case Mode::ADDITIVE:
        {
            // More efficient to set level here
            // Factor 10 because of the different
            // ranges depending on the mode (0-1
            // for additive, 0-10 for FM)
            level_ /= 10;
            
            amp_ = level_;
            
            mods_[LEVEL].setBaseValue(level_);
            
            // Like amplitude, between 0 and 1
            boundary_ = 1;
            
            break;
        }
    }
    
    mods_[LEVEL].setHigherBoundary(boundary_);
}

Operator::Mode Operator::getMode() const
{
    return mode_;
}

void Operator::setSilent()
{
    // 0 frequency means no increment and thus silence
    noteFreq_ = freq_ = incr_ = index_ = note_ = modOffset_ = 0;
    
    realFreq_ = freqOffset_;
}

void Operator::setLevel(double level)
{
    if (level > boundary_ || level < -boundary_)
    { throw std::invalid_argument("Level out of range!"); }

    level_ = level;
    
    // For FM Mode, the level is the index of modulation beta,
    // and the amplitude is the beta times the current real
    // frequency, as beta = amplitude/frequency. For Additive
    // Mode, the amplitude is simply the usual range from 0 to 1
    amp_ = (mode_ == Mode::FM) ? level * realFreq_ : level;
    
    mods_[LEVEL].setBaseValue(level);
}

double Operator::getLevel() const
{
    if (mods_[LEVEL].inUse())
    {
        return mods_[LEVEL].getBaseValue();
    }
    
    else return level_;
}

void Operator::modulateFrequency(double value)
{
    modOffset_ = Global::tableIncr * value;
}

void Operator::setNote(note_t note)
{
    if (note > 127)
    { throw std::invalid_argument("Invalid note supplied, must be between 0 and 127!"); }
    
    // Convert note to frequency
    noteFreq_ = notetable[note];
    
    freq_ = noteFreq_ * ratio_;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (mode_ == Mode::FM) amp_ = level_ * realFreq_;
    
    incr_ = Global::tableIncr * freq_;
    
    semitoneOffset_ = Util::freqToSemitones(freq_, realFreq_);
    
    note_ = note;
}

Operator::note_t Operator::getNote() const
{
    return note_;
}

void Operator::setFrequencyOffset(double Hz)
{
    freqOffset_ = Hz;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (mode_ == Mode::FM) amp_ = level_ * realFreq_;
    
    indexOffset_ = Global::tableIncr * freqOffset_;
    
    semitoneOffset_ = Util::freqToSemitones(freq_, realFreq_);
}

double Operator::getFrequencyOffset() const
{
    return freqOffset_;
}

double Operator::getFrequency() const
{
    return realFreq_;
}

void Operator::setSemitoneOffset(double semitones)
{
    freqOffset_ = Util::semitonesToFreq(freq_, semitones);
    
    // More efficient to do things here
    // than to call setFrequencyOffset
    
    realFreq_ = freq_ + freqOffset_;
    
    if (mode_ == Mode::FM) amp_ = level_ * realFreq_;
    
    indexOffset_ = Global::tableIncr * freqOffset_;
    
    semitoneOffset_ = semitones;
}

double Operator::getSemitoneOffset() const
{
    return semitoneOffset_;
}

void Operator::setRatio(double ratio)
{
    if (ratio_ < 0)
    { throw std::invalid_argument("Frequency ratio cannot be less than zero!"); }
    
    ratio_ = ratio;
    
    freq_ = noteFreq_ * ratio;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (mode_ == Mode::FM) amp_ = level_ * realFreq_;
    
    incr_ = Global::tableIncr * freq_;
    
    semitoneOffset_ = Util::freqToSemitones(freq_, realFreq_);
}

double Operator::getRatio() const
{
    return ratio_;
}

void Operator::update()
{
    // Normal frequency index increment     +
    // Index increment for frequency offset +
    // Index increment for frequency modulation value
    increment_(incr_ + indexOffset_ + modOffset_);
}

double Operator::tick()
{
    if (mods_[LEVEL].inUse())
    {
        level_ = mods_[LEVEL].tick();
        
        amp_ = level_;
        
        if (mode_ == Mode::FM) amp_ *= realFreq_;
    }
    
    return Oscillator::tick() * amp_;
}