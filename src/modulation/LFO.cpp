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
    mods_[FREQ].setHigherBoundary(100);
    mods_[FREQ].setLowerBoundary(0);
    mods_[FREQ].setBaseValue(freq);
    
    mods_[PHASE].setHigherBoundary(360);
    mods_[PHASE].setLowerBoundary(0);
    mods_[PHASE].setBaseValue(phaseOffset);
    
    mods_[AMP].setHigherBoundary(1);
    mods_[AMP].setLowerBoundary(0);
    mods_[AMP].setBaseValue(amp);
}

void LFO::setFrequency(double Hz)
{
    Oscillator::setFrequency(Hz);
    
    mods_[FREQ].setBaseValue(Hz);
}

double LFO::getFrequency() const
{
    if (mods_[FREQ].inUse())
    {
        return mods_[FREQ].getBaseValue();
    }
    
    else return freq_;
}

void LFO::setPhaseOffset(double degrees)
{
    Oscillator::setPhaseOffset(degrees);
    
    mods_[PHASE].setBaseValue(degrees);
}

double LFO::getPhaseOffset() const
{
    if (mods_[PHASE].inUse())
    {
        return mods_[PHASE].getBaseValue();
    }
    
    else return Oscillator::getPhaseOffset();
}

void LFO::setAmp(double amp)
{
    // For boundary checking
    if (amp < 0 || amp > 1)
    { throw std::invalid_argument("Amplitude cannot be greater 1 or less than 0!"); }
    
    amp_ = amp;
    
    mods_[AMP].setBaseValue(amp);
}

double LFO::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    else return amp_;
}

double LFO::modulate(double sample, double depth, double maximum)
{
    // Modulate rate/frequency
    if (mods_[FREQ].inUse())
    {
        Oscillator::setFrequency(mods_[FREQ].tick());
    }
    
    // Modulate phase offset
    if (mods_[PHASE].inUse())
    {
        Oscillator::setPhaseOffset(mods_[PHASE].tick());
    }
    
    // Get amplitude modulation
    if (mods_[AMP].inUse())
    {
        amp_ = mods_[AMP].tick();
    }

    // Actual modulation by LFO
    return sample + (maximum * Oscillator::tick() * depth * amp_);
}

LFOSequence::LFOSequence(unsigned short seqLength, double rate)
: ModEnvelopeSegmentSequence(seqLength,1), lfos_(seqLength)
{
    setLoopStart(0);
    setLoopEnd(seqLength - 1);
    
    setLoopInf();
    
    setRate(rate);
    
    // Init segs
    for (int i = 0; i < seqLength; ++i)
    {
        // Attach lfos to segment
        segments_[i].attachMod(EnvelopeSegment::START_LEVEL, &lfos_[i].lfo);
        segments_[i].attachMod(EnvelopeSegment::END_LEVEL, &lfos_[i].lfo);
        
        setSegmentBothLevels(i, 1);
        
        setScaledModFreq_(i);
    }
    
    mods_[RATE].setHigherBoundary(10);
    mods_[RATE].setLowerBoundary(0);
    mods_[RATE].setBaseValue(rate);
}

double LFOSequence::getScaledModFreqValue(double freq) const
{
    // Since the rate is in cycles per segment
    // and not cycles per second, we get the
    // "period" of the segment and multiply that
    // by the rate, giving the mod wave's frequency.
    
    if (! segLen_) return 0;
    
    // To go from samples to Hertz, simply
    // divide the samplerate by the length
    // in samples e.g. 44100 / 22050 = 2 Hz
    double temp = Global::samplerate / static_cast<double>(segLen_);
    
    // Multiply by wanted frequency
    return freq * temp;
}

void LFOSequence::setScaledModFreq_(segment_t seg)
{
    // Set scaled frequency to frequency of lfo
    lfos_[seg].lfo.setFrequency(getScaledModFreqValue(lfos_[seg].freq));
}

void LFOSequence::resizeSegmentsFromRate_(double rate)
{
    // get the period, divide up into _segNum pieces
    segLen_ = (Global::samplerate / rate) / segments_.size();
    
    // Set all segments' lengths
    for (int i = 0; i < segments_.size(); i++)
    {
        segments_[i].setLength(segLen_);
        
        // Scale frequency of mods according to length
        setScaledModFreq_(i);
    }
}

unsigned long LFOSequence::getSegmentLength() const
{
    return segLen_;
}

