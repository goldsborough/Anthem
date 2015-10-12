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
  _ratio(ratio),
  GenUnit(1),
  _noteFreq(0),
  _note(0),
  _modOffset(0),
  _semitoneOffset(0),
  _freqOffset(0),
  _realFreq(0),
  _level(0)
{
    setFrequencyOffset(freqOffset);
    
    // setMode only works if the modes are different
    _mode = (mode == Mode::FM) ? Mode::ADDITIVE : Mode::FM;
    
    setMode(mode);
    
    _mods[LEVEL].setLowerBoundary(0);
    _mods[LEVEL].setBaseValue(level);
    
    setLevel(level);
}

void Operator::setMode(Mode mode)
{
    if (_mode == mode) return;
    
    _mode = mode;
    
    switch(mode)
    {
        case Mode::FM:
        {
            setLevel(_level * 10);
            
            // Index of modulation, between 0 and 10
            _boundary = 10;
            
            break;
        }
    
        case Mode::ADDITIVE:
        {
            // More efficient to set level here
            // Factor 10 because of the different
            // ranges depending on the mode (0-1
            // for additive, 0-10 for FM)
            _level /= 10;
            
            _amp = _level;
            
            _mods[LEVEL].setBaseValue(_level);
            
            // Like amplitude, between 0 and 1
            _boundary = 1;
            
            break;
        }
    }
    
    _mods[LEVEL].setHigherBoundary(_boundary);
}

Operator::Mode Operator::getMode() const
{
    return _mode;
}

void Operator::setSilent()
{
    // 0 frequency means no increment and thus silence
    _noteFreq = _freq = _incr = _index = _note = _modOffset = 0;
    
    _realFreq = _freqOffset;
}

void Operator::setLevel(double level)
{
    if (level > _boundary || level < -_boundary)
    { throw std::invalid_argument("Level out of range!"); }

    _level = level;
    
    // For FM Mode, the level is the index of modulation beta,
    // and the amplitude is the beta times the current real
    // frequency, as beta = amplitude/frequency. For Additive
    // Mode, the amplitude is simply the usual range from 0 to 1
    _amp = (_mode == Mode::FM) ? level * _realFreq : level;
    
    _mods[LEVEL].setBaseValue(level);
}

double Operator::getLevel() const
{
    if (_mods[LEVEL].inUse())
    {
        return _mods[LEVEL].getBaseValue();
    }
    
    else return _level;
}

void Operator::modulateFrequency(double value)
{
    _modOffset = Global::tableIncrement * value;
}

void Operator::setNote(note_t note)
{
    if (note > 127)
    { throw std::invalid_argument("Invalid note supplied, must be between 0 and 127!"); }
    
    // Convert note to frequency
    _noteFreq = notetable[note];
    
    _freq = _noteFreq * _ratio;
    
    _realFreq = _freq + _freqOffset;
    
    if (_mode == Mode::FM) _amp = _level * _realFreq;
    
    _incr = Global::tableIncrement * _freq;
    
    _semitoneOffset = Util::freqToSemitones(_freq, _realFreq);
    
    _note = note;
}

Operator::note_t Operator::getNote() const
{
    return _note;
}

void Operator::setFrequencyOffset(double Hz)
{
    _freqOffset = Hz;
    
    _realFreq = _freq + _freqOffset;
    
    if (_mode == Mode::FM) _amp = _level * _realFreq;
    
    _indexOffset = Global::tableIncrement * _freqOffset;
    
    _semitoneOffset = Util::freqToSemitones(_freq, _realFreq);
}

double Operator::getFrequencyOffset() const
{
    return _freqOffset;
}

double Operator::getFrequency() const
{
    return _realFreq;
}

void Operator::setSemitoneOffset(double semitones)
{
    _freqOffset = Util::semitonesToFreq(_freq, semitones);
    
    // More efficient to do things here
    // than to call setFrequencyOffset
    
    _realFreq = _freq + _freqOffset;
    
    if (_mode == Mode::FM) _amp = _level * _realFreq;
    
    _indexOffset = Global::tableIncrement * _freqOffset;
    
    _semitoneOffset = semitones;
}

double Operator::getSemitoneOffset() const
{
    return _semitoneOffset;
}

void Operator::setRatio(double ratio)
{
    if (_ratio < 0)
    { throw std::invalid_argument("Frequency ratio cannot be less than zero!"); }
    
    _ratio = ratio;
    
    _freq = _noteFreq * ratio;
    
    _realFreq = _freq + _freqOffset;
    
    if (_mode == Mode::FM) _amp = _level * _realFreq;
    
    _incr = Global::tableIncrement * _freq;
    
    _semitoneOffset = Util::freqToSemitones(_freq, _realFreq);
}

double Operator::getRatio() const
{
    return _ratio;
}

void Operator::update()
{
    // Normal frequency index increment     +
    // Index increment for frequency offset +
    // Index increment for frequency modulation value
    _increment(_incr + _indexOffset + _modOffset);
}

double Operator::tick()
{
    if (_mods[LEVEL].inUse())
    {
        _level = _mods[LEVEL].tick();
        
        _amp = _level;
        
        if (_mode == Mode::FM) _amp *= _realFreq;
    }
    
    return Oscillator::tick() * _amp;
}