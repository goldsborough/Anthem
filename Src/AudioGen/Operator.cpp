#include "Operator.h"
#include "Wavetable.h"
#include "ModDock.h"
#include "Util.h"
#include "Global.h"

Operator::Operator(short wt, double frq,
                   double amp, short phaseOffset,
                   double ratio, double freqOffset)
: Oscillator(wt,frq,phaseOffset),
  ratio_(ratio),
  GenUnit(1,amp)
{
    setNoteFrequency(frq);
    
    setFrequencyOffset(freqOffset);
    
    mods_[AMP].setHigherBoundary(1);
    mods_[AMP].setLowerBoundary(0);
    mods_[AMP].setBaseValue(amp);
}

void Operator::setAmp(double amp)
{
    if (amp < 0)
    { throw std::invalid_argument("Amplitude cannot be less than 0!"); }
    
    amp_ = amp;
    
    mods_[AMP].setBaseValue(amp);
}

double Operator::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    return amp_;
}

void Operator::setNoteFrequency(double frequency)
{
    // Check nyquist limit
    if (frequency > Global::nyquistLimit)
    { throw std::invalid_argument("Base frequecy cannot be higher than the nyquist limit!"); }
    
    noteFreq_ = frequency;
    
    note_ = Util::freqToNote(noteFreq_);
    
    freq_ = modBaseFreq_ = noteFreq_ * ratio_;
    
    modBaseNote_ = Util::freqToNote(modBaseFreq_);
    
    indIncr_ = Global::tableIncr * modBaseFreq_;
}

double Operator::getNoteFrequency() const
{
    return noteFreq_;
}

double Operator::getModBaseFrequency() const
{
    // Return the frequency of the note
    return modBaseFreq_;
}

void Operator::modulateFrequency(double value)
{
    double newFreq = modBaseFreq_ + value;
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { throw std::invalid_argument("Modulated frequecy cannot be higher than the nyquist limit!"); }
    
    freq_ = newFreq;
    
    indIncr_ =  Global::tableIncr * freq_;
}

void Operator::setNote(note_t note)
{
    double newFreq = Util::noteToFreq(note);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { throw std::invalid_argument("Note frequecy cannot be higher than the nyquist limit!"); }
    
    noteFreq_ = newFreq;
    
    freq_ = modBaseFreq_ = noteFreq_ * ratio_;
    
    modBaseNote_ = Util::freqToNote(modBaseFreq_);
    
    indIncr_ = Global::tableIncr * modBaseFreq_;
    
    note_ = note;
}

Operator::note_t Operator::getNote() const
{
    return note_;
}

void Operator::setFrequencyOffset(double Hz)
{
    double newFreq = freq_ + Hz;
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { throw std::invalid_argument("Frequeny offset cannot make frequency higher than the nyquist limit!"); }
    
    freqOffset_ = Hz;
    
    indexOffset_ = Global::tableIncr * freqOffset_;
    
    semitoneOffset_ = Util::freqToSemitones(freq_, newFreq);
}

double Operator::getFrequencyOffset() const
{
    return freqOffset_;
}

void Operator::setSemitoneOffset(double semitones)
{
    double newFreq = Util::semitonesToFreq(freq_, semitones);
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { throw std::invalid_argument("Semitone offset cannot make frequency higher than the nyquist limit!"); }
    
    freqOffset_ = newFreq - freq_;
    
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
    
    double newFreq = noteFreq_ * ratio;
    
    // Check nyquist limit
    if (newFreq > Global::nyquistLimit)
    { throw std::invalid_argument("Ratio cannot make frequency higher than the nyquist limit!"); }
    
    ratio_ = ratio;
    
    freq_ = modBaseFreq_ = newFreq;
    
    indIncr_ = Global::tableIncr * freq_;
}

double Operator::getRatio() const
{
    return ratio_;
}

void Operator::increment()
{
    Oscillator::increment();
    
    indIncr_ += indexOffset_;
}

double Operator::tick()
{
    if (mods_[AMP].inUse())
    {
        amp_ = mods_[AMP].tick();
    }
    
    return Oscillator::tick() * amp_;
}