void LFOSequence::setAmp(double amp)
{
    // For boundary checking
    ModUnit::setAmp(amp);

    mods_[AMP].setBaseValue(amp);
}

double LFOSequence::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    else return amp_;
}

void LFOSequence::setRate(double Hz)
{
    if (Hz < 0 || Hz > 10)
    { throw std::invalid_argument("Rate cannot be less than zero or greater 10!"); }
    
    rate_ = Hz;
    
    mods_[RATE].setBaseValue(rate_);
    
    resizeSegmentsFromRate_(rate_);
}

double LFOSequence::getRate() const
{
    if (mods_[RATE].inUse())
    {
        return mods_[RATE].getBaseValue();
    }
    
    else return rate_;
}

void LFOSequence::setSegmentRate(segment_t seg, double rate)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    segments_[seg].setRate(rate);
}

void LFOSequence::setModWavetable(segment_t seg, unsigned short wt)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    lfos_[seg].lfo.setWavetable(wt);
}

std::shared_ptr<Wavetable> LFOSequence::getModWavetable(segment_t seg) const
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return lfos_[seg].lfo.getWavetable();
}

void LFOSequence::setModDepth(segment_t seg, double depth)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    segments_[seg].setModUnitDepth(EnvelopeSegment::START_LEVEL, 0, depth);
    segments_[seg].setModUnitDepth(EnvelopeSegment::END_LEVEL, 0, depth);
}

double LFOSequence::getModDepth(segment_t seg) const
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return segments_[seg].getModUnitDepth(EnvelopeSegment::END_LEVEL, 0);
    return segments_[seg].getModUnitDepth(EnvelopeSegment::END_LEVEL, 0);
}

void LFOSequence::setModFreq(segment_t seg, double freq)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    lfos_[seg].freq = freq;
    
    setScaledModFreq_(seg);
}

double LFOSequence::getModFreq(segment_t seg) const
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return lfos_[seg].freq;
}

void LFOSequence::setModPhaseOffset(segment_t seg, double degrees)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    lfos_[seg].lfo.setPhaseOffset(degrees);
}

double LFOSequence::getModPhaseOffset(segment_t seg)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Segment out of range for LFOSequence!"); }
    
    return lfos_[seg].lfo.getPhaseOffset();
}

void LFOSequence::insertSegment(segment_t pos, const EnvelopeSegment& seg)
{
    if (pos > segments_.size())
    { throw std::invalid_argument("Invalid insertion position!"); }
    
    // pushing back a segment is faster and simpler than insertion
    else if (pos == segments_.size() && segments_.size() + 1 < segments_.capacity())
    { addSegment(); }
    
    // Store distances because the iterators will be invalidated
    segment_t start = std::distance(segments_.begin(), loopStart_);
    segment_t end = std::distance(segments_.begin(), loopEnd_);
    
    // Positions that are greater than the insertion point have to
    // increment together with the segment they refer to
    if (end > pos) ++end;
    
    if (start > pos) ++start;
    
    // Note the greater or equal
    if (currSegmentNum_ >= pos) ++currSegmentNum_;
    
    segments_.insert(segments_.begin() + pos, seg);
    
    // Set iterators to valid items now
    loopEnd_ = segments_.begin() + end;
    loopStart_ = segments_.begin() + start;
    
    currSegment_ = segments_.begin() + currSegmentNum_;
    
    resizeSegmentsFromRate_(rate_);
    
    lfos_.insert(lfos_.begin() + pos, LFOSequence_LFO());
}

void LFOSequence::insertSegment(segment_t pos)
{
    insertSegment(pos, EnvelopeSegment());
}

void LFOSequence::addSegment()
{
    // If there will be a re-allocation we need to update
    // iterators whicc is done in insertSegment()
    if (segments_.size() + 1 == segments_.capacity())
    { insertSegment(segments_.size() - 1); }
    
    else
    {
        segments_.push_back(EnvelopeSegment());
        
        resizeSegmentsFromRate_(rate_);
        
        lfos_.push_back(LFOSequence_LFO());
    }
}

void LFOSequence::removeLastSegment()
{
    removeSegment(segments_.size() - 1);
}

