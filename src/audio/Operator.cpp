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

Operator::Operator(unsigned short wt, double freqOffset,
                   double level, bool mode,
                   short phaseOffset, double ratio)

: Oscillator(wt,0,phaseOffset), ratio_(ratio),
  GenUnit(1), noteFreq_(0), note_(0), modOffset_(0),
  semitoneOffset_(0), freqOffset_(0), realFreq_(0),
  level_(0)

{
    setFrequencyOffset(freqOffset);
    
    setMode(mode);
    
    mods_[LEVEL].setLowerBoundary(0);
    mods_[LEVEL].setBaseValue(level);
    
    setLevel(level);
}

void Operator::setMode(bool mode)
{
    if (mode_ == mode) return;
    
    mode_ = mode;
    
    if (mode)
    {
        // More efficient to set level here
        // Factor 10 because of the different
        // ranges depending on the mode (0-1
        // for additive, 0-10 for FM)
        level_ /= (realFreq_ * 10);
        
        mods_[LEVEL].setBaseValue(level_);
        
        // Like amplitude, between 0 and 1
        mods_[LEVEL].setHigherBoundary(1);
    }
    
    else
    {
        setLevel(level_ * 10);
        
        // Index of modulation, between 0 and 10
        mods_[LEVEL].setHigherBoundary(10);
    }
}

bool Operator::getMode() const
{
    return mode_;
}

void Operator::setSilent()
{
    // 0 frequency means no increment and thus silence
    noteFreq_ = freq_ = indIncr_ = note_ = 0;
}

void Operator::setLevel(double level)
{
    if (level < 0 || (mode_ && level > 1) || (! mode_ && level > 10))
    { throw std::invalid_argument("Level out of range!"); }
    
    level_ = (mode_) ? level : level * realFreq_;
    
    mods_[LEVEL].setBaseValue(level);
}

double Operator::getLevel() const
{
    double ret;
    
    if (mods_[LEVEL].inUse())
    {
        ret = mods_[LEVEL].getBaseValue();
    }
    
    else ret = level_;
    
    return (mode_) ? ret : ret / realFreq_;
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
    
    if (! mode_ && realFreq_) level_ /= realFreq_;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (! mode_) level_ *= realFreq_;
    
    indIncr_ = Global::tableIncr * freq_;
    
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
    
    if (! mode_ && realFreq_) level_ /= realFreq_;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (! mode_) level_ *= realFreq_;
    
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
    
    if (! mode_ && realFreq_) level_ /= realFreq_;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (! mode_) level_ *= realFreq_;
    
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
    
    if (! mode_ && realFreq_) level_ /= realFreq_;
    
    realFreq_ = freq_ + freqOffset_;
    
    if (! mode_) level_ *= realFreq_;
    
    indIncr_ = Global::tableIncr * freq_;
    
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
    increment_(indIncr_ + indexOffset_ + modOffset_);
}

double Operator::tick()
{
    if (mods_[LEVEL].inUse())
    {
        level_ = mods_[LEVEL].tick();
        
        if (! mode_) level_ *= realFreq_;
    }
    
    return Oscillator::tick() * level_;
}