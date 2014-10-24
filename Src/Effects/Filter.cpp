//
//  Filter.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Filter.h"
#include "Global.h"
#include "Util.h"
#include "ModDock.h"

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
    _calcCoefs(mode,cutoff,q, gain);
    
    mods_[CUTOFF]->setHigherBoundary(Global::nyquistLimit);
    mods_[CUTOFF]->setLowerBoundary(0);
    mods_[CUTOFF]->setBaseValue(cutoff);
    
    mods_[Q]->setHigherBoundary(20);
    mods_[Q]->setLowerBoundary(0.01);
    mods_[Q]->setBaseValue(q);
    
    mods_[GAIN]->setHigherBoundary(20);
    mods_[GAIN]->setLowerBoundary(-20);
    mods_[GAIN]->setBaseValue(gain);
    
    mods_[DRYWET]->setHigherBoundary(1);
    mods_[DRYWET]->setLowerBoundary(0);
    mods_[DRYWET]->setBaseValue(1);
}

void Filter::setDryWet(double dw)
{
    mods_[DRYWET]->setBaseValue(dw);
    
    // For error checking
    EffectUnit::setDryWet(dw);
}

double Filter::process(double sample)
{
    // Modulate cutoff
    if (mods_[CUTOFF]->inUse())
    {
        double newCutoff = mods_[CUTOFF]->tick();
        
        _calcCoefs(_mode, newCutoff, _q, _gain);
    }
    
    // And Q factor
    if (mods_[Q]->inUse())
    {
        double newQ = mods_[Q]->tick();
        
        _calcCoefs(_mode, _cutoff, newQ, _gain);
    }
    
    double temp = sample
                - (_coefA1 * _delayA)
                - (_coefA2 * _delayB);
    
    double output = (_coefB0 * temp)
                  + (_coefB1 * _delayA)
                  + (_coefB2 * _delayB);
    
    
    // Store values into delay line
    _delayB = _delayA;
    _delayA = temp;
    
    // Check the gain modulation
    if (mods_[GAIN]->inUse())
    {
        // Convert db to amplitude
        double newGain = Util::dbToAmp(1,mods_[GAIN]->tick());
        
        _calcCoefs(_mode, _cutoff, _q, newGain);
    }
    
    output *= amp_;
    
    // Set the dry/wet
    if (mods_[DRYWET]->inUse())
    {
        double newDryWet = mods_[DRYWET]->tick();
        
        // Call dryWet_ with custom dry/wet value (instead of dw_)
        return dryWet_(sample, output, newDryWet);
    }
    
    return dryWet_(sample, output);
}

void Filter::_calcCoefs(short mode, double cutoff, double q, double gain)
{
    double omega = (Global::twoPi / Global::samplerate) * cutoff;
    
    double cosine = cos(omega);
    
    double sine = sin(omega);
    
    double alpha = sine / (2.0 * q);
    
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
            b2 = _coefB0;
            
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
            double A = pow(10, (gain/40));
        
            b0 = 1 + (alpha * A);
            b2 = 1 - (alpha * A);
            
            a0 = 1.0 + (alpha / A);
            a2 = 1.0 - (alpha / A);
                            
            break;
        }

        case LOW_SHELF:
        {
            double A = pow(10, (gain/40));
            
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
            double A = pow(10, (gain/40));
            
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
    
    _coefB0 = b0/a0;
    _coefB1 = b1/a0;
    _coefB2 = b2/a0;
    
    _coefA1 = a1/a0;
    _coefA2 = a2/a0;
}

void Filter::setMode(unsigned short mode)
{
    if (mode > HIGH_SHELF)
    { throw std::invalid_argument("Filter mode out of range!"); }
        
    _mode = mode;
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

unsigned short Filter::getMode() const
{
    return _mode;
}

void Filter::setCutoff(double cutoff)
{
    if (cutoff < 0 || cutoff > Global::nyquistLimit)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and nyquist limit (20 Khz)"); }
    
    mods_[CUTOFF]->setBaseValue(cutoff);
    
    _cutoff = cutoff;
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

double Filter::getCutoff() const
{
    return _cutoff;
}

void Filter::setQ(double q)
{
    // Q factor cannot be 0 because of division by (2.0 * q) in _calcCoefs
    if (q < 0.01 || q > 20)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 20!"); }
    
    mods_[Q]->setBaseValue(q);
    
    _q = q;
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

double Filter::getQ() const
{
    return _q;
}

void Filter::setGain(double gain)
{
    if (gain < -20 || gain > 20)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and +20dB! "); }
    
    mods_[GAIN]->setBaseValue(gain);
    
    _gain = gain;
    
    // Convert decibels to amplitude
    amp_ = Util::dbToAmp(1,gain);
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

double Filter::getGain() const
{
    return _gain;
}