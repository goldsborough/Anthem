/********************************************************************************************//*!
*
*  @file        Envelope.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Envelope.hpp"
#include "Global.hpp"
#include "ModDock.hpp"

Envelope::Envelope(bool sustainEnabled)
: ModEnvelopeSegmentSequenceFlexible(7,1),
  _sustainEnabled(sustainEnabled),
  _lastTick(0)
{
    _currSegment = _segments.begin() + Segments::DELAY;
    
    // Set the hidden connector length to a 40th of the
    // samplerate, at 44.1 Khz that's 1102 samples. Not
    // too long to be noticed too much but just enough
    // to prevent transitions between loops from being
    // too abrupt
    _segments[CONNECTOR].setLength(Global::samplerate / 40.0);
    
    // Initial settings
    /*
    setSegmentLevel(Segments::ATTACK, 0.8);
    setSegmentLength(Segments::ATTACK, 500);
    setSegmentRate(Segments::ATTACK, 1.2);
    
    setSegmentLevel(Segments::A, 0.5);
    setSegmentLength(Segments::A, 500);
    setSegmentRate(Segments::A, 1.2);
    
    setSegmentStartLevel(Segments::RELEASE, 0.5);
    setSegmentRate(Segments::RELEASE, 0.6);
    setSegmentLength(Segments::RELEASE, 500);
    */

    _mods[AMP].setHigherBoundary(1);
    _mods[AMP].setLowerBoundary(0);
    _mods[AMP].setBaseValue(1);
}

void Envelope::setAmp(double amp)
{
    // Takes care of boundary checking and
    // member setting
    ModUnit::setAmp(amp);
    
    // Sets new base value for modulation
    _mods[AMP].setBaseValue(amp);
}

double Envelope::getAmp() const
{
    if (_mods[AMP].inUse())
    {
        return _mods[AMP].getBaseValue();
    }
    
    else return _amp;
}

void Envelope::_changeSegment(segmentItr segment)
{
    _currSegment = segment;
    
    _currSample = 0;

    _currSegmentNum = std::distance(_segments.begin(), segment);
    
    // If we just changed to the release segmentment
    // set the start level of the segmentment to the
    // last ticked value, (because note-off could be
    // abrupt)
    if (_currSegmentNum == Segments::RELEASE)
    {
        _currSegment->setStartLevel(_lastTick);
    }
    
}

void Envelope::_resetLoop()
{
    // Reset all segmentments
    for(segmentItr itr = _loopStart, end = _loopEnd + 1;
        itr != end;
        ++itr)
    {
        itr->reset();
    }
    
    // no need for connecting segmentment if the amplitude
    // is the same e.g. for the constant sustain
    if (_loopEnd->getEndLevel() == _loopStart->getStartLevel())
    {
        _changeSegment(_loopStart);
    }
    
    else
    {
        // Update values
        _changeSegment(_segments.begin() + CONNECTOR);
        
        // Reset the connector segmentment first
        _currSegment->reset();
    }
}

double Envelope::_tick()
{
    if (_currSample >= _currSegment->getLength())
    {
        if (_currSegmentNum == Segments::RELEASE) return 0;
        
        // If this is the end of the connecting segmentment,
        // change back to the loop start
        else if (_currSegmentNum == Hidden::CONNECTOR)
        {
            _changeSegment(_loopStart);
        }
        
        // If we reached the loop end segmentment and there are more loops to go
        // through, reset the loop
        else if (_currSegment == _loopEnd && (_loopInf || _loopCount++ < _loopMax))
        {
            _resetLoop();
        }
        
        // if we reached the end of segmentment c and the sustain is set to infinity
        // return the last tick
        else if (_currSegmentNum == Segments::C && _sustainEnabled)
        {
            return _lastTick;
        }
        
        else
        {
            // Change to next segmentment
            _changeSegment(++_currSegment);
            
            // Return last tick if the new segmentment has no length
            if (! _currSegment->getLength()) return _lastTick;
        }
    }
    
    _lastTick = _currSegment->tick();
    
    return _lastTick;
}

double Envelope::modulate(double sample, double depth, double maximum)
{
    // Modulate
    if (_mods[AMP].inUse())
    {
        _amp = _mods[AMP].tick();
    }
    
    return sample + (maximum * _tick() * depth * _amp);
}

void Envelope::setLoopStart(segment_t segment)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Cannot loop release segmentment!"); }
    
    _loopStart = _segments.begin() + segment;
    
    _segments[CONNECTOR].setEndLevel(_loopStart->getStartLevel());

    if (_loopStart > _loopEnd)
    { _loopEnd = _loopStart; }
}

void Envelope::setLoopEnd(segment_t segment)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Invalid loop segmentment, can only loop from\
                                  Segments Segments::ATTACK to Segments::C!"); }
    
    _loopEnd = _segments.begin() + segment;
    
    _segments[CONNECTOR].setStartLevel(_loopEnd->getEndLevel());
    
    if (_loopEnd < _loopStart)
    { _loopStart = _loopEnd; }
}