void LFOSequence::removeSegment(segment_t seg)
{
    if (seg >= segments_.size())
    { throw std::invalid_argument("Invalid segment index!"); }
    
    // Store distances because the iterators will be invalidated
    segment_t start = std::distance(segments_.begin(), loopStart_);
    segment_t end = std::distance(segments_.begin(), loopEnd_);
    
    if (end > seg) --end;
    
    if (start > seg) --start;
    
    if (currSegmentNum_ > seg) --currSegmentNum_;
    
    segments_.erase(segments_.begin() + seg);
    
    // Set iterators to valid items now
    loopEnd_ = segments_.begin() + end;
    loopStart_ = segments_.begin() + start;
    
    currSegment_ = segments_.begin() + currSegmentNum_;
    
    if (seg == currSegmentNum_)
    {
        // Change segment to new current one
        changeSegment_(currSegment_);
    }
    
    resizeSegmentsFromRate_(rate_);
    
    lfos_.erase(lfos_.begin() + seg);
}

void LFOSequence::setModUnitDepth_Segment(segment_t segNum,
                                index_t dockNum,
                                index_t modNum,
                                double depth)
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // The depth ModDock is not part of the LFO but of the segment
        // itself because the depth is the depth of the LFO in the
        // EnvelopeSegment's Moddock. The depth is modulated by having two
        // other ModUnits sidechain the internal LFO, so the LFO's
        // depth ModDock is actually just an extension of the EnvelopeSegment's
        // ModDock. modNum + 1 because the LFO is at index 0
        segments_[segNum].setModUnitDepth(EnvelopeSegment::END_LEVEL, modNum + 1, depth);
    }
    
    // All other parameters belong to the lfo (phase and freq)
    else lfos_[segNum].lfo.setModUnitDepth(dockNum,modNum, depth);
}

double LFOSequence::getModUnitDepth_Segment(segment_t segNum, index_t dockNum, index_t modNum) const
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        
        return segments_[segNum].getModUnitDepth(EnvelopeSegment::END_LEVEL, modNum + 1);
    }
    
    // All other parameters belong to the lfo (phase + freq)
    else return lfos_[segNum].lfo.getModUnitDepth(dockNum, modNum);
}

void LFOSequence::attachMod_Segment(segment_t segNum,
                           index_t dockNum,
                           ModUnit *mod)
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Attach the depth ModUnit to the EnvelopeSegment's dock
        // because they sidechain the internal LFO's depth
        segments_[segNum].attachMod(EnvelopeSegment::END_LEVEL, mod);
        
        // Make the latest addition side chain the internal LFO's depth
        segments_[segNum].setSidechain(EnvelopeSegment::END_LEVEL, segments_[segNum].dockSize(EnvelopeSegment::END_LEVEL) - 1, 0);
    }
    
    else lfos_[segNum].lfo.attachMod(dockNum, mod);
}

void LFOSequence::detachMod_Segment(segment_t segNum,
                           index_t dockNum,
                           index_t modNum)
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        segments_[segNum].detachMod(EnvelopeSegment::END_LEVEL, modNum);
    }
    
    else lfos_[segNum].lfo.detachMod(dockNum, modNum);
}

void LFOSequence::setSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master from the internal LFO
        segments_[segNum].unSidechain(EnvelopeSegment::END_LEVEL, master + 1, 0);
        
        // Make it sidechain another ModUnit. + 1 because internal LFO is at
        // index 0
        segments_[segNum].setSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
    }
    
    else lfos_[segNum].lfo.setSidechain(dockNum, master, slave);
}

void LFOSequence::unSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave)
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // Unsidechain the master and slave
        segments_[segNum].unSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
        
        // Make the master sidechain the internal LFO again
        segments_[segNum].setSidechain(EnvelopeSegment::END_LEVEL, master + 1, 0);
    }
    
    else lfos_[segNum].lfo.unSidechain(dockNum, master, slave);
}

bool LFOSequence::isSidechain_Segment(segment_t segNum, index_t dockNum, index_t master, index_t slave) const
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // + 1 because internal LFO is at index 0
        return segments_[segNum].isSidechain(EnvelopeSegment::END_LEVEL, master + 1, slave + 1);
    }
    
    else return lfos_[segNum].lfo.isSidechain(dockNum, master, slave);
}

bool LFOSequence::isMaster_Segment(segment_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segments_[segNum].isMaster(EnvelopeSegment::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].lfo.isMaster(dockNum, index);
}

