/********************************************************************************************//*!
*
*  @file        LFO.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "LFO.hpp"
#include "EnvelopeSegment.hpp"
#include "Crossfader.hpp"
#include "Oscillator.hpp"
#include "ModDock.hpp"
#include "Global.hpp"

#include <stdexcept>

LFO::LFO(short wt, double freq, double amp, double phaseOffset)
: Oscillator(wt,freq,phaseOffset), ModUnit(3,amp)
{
    _mods[FREQ].setHigherBoundary(100);
    _mods[FREQ].setLowerBoundary(0);
    _mods[FREQ].setBaseValue(freq);
    
    _mods[PHASE].setHigherBoundary(360);
    _mods[PHASE].setLowerBoundary(0);
    _mods[PHASE].setBaseValue(phaseOffset);
    
    _mods[AMP].setHigherBoundary(1);
    _mods[AMP].setLowerBoundary(0);
    _mods[AMP].setBaseValue(amp);
}

void LFO::setFrequency(double Hz)
{
    Oscillator::setFrequency(Hz);
    
    _mods[FREQ].setBaseValue(Hz);
}

double LFO::getFrequency() const
{
    if (_mods[FREQ].inUse())
    {
        return _mods[FREQ].getBaseValue();
    }
    
    else return _freq;
}

void LFO::setPhaseOffset(double degrees)
{
    Oscillator::setPhaseOffset(degrees);
    
    _mods[PHASE].setBaseValue(degrees);
}

double LFO::getPhaseOffset() const
{
    if (_mods[PHASE].inUse())
    {
        return _mods[PHASE].getBaseValue();
    }
    
    else return Oscillator::getPhaseOffset();
}

void LFO::setAmp(double amp)
{
    // For boundary checking
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude cannot be greater 1 or less than 0!"); }
    
    _amp = amp;
    
    _mods[AMP].setBaseValue(amp);
}

double LFO::getAmp() const
{
    if (_mods[AMP].inUse())
    {
        return _mods[AMP].getBaseValue();
    }
    
    else return _amp;
}

double LFO::modulate(double sample, double depth, double maximum)
{
    // Modulate rate/frequency
    if (_mods[FREQ].inUse())
    {
        Oscillator::setFrequency(_mods[FREQ].tick());
    }
    
    // Modulate phase offset
    if (_mods[PHASE].inUse())
    {
        Oscillator::setPhaseOffset(_mods[PHASE].tick());
    }
    
    // Get amplitude modulation
    if (_mods[AMP].inUse())
    {
        _amp = _mods[AMP].tick();
    }

    // Actual modulation by LFO
    return sample + (maximum * Oscillator::tick() * depth * _amp);
}

LFOSequence::LFOSequence(unsigned short seqLength, double rate)
: ModEnvelopeSegmentSequence(seqLength,1), _lfos(seqLength)
{
    setLoopStart(0);
    setLoopEnd(seqLength - 1);
    
    setLoopInf();
    
    setRate(rate);
    
    // Init segs
    for (int i = 0; i < seqLength; ++i)
    {
        // Attach lfos to segment
        _segments[i].attachMod(EnvelopeSegment::START_LEVEL, &_lfos[i].lfo);
        _segments[i].attachMod(EnvelopeSegment::END_LEVEL, &_lfos[i].lfo);
        
        setSegmentBothLevels(i, 1);
        
        _setScaledModFreq(i);
    }
    
    _mods[RATE].setHigherBoundary(10);
    _mods[RATE].setLowerBoundary(0);
    _mods[RATE].setBaseValue(rate);
}

double LFOSequence::getScaledModFreqValue(double freq) const
{
    // Since the rate is in cycles per segment
    // and not cycles per second, we get the
    // "period" of the segment and multiply that
    // by the rate, giving the mod wave's frequency.
    
    if (! _segLen) return 0;
    
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    double temp = Global::samplerate / static_cast<double>(_segLen);
    
    // Multiply by wanted frequency
    return freq * temp;
}

void LFOSequence::_setScaledModFreq(segment_t seg)
{
    // Set scaled frequency to frequency of lfo
    _lfos[seg].lfo.setFrequency(getScaledModFreqValue(_lfos[seg].freq));
}

void LFOSequence::_resizeSegmentsFromRate(double rate)
{
    // get the period, divide up into _segNum pieces
    _segLen = (Global::samplerate / rate) / _segments.size();
    
    // Set all segments' lengths
    for (int i = 0; i < _segments.size(); i++)
    {
        _segments[i].setLength(_segLen);
        
        // Scale frequency of mods according to length
        _setScaledModFreq(i);
    }
}

unsigned long LFOSequence::getSegmentLength() const
{
    return _segLen;
}

void LFOSequence::setAmp(double amp)
{
    // For boundary checking
    ModUnit::setAmp(amp);

    _mods[AMP].setBaseValue(amp);
}

double LFOSequence::getAmp() const
{
    if (_mods[AMP].inUse())
    {
        return _mods[AMP].getBaseValue();
    }
    
    else return _amp;
}

void LFOSequence::setRate(double Hz)
{
    if (Hz < 0 || Hz > 10)
    { throw std::invalid_argument("Rate cannot be less than zero or greater 10!"); }
    
    _rate = Hz;
    
    _mods[RATE].setBaseValue(_rate);
    
    _resizeSegmentsFromRate(_rate);
}

double LFOSequence::getRate() const
{
    if (_mods[RATE].inUse())
    {
        return _mods[RATE].getBaseValue();
    }
    
    else return _rate;
}

void LFOSequence::setSegmentRate(segment_t seg, double rate)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    _segments[seg].setRate(rate);
}

void LFOSequence::setModWavetable(segment_t seg, unsigned short wt)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    _lfos[seg].lfo.setWavetable(wt);
}

std::shared_ptr<Wavetable> LFOSequence::getModWavetable(segment_t seg) const
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return _lfos[seg].lfo.getWavetable();
}

void LFOSequence::setModDepth(segment_t seg, double depth)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    _segments[seg].setModUnitDepth(EnvelopeSegment::START_LEVEL, 0, depth);
    _segments[seg].setModUnitDepth(EnvelopeSegment::END_LEVEL, 0, depth);
}

double LFOSequence::getModDepth(segment_t seg) const
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return _segments[seg].getModUnitDepth(EnvelopeSegment::END_LEVEL, 0);
    return _segments[seg].getModUnitDepth(EnvelopeSegment::END_LEVEL, 0);
}

void LFOSequence::setModFreq(segment_t seg, double freq)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    _lfos[seg].freq = freq;
    
    _setScaledModFreq(seg);
}

double LFOSequence::getModFreq(segment_t seg) const
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return _lfos[seg].freq;
}

void LFOSequence::setModPhaseOffset(segment_t seg, double degrees)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    _lfos[seg].lfo.setPhaseOffset(degrees);
}

double LFOSequence::getModPhaseOffset(segment_t seg)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return _lfos[seg].lfo.getPhaseOffset();
}

void LFOSequence::insertSegment(segment_t pos, const EnvelopeSegment& seg)
{
    if (pos > _segments.size())
    { throw std::invalid_argument("Invalid insertion position!"); }
    
    // pushing back a segment is faster and simpler than insertion
    else if (pos == _segments.size() && _segments.size() + 1 < _segments.capacity())
    { addSegment(); }
    
    // Store distances because the iterators will be invalidated
    segment_t start = std::distance(_segments.begin(), _loopStart);
    segment_t end = std::distance(_segments.begin(), _loopEnd);
    
    // Positions that are greater than the insertion point have to
    // increment together with the segment they refer to
    if (end > pos) ++end;
    
    if (start > pos) ++start;
    
    // Note the greater or equal
    if (_currSegmentNum >= pos) ++_currSegmentNum;
    
    _segments.insert(_segments.begin() + pos, seg);
    
    // Set iterators to valid items now
    _loopEnd = _segments.begin() + end;
    _loopStart = _segments.begin() + start;
    
    _currSegment = _segments.begin() + _currSegmentNum;
    
    _resizeSegmentsFromRate(_rate);
    
    _lfos.insert(_lfos.begin() + pos, LFOSequence_LFO());
}

void LFOSequence::insertSegment(segment_t pos)
{
    insertSegment(pos, EnvelopeSegment());
}

void LFOSequence::addSegment()
{
    // If there will be a re-allocation we need to update
    // iterators whicc is done in insertSegment()
    if (_segments.size() + 1 == _segments.capacity())
    { insertSegment(_segments.size() - 1); }
    
    else
    {
        _segments.push_back(EnvelopeSegment());
        
        _resizeSegmentsFromRate(_rate);
        
        _lfos.push_back(LFOSequence_LFO());
    }
}

void LFOSequence::removeLastSegment()
{
    removeSegment(_segments.size() - 1);
}

void LFOSequence::removeSegment(segment_t seg)
{
    if (seg >= _segments.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    // Store distances because the iterators will be invalidated
    segment_t start = std::distance(_segments.begin(), _loopStart);
    segment_t end = std::distance(_segments.begin(), _loopEnd);
    
    if (end > seg) --end;
    
    if (start > seg) --start;
    
    if (_currSegmentNum > seg) --_currSegmentNum;
    
    _segments.erase(_segments.begin() + seg);
    
    // Set iterators to valid items now
    _loopEnd = _segments.begin() + end;
    _loopStart = _segments.begin() + start;
    
    _currSegment = _segments.begin() + _currSegmentNum;
    
    if (seg == _currSegmentNum)
    {
        // Change segment to new current one
        _changeSegment(_currSegment);
    }
    
    _resizeSegmentsFromRate(_rate);
    
    _lfos.erase(_lfos.begin() + seg);
}

void LFOSequence::setModUnitDepth_Segment(segment_t segNum,
                                index_t dockNum,
                                index_t modNum,
                                double depth)
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // The depth ModDock is not part of the LFO but of the segment
        // itself because the depth is the depth of the LFO in the
        // EnvelopeSegment's Moddock. The depth is modulated by having two
        // other ModUnits sidechain the internal LFO, so the LFO's
        // depth ModDock is actually just an extension of the EnvelopeSegment's
        // ModDock. modNum + 1 because the LFO is at index 0
        _segments[segNum].setModUnitDepth(EnvelopeSegment::END_LEVEL, modNum + 1, depth);
    }
    
    // All other parameters belong to the lfo (phase and freq)
    else _lfos[segNum].lfo.setModUnitDepth(dockNum,modNum, depth);
}

double LFOSequence::getModUnitDepth_Segment(segment_t segNum, index_t dockNum, index_t modNum) const
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        
        return _segments[segNum].getModUnitDepth(EnvelopeSegment::END_LEVEL, modNum + 1);
    }
    
    // All other parameters belong to the lfo (phase + freq)
    else return _lfos[segNum].lfo.getModUnitDepth(dockNum, modNum);
}

void LFOSequence::attachMod_Segment(segment_t segNum,
                           index_t dockNum,
                           ModUnit *mod)
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Attach the depth ModUnit to the EnvelopeSegment's dock
        // because they sidechain the internal LFO's depth
        _segments[segNum].attachMod(EnvelopeSegment::END_LEVEL, mod);
        
        // Make the latest addition side chain the internal LFO's depth
        _segments[segNum].setSidechain(EnvelopeSegment::END_LEVEL, _segments[segNum].dockSize(EnvelopeSegment::END_LEVEL) - 1, 0);
    }
    
    else _lfos[segNum].lfo.attachMod(dockNum, mod);
}

void LFOSequence::detachMod_Segment(segment_t segNum,
                           index_t dockNum,
                           index_t modNum)
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        _segments[segNum].detachMod(EnvelopeSegment::END_LEVEL, modNum);
    }
    
    else _lfos[segNum].lfo.detachMod(dockNum, modNum);
}

void LFOSequence::setSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master from the internal LFO
        _segments[segNum].unSidechain(EnvelopeSegment::END_LEVEL, master + 1, 0);
        
        // Make it sidechain another ModUnit. + 1 because internal LFO is at
        // index 0
        _segments[segNum].setSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
    }
    
    else _lfos[segNum].lfo.setSidechain(dockNum, master, slave);
}

void LFOSequence::unSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master and slave
        _segments[segNum].unSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
        
        // Make the master sidechain the internal LFO again
        _segments[segNum].setSidechain(EnvelopeSegment::END_LEVEL, master + 1, 0);
    }
    
    else _lfos[segNum].lfo.unSidechain(dockNum, master, slave);
}

bool LFOSequence::isSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // + 1 because internal LFO is at index 0
        return _segments[segNum].isSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
    }
    
    else return _lfos[segNum].lfo.isSidechain(dockNum, master, slave);
}

bool LFOSequence::isMaster_Segment(segment_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return _segments[segNum].isMaster(EnvelopeSegment::END_LEVEL, index + 1);
    }
    
    else return _lfos[segNum].lfo.isMaster(dockNum, index);
}

bool LFOSequence::isSlave_Segment(segment_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return _segments[segNum].isSlave(EnvelopeSegment::END_LEVEL, index + 1);
    }
    
    else return _lfos[segNum].lfo.isSlave(dockNum, index);
}

unsigned long LFOSequence::dockSize_Segment(segment_t segNum, index_t dockNum) const
{
    if (segNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // - 1 because internal LFO is part of it
        return _segments[segNum].dockSize(EnvelopeSegment::END_LEVEL) - 1;
    }
    
    else return _lfos[segNum].lfo.dockSize(dockNum);
}

void LFOSequence::update()
{
    ModEnvelopeSegmentSequence::update();
    
    for (std::vector<LFOSequence_LFO>::iterator itr = _lfos.begin(), end = _lfos.end();
         itr != end;
         ++itr)
    {
        itr->lfo.update();
    }
}

double LFOSequence::modulate(double sample, double depth, double)
{
    if (_mods[RATE].inUse())
    {
        _rate = _mods[RATE].tick();
        
        _resizeSegmentsFromRate(_rate);
    }
    
    if (_mods[AMP].inUse())
    {
        _amp =  _mods[AMP].tick();
    }
    
    return sample * tick() * depth * _amp;
}

LFOUnit::LFOUnit(Mode mode)
: ModUnit(1), _fader(new Crossfader)
{
    _mods[AMP].setHigherBoundary(1);
    _mods[AMP].setLowerBoundary(0);
    _mods[AMP].setBaseValue(1);
    
    setMode(mode);
}

LFOUnit::LFOUnit(const LFOUnit& other)
: ModUnit(other),
  _fader(new Crossfader(*other._fader))
{
    for (unsigned short i = 0; i < 2; ++i)
    {
        _lfoSequences[i] = other._lfoSequences[i];
        _lfos[i] = other._lfos[i];
    }
    
    setMode(other._mode);
}

LFOUnit& LFOUnit::operator= (const LFOUnit& other)
{
    if (this != &other)
    {
        ModUnit::operator=(other);
        
        *_fader = *other._fader;
        
        for (unsigned short i = 0; i < 2; ++i)
        {
            _lfoSequences[i] = other._lfoSequences[i];
            _lfos[i] = other._lfos[i];
        }
        
        setMode(other._mode);
    }
    
    return *this;
}

void LFOUnit::setAmp(double amp)
{
    // For boundary checking
    ModUnit::setAmp(amp);
    
    if (_mods[AMP].inUse())
    {
        _mods[AMP].setBaseValue(amp);
    }
}

double LFOUnit::getAmp() const
{
    if (_mods[AMP].inUse())
    {
        return _mods[AMP].getBaseValue();
    }
    
    else return _amp;
}

LFOSequence& LFOUnit::seqs(bool unit)
{
    return _lfoSequences[unit];
}

LFO& LFOUnit::lfos(bool unit)
{
    return _lfos[unit];
}

Crossfader& LFOUnit::fader()
{
    return *_fader;
}

void LFOUnit::setMode(Mode mode)
{
    _mode = mode;
    
    switch(mode)
    {
        case Mode::SEQ:
        {
            _fader->setLeftUnit(&_lfoSequences[0]);
            
            _fader->setRightUnit(&_lfoSequences[1]);
            
            break;
        }
    
        case Mode::LFO:
        {
            _fader->setLeftUnit(&_lfos[0]);
            
            _fader->setRightUnit(&_lfos[1]);
            
            break;
        }
    }
}

LFOUnit::Mode LFOUnit::getMode() const
{
    return _mode;
}

void LFOUnit::update()
{
    switch(_mode)
    {
        case Mode::SEQ:
        {
            _lfoSequences[0].update();
            _lfoSequences[1].update();
            
            break;
        }
            
        case Mode::LFO:
        {
            _lfos[0].update();
            _lfos[1].update();
            
            break;
        }
    }
    
}

double LFOUnit::modulate(double sample, double depth, double maximum)
{
    if (_mods[AMP].inUse())
    {
        _amp =  _mods[AMP].tick();
    }
    
    if (! _active) return sample;
    
    // Tick the crossfaded value from the lfos and multiply by the envelope
    // value and the total amplitude value
    return _fader->modulate(sample, depth, maximum) * _amp;
}
