/********************************************************************************************//*!
*
*  @file        EnvelopeSegment.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "EnvelopeSegment.hpp"
#include "Global.hpp"
#include "Wavetable.hpp"
#include "ModDock.hpp"

#include <cmath>

EnvelopeSegment::EnvelopeSegment(double startLevel,
               double endLevel,
               length_t len,
               double rate)

: _startLevel(startLevel),
  _endLevel(endLevel),
  _rate(rate),
  _curr(0),
  _len(len),
  GenUnit(3) // three ModDocks

{
    _mods[RATE].setHigherBoundary(10);
    _mods[RATE].setLowerBoundary(0);
    _mods[RATE].setBaseValue(rate);
    
    _mods[START_LEVEL].setHigherBoundary(1);
    _mods[START_LEVEL].setLowerBoundary(0);
    _mods[START_LEVEL].setBaseValue(startLevel);
    
    _mods[END_LEVEL].setHigherBoundary(1);
    _mods[END_LEVEL].setLowerBoundary(0);
    _mods[END_LEVEL].setBaseValue(endLevel);
    
    _calculateRange();
    _calculateIncr();
}

void EnvelopeSegment::reset()
{
    _curr = 0;
}

void EnvelopeSegment::_calculateRange()
{
    // The range between start and end
    _range = _endLevel - _startLevel;
}

void EnvelopeSegment::_calculateIncr()
{
    _incr = (_len) ? 1.0/_len : 0;
}

void EnvelopeSegment::update()
{
    // Increment _curr
    _curr += _incr;
}

double EnvelopeSegment::tick()
{
    // If the segment is still supposed to
    // tick after reaching the end amplitude
    // just return the end amplitude
    
    if (_curr >= 1 || ! _len) return _endLevel;
    
    // Modulate members
    // All together so we only call _calculateRange once
    if (_mods[RATE].inUse()        ||
        _mods[START_LEVEL].inUse() ||
        _mods[END_LEVEL].inUse())
    {
        if (_mods[RATE].inUse())
        {
            _rate = _mods[RATE].tick();
        }
        
        if (_mods[START_LEVEL].inUse())
        {
            _startLevel = _mods[START_LEVEL].tick();
        }
        
        if (_mods[END_LEVEL].inUse())
        {
            _endLevel = _mods[END_LEVEL].tick();
        }
        
        _calculateRange();
    }
    
    return _range * std::pow(_curr, _rate) + _startLevel;
}

void EnvelopeSegment::setLength(EnvelopeSegment::length_t sampleLength)
{
    _len = sampleLength;
    
    _calculateIncr();
}

EnvelopeSegment::length_t EnvelopeSegment::getLength() const
{
    return _len;
}

void EnvelopeSegment::setStartLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    _startLevel = lv;
    
    _mods[START_LEVEL].setBaseValue(_startLevel);
    
    _calculateRange();
}

double EnvelopeSegment::getStartLevel() const
{
    if (_mods[START_LEVEL].inUse())
    {
        return _mods[START_LEVEL].getBaseValue();
    }
    
    else return _startLevel;
}

void EnvelopeSegment::setEndLevel(double lv)
{
    if (lv > 1 || lv < 0)
    { throw std::invalid_argument("Level must be between 0 and 1"); }
    
    _endLevel = lv;
    
    _mods[END_LEVEL].setBaseValue(_endLevel);
    
    _calculateRange();
}

double EnvelopeSegment::getEndLevel() const
{
    if (_mods[END_LEVEL].inUse())
    {
        return _mods[END_LEVEL].getBaseValue();
    }
    
    else return _endLevel;
}

void EnvelopeSegment::setBothLevels(double lv)
{
    setStartLevel(lv);
    setEndLevel(lv);
}

void EnvelopeSegment::setRate(double rate)
{
    if (rate > 10 || rate < 0)
    { throw std::invalid_argument("Rate must be between 0 and 10"); }
    
    _rate = rate;
    
    _mods[RATE].setBaseValue(_rate);
}

double EnvelopeSegment::getRate() const
{
    if (_mods[RATE].inUse())
    {
        return _mods[RATE].getBaseValue();
    }
    
    else return _rate;
}

EnvelopeSegmentSequence::EnvelopeSegmentSequence(EnvelopeSegmentSequence::segment_t seqLength)
: _segments(seqLength), _currSample(0), _loopCount(0),
  _loopMax(0), _loopInf(false), _currSegmentNum(0)
{
    _loopStart = _loopEnd = _segments.end();
    
    _currSegment = _segments.begin();
}

EnvelopeSegmentSequence::EnvelopeSegmentSequence(const EnvelopeSegmentSequence& other)
: _segments(other._segments), _currSample(other._currSample),
  _currSegmentNum(other._currSegmentNum), _loopCount(other._loopCount),
  _loopMax(other._loopMax), _loopInf(other._loopInf)
{
    _currSegment = _segments.begin() + _currSegmentNum;
    
    std::vector<EnvelopeSegment>::const_iterator itr = other._loopStart;
    
    _loopStart = _segments.begin() + std::distance(other._segments.begin(), itr);
    
    itr = other._loopEnd;
    
    _loopEnd = _segments.begin() + std::distance(other._segments.begin(), itr);
}

EnvelopeSegmentSequence& EnvelopeSegmentSequence::operator= (const EnvelopeSegmentSequence& other)
{
    if (this != &other)
    {
        _segments = other._segments;
        
        _currSample = other._currSample;
        
        _currSegmentNum = other._currSegmentNum;
        
        _loopCount = other._loopCount;
        
        _loopMax = other._loopMax;
        
        _loopInf = other._loopInf;
        
        _currSegment = _segments.begin() + _currSegmentNum;
        
        std::vector<EnvelopeSegment>::const_iterator itr = other._loopStart;
        
        _loopStart = _segments.begin() + std::distance(other._segments.begin(), itr);
        
        itr = other._loopEnd;
        
        _loopEnd = _segments.begin() + std::distance(other._segments.begin(), itr);
    }
    
    return *this;
}

void EnvelopeSegmentSequence::reset()
{
    for(segmentItr itr = _segments.begin(), end = _segments.end();
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    _changeSegment(_segments.begin());
}

void EnvelopeSegmentSequence::setSegmentRate(segment_t segment, double rate)
{
    _segments[segment].setRate(rate);
}

double EnvelopeSegmentSequence::getSegmentRate(segment_t segment) const
{
    return _segments[segment].getRate();
}


void EnvelopeSegmentSequence::setSegmentStartLevel(segment_t segment, double lv)
{
    _segments[segment].setStartLevel(lv);
}

double EnvelopeSegmentSequence::getSegmentStartLevel(segment_t segment) const
{
    return _segments[segment].getStartLevel();
}


void EnvelopeSegmentSequence::setSegmentEndLevel(segment_t segment, double lv)
{
    _segments[segment].setEndLevel(lv);
}

double EnvelopeSegmentSequence::getSegmentEndLevel(segment_t segment) const
{
    return _segments[segment].getEndLevel();
}

void EnvelopeSegmentSequence::setSegmentBothLevels(segment_t segment, double lv)
{
    setSegmentStartLevel(segment, lv);
    setSegmentEndLevel(segment, lv);
}


void EnvelopeSegmentSequence::setLinkedLevel(segment_t segment, double lv)
{
    _segments[segment].setEndLevel(lv);
    
    // Set starting level of adjacent segment (unless
    // this is the last segment)
    if (segment < _segments.size() - 1)
    {
        _segments[segment + 1].setStartLevel(lv);
    }
}


void EnvelopeSegmentSequence::setLoopStart(EnvelopeSegmentSequence::segment_t segment)
{
    _loopStart = _segments.begin() + segment;
}

EnvelopeSegmentSequence::segment_t EnvelopeSegmentSequence::getLoopStart() const
{
    return _loopStart - _segments.begin();
}


void EnvelopeSegmentSequence::setLoopEnd(EnvelopeSegmentSequence::segment_t segment)
{
    // Plus one because the end points to one after
    // the loop end segment
    _loopEnd = _segments.begin() + segment + 1;
}

EnvelopeSegmentSequence::segment_t EnvelopeSegmentSequence::getLoopEnd() const
{
    return _loopEnd - _segments.begin();
}


void EnvelopeSegmentSequence::setLoopMax(unsigned short n)
{
    if (n > 64) _loopInf = true;
    else
    {
        _loopInf = false;
        _loopMax = n;
    }
}

unsigned short EnvelopeSegmentSequence::getLoopMax() const
{
    return _loopMax;
}


void EnvelopeSegmentSequence::setLoopInf(bool state)
{
    _loopInf = state;
}

bool EnvelopeSegmentSequence::loopInf() const
{
    return _loopInf;
}


std::vector<EnvelopeSegment>::size_type EnvelopeSegmentSequence::size() const
{
    return _segments.size();
}

void EnvelopeSegmentSequence::_changeSegment(EnvelopeSegmentSequence::segmentItr segment)
{
    _currSegmentNum = std::distance(_segments.begin(), segment);
    
    _currSample = 0;
}

void EnvelopeSegmentSequence::_resetLoop()
{
    // reset all the loop segments
    for (segmentItr itr = _loopStart; itr != _loopEnd; ++itr)
    { itr->reset(); }
    
    // Set current segment to loop start
    _currSegment = _loopStart;
    
    // Change _currSegmentNum and reset currSample
    _changeSegment(_loopStart);
    
    ++_loopCount;
}

void EnvelopeSegmentSequence::update()
{
    _currSample++;
    _currSegment->update();
}

double EnvelopeSegmentSequence::tick()
{
    if (_currSample >= _currSegment->getLength())
    {
        // Increment segment
        _currSegment++;
        
        // Check if we need to reset the loop
        if (_currSegment == _loopEnd && (_loopInf || _loopCount < _loopMax))
        { _resetLoop(); }
        
        // If we've reached the end, go back to last segment
        // which will continue to tick its end amplitude value
        else if (_currSegment == _segments.end()) _currSegment--;
        
        // else change
        else _changeSegment(_currSegment);
    }

    return _currSegment->tick();
}

ModEnvelopeSegmentSequence::ModEnvelopeSegmentSequence(segment_t numSegments,
                           segment_t numDocks,
                           double masterAmp)
: EnvelopeSegmentSequence(numSegments),
  ModUnit(numDocks + 2,masterAmp)
{ }

std::vector<ModDock*>::size_type ModEnvelopeSegmentSequence::numDocks_Segment(segment_t segmentNum) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].numDocks();
}

void ModEnvelopeSegmentSequence::setModUnitDepth_Segment(segment_t segmentNum,
                                       index_t dockNum,
                                       index_t modNum,
                                       double depth)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    _segments[segmentNum].setModUnitDepth(dockNum,modNum, depth);
}

double ModEnvelopeSegmentSequence::getModUnitDepth_Segment(segment_t segmentNum, index_t dockNum, index_t modNum) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].getModUnitDepth(dockNum, modNum);
}

void ModEnvelopeSegmentSequence::attachMod_Segment(segment_t segmentNum,
                                 index_t dockNum,
                                 ModUnit *mod)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    _segments[segmentNum].attachMod(dockNum, mod);
}

void ModEnvelopeSegmentSequence::detachMod_Segment(segment_t segmentNum,
                                 index_t dockNum,
                                 index_t modNum)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }

    _segments[segmentNum].detachMod(dockNum, modNum);
}

bool ModEnvelopeSegmentSequence::dockInUse_Segment(segment_t segmentNum, index_t dockNum) const
{
    return _segments[segmentNum].dockInUse(dockNum);
}

void ModEnvelopeSegmentSequence::setSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    _segments[segmentNum].setSidechain(dockNum, master, slave);
}

void ModEnvelopeSegmentSequence::unSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    _segments[segmentNum].unSidechain(dockNum, master, slave);
}

bool ModEnvelopeSegmentSequence::isSidechain_Segment(segment_t segmentNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].isSidechain(dockNum, master, slave);
}

bool ModEnvelopeSegmentSequence::isMaster_Segment(segment_t segmentNum, index_t dockNum, index_t index) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].isMaster(dockNum, index);
}

bool ModEnvelopeSegmentSequence::isSlave_Segment(segment_t segmentNum, index_t dockNum, index_t index) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].isSlave(dockNum, index);
}

unsigned long ModEnvelopeSegmentSequence::dockSize_Segment(segment_t segmentNum, index_t dockNum) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("segment index out of range!"); }
    
    return _segments[segmentNum].dockSize(dockNum);
}

void ModEnvelopeSegmentSequence::setSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum, double value)
{
    _segments[segmentNum]._mods[dockNum].setBaseValue(value);
}

double ModEnvelopeSegmentSequence::getSegmentModDockBaseValue(segment_t segmentNum, index_t dockNum) const
{
    return _segments[segmentNum]._mods[dockNum].getBaseValue();
}

ModEnvelopeSegmentSequenceFlexible::ModEnvelopeSegmentSequenceFlexible(segment_t numSegments,
                                           segment_t numDocks,
                                           double masterAmp)
: ModEnvelopeSegmentSequence(numSegments,numDocks,masterAmp)
{ }

void ModEnvelopeSegmentSequenceFlexible::setSegmentLength(segment_t segment, unsigned long ms)
{
    _segments[segment].setLength(ms * (Global::samplerate/1000.0));
}

unsigned long ModEnvelopeSegmentSequenceFlexible::getSegmentLength(segment_t segment) const
{
    return _segments[segment].getLength();
}