bool LFOSequence::isSlave_Segment(segment_t segNum, index_t dockNum, index_t index) const
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        return segments_[segNum].isSlave(EnvelopeSegment::END_LEVEL, index + 1);
    }
    
    else return lfos_[segNum].lfo.isSlave(dockNum, index);
}

unsigned long LFOSequence::dockSize_Segment(segment_t segNum, index_t dockNum) const
{
    if (segNum >= segments_.size())
    { throw std::invalid_argument("Segment index out of range!"); }
    
    if (dockNum == MOD_DEPTH)
    {
        // - 1 because internal LFO is part of it
        return segments_[segNum].dockSize(EnvelopeSegment::END_LEVEL) - 1;
    }
    
    else return lfos_[segNum].lfo.dockSize(dockNum);
}

void LFOSequence::update()
{
    ModEnvelopeSegmentSequence::update();
    
    for (std::vector<LFOSequence_LFO>::iterator itr = lfos_.begin(), end = lfos_.end();
         itr != end;
         ++itr)
    {
        itr->lfo.update();
    }
}

double LFOSequence::modulate(double sample, double depth, double)
{
    if (mods_[RATE].inUse())
    {
        rate_ = mods_[RATE].tick();
        
        resizeSegmentsFromRate_(rate_);
    }
    
    if (mods_[AMP].inUse())
    {
        amp_ =  mods_[AMP].tick();
    }
    
    return sample * tick() * depth * amp_;
}

LFOUnit::LFOUnit(Mode mode)
: ModUnit(1), fader_(new Crossfader)
{
    mods_[AMP].setHigherBoundary(1);
    mods_[AMP].setLowerBoundary(0);
    mods_[AMP].setBaseValue(1);
    
    setMode(mode);
}

LFOUnit::LFOUnit(const LFOUnit& other)
: ModUnit(other),
  fader_(new Crossfader(*other.fader_))
{
    for (unsigned short i = 0; i < 2; ++i)
    {
        lfoSequences_[i] = other.lfoSequences_[i];
        lfos_[i] = other.lfos_[i];
    }
    
    setMode(other.mode_);
}

LFOUnit& LFOUnit::operator= (const LFOUnit& other)
{
    if (this != &other)
    {
        ModUnit::operator=(other);
        
        *fader_ = *other.fader_;
        
        for (unsigned short i = 0; i < 2; ++i)
        {
            lfoSequences_[i] = other.lfoSequences_[i];
            lfos_[i] = other.lfos_[i];
        }
        
        setMode(other.mode_);
    }
    
    return *this;
}

void LFOUnit::setAmp(double amp)
{
    // For boundary checking
    ModUnit::setAmp(amp);
    
    if (mods_[AMP].inUse())
    {
        mods_[AMP].setBaseValue(amp);
    }
}

double LFOUnit::getAmp() const
{
    if (mods_[AMP].inUse())
    {
        return mods_[AMP].getBaseValue();
    }
    
    else return amp_;
}

LFOSequence& LFOUnit::seqs(bool unit)
{
    return lfoSequences_[unit];
}

LFO& LFOUnit::lfos(bool unit)
{
    return lfos_[unit];
}

Crossfader& LFOUnit::fader()
{
    return *fader_;
}

void LFOUnit::setMode(Mode mode)
{
    mode_ = mode;
    
    switch(mode)
    {
        case Mode::SEQ:
        {
            fader_->setLeftUnit(&lfoSequences_[0]);
            
            fader_->setRightUnit(&lfoSequences_[1]);
            
            break;
        }
    
        case Mode::LFO:
        {
            fader_->setLeftUnit(&lfos_[0]);
            
            fader_->setRightUnit(&lfos_[1]);
            
            break;
        }
    }
}

LFOUnit::Mode LFOUnit::getMode() const
{
    return mode_;
}

void LFOUnit::update()
{
    switch(mode_)
    {
        case Mode::SEQ:
        {
            lfoSequences_[0].update();
            lfoSequences_[1].update();
            
            break;
        }
            
        case Mode::LFO:
        {
            lfos_[0].update();
            lfos_[1].update();
            
            break;
        }
    }
    
}

double LFOUnit::modulate(double sample, double depth, double maximum)
{
    if (mods_[AMP].inUse())
    {
        amp_ =  mods_[AMP].tick();
    }
    
    if (! active_) return sample;
    
    // Tick the crossfaded value from the lfos and multiply by the envelope
    // value and the total amplitude value
    return fader_->modulate(sample, depth, maximum) * amp_;
}
