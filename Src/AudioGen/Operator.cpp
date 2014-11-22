#include "Operator.hpp"
#include "Wavetable.hpp"
#include "ModDock.hpp"
#include "Util.hpp"
#include "Global.hpp"


#include <iostream>
#include <iomanip>


Operator::Operator(short wt, double freqOffset,
                   double level, bool mode,
                   short phaseOffset, double ratio)
: Oscillator(wt,0,phaseOffset), ratio_(ratio),
  GenUnit(1), noteFreq_(0), note_(0), modOffset_(0),
  semitoneOffset_(0), freqOffset_(0), realFreq_(0)
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

void Operator::setLevel(double level)
{
    if (level < 0 || (mode_ && level > 1) || (! mode_ && level > 10))
    { throw std::invalid_argument("Level out of range!"); }
    
    level_ = (mode_) ? level : level * realFreq_;
    
    mods_[LEVEL].setBaseValue(level);
}

double Operator::getLevel() const
{
    if (mods_[LEVEL].inUse())
    {
        return mods_[LEVEL].getBaseValue();
    }
    
    return level_ / realFreq_;
}

void Operator::modulateFrequency(double value)
{
    modOffset_ = Global::tableIncr * value;
}

void Operator::setNote(note_t note)
{
    // Convert note to frequency
    noteFreq_ = Util::noteToFreq(note);
    
    freq_ = noteFreq_ * ratio_;
    
    indIncr_ = Global::tableIncr * freq_;
    
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
    
    indexOffset_ = Global::tableIncr * freqOffset_;
    
    semitoneOffset_ = Util::freqToSemitones(freq_, freq_ + Hz);
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
    freqOffset_ = Util::semitonesToFreq(freq_, semitones) - freq_;
    
    realFreq_ = freq_ + freqOffset_;
    
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
    
    indIncr_ = Global::tableIncr * freq_;
}

double Operator::getRatio() const
{
    return ratio_;
}

void Operator::increment()
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