void Envelope::setSegmentRate(segment_t segment, double rate)
{
    if (segment < Segments::ATTACK || segment > Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment!"); }
    
    _segments[segment].setRate(rate);
    
    setSegmentModDockBaseValue(segment, EnvelopeSegment::RATE, rate);
}

double Envelope::getSegmentRate(segment_t segment) const
{
    if (segment < Segments::ATTACK || segment > Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment!"); }
    
    if (_segments[segment].dockInUse(EnvelopeSegment::RATE))
    {
        return getSegmentModDockBaseValue(segment, EnvelopeSegment::RATE);
    }
    
    else return _segments[segment].getRate();
}

void Envelope::setSegmentLevel(segment_t segment, double lv)
{
    if (segment >= Segments::RELEASE)
    { throw std::invalid_argument("Invalid segmentment for setting the level!"); }
    
    // Delay is always sustain so also set start level
    if (segment == Segments::DELAY)
    {
        _segments[Segments::DELAY].setStartLevel(lv);
    }
    
    // Set new linked levels
    EnvelopeSegmentSequence::setLinkedLevel(segment, lv);
    
    setSegmentModDockBaseValue(segment, EnvelopeSegment::END_LEVEL, lv);
    setSegmentModDockBaseValue(segment + 1, EnvelopeSegment::START_LEVEL, lv);
}

double Envelope::getSegmentLevel(segment_t segment) const
{
    if (_segments[segment].dockInUse(EnvelopeSegment::END_LEVEL))
    {
        return getSegmentModDockBaseValue(segment, EnvelopeSegment::END_LEVEL);
    }
    
    else return _segments[segment].getEndLevel();
}

void Envelope::noteOff()
{
    // If we aren't already in the release segmentment
    if (_currSegmentNum != Segments::RELEASE)
    {
        // Switch to release segmentment
        _changeSegment(_segments.begin() + Segments::RELEASE);
    }
}

void Envelope::setSustainEnabled(bool sustainEnabled)
{
    _sustainEnabled = sustainEnabled;
}

bool Envelope::sustainIsEnabled() const
{
    return _sustainEnabled;
}

void Envelope::setModUnitDepth_Segment(segment_t segmentNum,
                                   index_t dockNum,
                                   index_t modNum,
                                   double depth)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        _segments[segmentNum].setModUnitDepth(EnvelopeSegment::END_LEVEL, modNum, depth);
        
        if (segmentNum < _segments.size() - 1)
        {
            _segments[segmentNum + 1].setModUnitDepth(EnvelopeSegment::START_LEVEL, modNum, depth);
        }
    }
    
    else
    {
        _segments[segmentNum].setModUnitDepth(dockNum,modNum, depth);
    }
}

double Envelope::getModUnitDepth_Segment(segment_t segmentNum,
                                     index_t dockNum,
                                     index_t modNum) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return _segments[segmentNum].getModUnitDepth(EnvelopeSegment::END_LEVEL, modNum);
    }
    
    else return _segments[segmentNum].getModUnitDepth(dockNum, modNum);
}

void Envelope::attachMod_Segment(segment_t segmentNum,
                             index_t dockNum,
                             ModUnit *mod)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    // Set linked modulator if SEG_LEVEL
    if (dockNum == SEG_LEVEL)
    {
        _segments[segmentNum].attachMod(EnvelopeSegment::END_LEVEL, mod);
        
        // If last segmentment chosen, no next segmentment to set start level for
        if (segmentNum < _segments.size() - 1)
        {
            _segments[segmentNum + 1].attachMod(EnvelopeSegment::START_LEVEL, mod);
        }
    }
    
    else
    {
        _segments[segmentNum].attachMod(dockNum, mod);
    }
}

void Envelope::detachMod_Segment(segment_t segmentNum,
                             index_t dockNum,
                             index_t modNum)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        _segments[segmentNum].detachMod(dockNum, modNum);
        
        // If last segmentment chosen, no next segmentment to set start level for
        if (segmentNum < _segments.size() - 1)
        {
            _segments[segmentNum + 1].detachMod(dockNum, modNum);
        }
    }
    
    else
    {
        _segments[segmentNum].detachMod(dockNum, modNum);
    }
}

void Envelope::setSidechain_Segment(segment_t segmentNum,
                                index_t dockNum,
                                index_t master,
                                index_t slave)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        _segments[segmentNum].setSidechain(EnvelopeSegment::END_LEVEL, master, slave);
        
        if (segmentNum < _segments.size() - 1)
        {
            _segments[segmentNum + 1].setSidechain(EnvelopeSegment::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        _segments[segmentNum].setSidechain(dockNum, master, slave);
    }
}

void Envelope::unSidechain_Segment(segment_t segmentNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave)
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        _segments[segmentNum].unSidechain(EnvelopeSegment::END_LEVEL, master, slave);
        
        if (segmentNum < _segments.size() - 1)
        {
            _segments[segmentNum + 1].unSidechain(EnvelopeSegment::START_LEVEL, master, slave);
        }
    }
    
    else
    {
        _segments[segmentNum].unSidechain(dockNum, master, slave);
    }
}

bool Envelope::isSidechain_Segment(segment_t segmentNum,
                               index_t dockNum,
                               index_t master,
                               index_t slave) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        // Only need this segmentment here, segmentNum + 1 is the same for START_LEVEL
        return _segments[segmentNum].isSidechain(EnvelopeSegment::END_LEVEL, master, slave);
    }
    
    else return _segments[segmentNum].isSidechain(dockNum, master, slave);
}

bool Envelope::isMaster_Segment(segment_t segmentNum,
                            index_t dockNum,
                            index_t index) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return _segments[segmentNum].isMaster(EnvelopeSegment::END_LEVEL, index);
    }
    
    else return _segments[segmentNum].isMaster(dockNum, index);
}

bool Envelope::isSlave_Segment(segment_t segmentNum,
                           index_t dockNum,
                           index_t index) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return _segments[segmentNum].isSlave(EnvelopeSegment::END_LEVEL, index);
    }
    
    else return _segments[segmentNum].isSlave(dockNum, index);
}

unsigned long Envelope::dockSize_Segment(segment_t segmentNum, index_t dockNum) const
{
    if (segmentNum >= _segments.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == SEG_LEVEL)
    {
        return _segments[segmentNum].dockSize(EnvelopeSegment::END_LEVEL);
    }
    
    else return _segments[segmentNum].dockSize(dockNum);
}