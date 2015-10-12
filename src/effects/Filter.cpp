/********************************************************************************************//*!
*
*  @file        Filter.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Filter.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "ModDock.hpp"

#include <stdexcept>
#include <cmath>

Filter::Filter(unsigned short mode,
               double cutoff,
               double q,
               double gain)
: EffectUnit(4,1),_delayA(0), _delayB(0),
  _cutoff(cutoff), _q(q), _mode(mode)
{
    setGain(gain);
    
    // Initial coefficients
    _calcCoefs();
    
    _mods[CUTOFF].setHigherBoundary(Global::nyquistLimit);
    _mods[CUTOFF].setLowerBoundary(0);
    _mods[CUTOFF].setBaseValue(cutoff);
    
    _mods[Q].setHigherBoundary(20);
    _mods[Q].setLowerBoundary(0.01);
    _mods[Q].setBaseValue(q);
    
    _mods[GAIN].setHigherBoundary(20);
    _mods[GAIN].setLowerBoundary(-20);
    _mods[GAIN].setBaseValue(gain);
    
    _mods[DRYWET].setHigherBoundary(1);
    _mods[DRYWET].setLowerBoundary(0);
    _mods[DRYWET].setBaseValue(1);
}

void Filter::setDryWet(double dw)
{
    // For error checking
    EffectUnit::setDryWet(dw);
    
    _mods[DRYWET].setBaseValue(dw);
}

double Filter::getDryWet() const
{
    if (_mods[DRYWET].inUse())
    {
        return _mods[DRYWET].getBaseValue();
    }
    
    else return _dw;
}

double Filter::process(double sample)
{
    if (_mods[CUTOFF].inUse() ||
        _mods[Q].inUse()      ||
        _mods[GAIN].inUse())
    {
        // Modulate cutoff
        if (_mods[CUTOFF].inUse())
        {
            _cutoff = _mods[CUTOFF].tick();
        }
        
        // And Q factor
        if (_mods[Q].inUse())
        {
            _q = _mods[Q].tick();
        }
        
        // Check the gain modulation
        if (_mods[GAIN].inUse())
        {
            // Convert db to amplitude
            _gain = Util::dbToAmp(1,_mods[GAIN].tick());
        }
        
        _calcCoefs();
    }
    
    // Set the dry/wet
    if (_mods[DRYWET].inUse())
    {
        _dw = _mods[DRYWET].tick();
    }
    
    double temp = sample
                - (coefA1_ * _delayA)
                - (coefA2_ * _delayB);
    
    double output = (coefB0_ * temp)
                  + (coefB1_ * _delayA)
                  + (coefB2_ * _delayB);
    
    
    // Store values into delay line
    _delayB = _delayA;
    _delayA = temp;
    
    output *= _amp;
    
    return _dryWet(sample, output);
}

void Filter::_calcCoefs()
{
    double omega = (Global::twoPi / Global::samplerate) * _cutoff;
    
    double cosine = cos(omega);
    
    double sine = sin(omega);
    
    double alpha = sine / (2.0 * _q);
    
    double a0,a1,a2,b0,b1,b2;
    
    // Took out some constants
    a0 = 1.0 + alpha;
    a1 = -2.0 * cosine;
    a2 = 1.0 - alpha;
    
    b1 = -2 * cosine;
    
    switch (_mode)
    {
        case LOW_PASS:
        {
            b0 = (1.0 - cosine) / 2.0;
            b1 = (1.0 - cosine);
            b2 = coefB0_;
            
            break;
        }
        
        case HIGH_PASS:
        {
            b0 = (1 + cosine) / 2;
            b1 = -(1 + cosine);
            b2 = b0;
            
            break;
        }
            
        case BAND_PASS:
        {
            b0 = sine / 2;
            b1 = 0;
            b2 = -sine / 2;
            
            break;
        }
            
        case BAND_REJECT:
        {
            b0 = 1;
            b2 = 1;

            break;
        }
            
        case ALL_PASS:
        {
            b0 = 1 - alpha;
            b2 = 1 + alpha;
            
            break;
        }
            
        case PEAK:
        {
            double A = pow(10, (_gain/40));
        
            b0 = 1 + (alpha * A);
            b2 = 1 - (alpha * A);
            
            a0 = 1.0 + (alpha / A);
            a2 = 1.0 - (alpha / A);
                            
            break;
        }

        case LOW_SHELF:
        {
            double A = pow(10, (_gain/40));
            
            double temp = 2 * sqrt(A) * alpha;
            
            b0 = A * ((A + 1) - ((A-1) * cosine) + temp);
            b1 = 2 * A * ((A - 1) - ((A + 1) * cosine));
            b2 = A * ((A + 1) - ((A-1) * cosine) - temp);
            
            a0 = ((A + 1) + ((A - 1) * cosine) + temp);
            a1 = -2 * ((A - 1) + ((A + 1) * cosine));
            a2 = ((A + 1) + ((A - 1) * cosine) - temp);
            
            break;
        }
            
        case HIGH_SHELF:
        {
            double A = pow(10, (_gain/40));
            
            double temp = 2 * sqrt(A) * alpha;
            
            b0 = A * ((A + 1) + ((A-1) * cosine) + temp);
            b1 = -2 * A * ((A - 1) + ((A + 1) * cosine));
            b2 = A * ((A + 1) + ((A-1) * cosine) - temp);
            
            a0 = ((A + 1) - ((A - 1) * cosine) + temp);
            a1 = 2 * ( (A - 1) - ((A + 1) * cosine));
            a2 = ((A + 1) - ((A - 1) * cosine) - temp);
            
            break;
        }
    }
    
    coefB0_ = b0/a0;
    coefB1_ = b1/a0;
    coefB2_ = b2/a0;
    
    coefA1_ = a1/a0;
    coefA2_ = a2/a0;
}

void Filter::setMode(unsigned short mode)
{
    if (mode > HIGH_SHELF)
    { throw std::invalid_argument("Filter mode out of range!"); }
        
    _mode = mode;
    
    _calcCoefs();
}

unsigned short Filter::getMode() const
{
    return _mode;
}

void Filter::setCutoff(double cutoff)
{
    if (cutoff < 0 || cutoff > Global::nyquistLimit)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and nyquist limit (20 Khz)"); }
    
    if (_mods[CUTOFF].inUse())
    {
        _mods[CUTOFF].setBaseValue(cutoff);
    }
    
    _cutoff = cutoff;
    
    _calcCoefs();
}

double Filter::getCutoff() const
{
    if (_mods[CUTOFF].inUse())
    {
        return _mods[CUTOFF].getBaseValue();
    }
    
    else return _cutoff;
}

void Filter::setQ(double q)
{
    // Q factor cannot be 0 because of division by (2.0 * q) in _calcCoefs
    if (q < 0.01 || q > 20)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 20!"); }
    
    if (_mods[Q].inUse())
    {
        _mods[Q].setBaseValue(q);
    }
    
    _q = q;
    
    _calcCoefs();
}

double Filter::getQ() const
{
    if (_mods[Q].inUse())
    {
        return _mods[Q].getBaseValue();
    }
    
    else return _q;
}

void Filter::setGain(double gain)
{
    if (gain < -20 || gain > 20)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and +20dB! "); }
    
    if (_mods[GAIN].inUse())
    {
        _mods[GAIN].setBaseValue(gain);
    }
    
    _gain = gain;
    
    // Convert decibels to amplitude
    _amp = Util::dbToAmp(1,gain);
    
    _calcCoefs();
}

double Filter::getGain() const
{
    if (_mods[GAIN].inUse())
    {
        return  _mods[GAIN].getBaseValue();
    }
    
    else return _gain;
}