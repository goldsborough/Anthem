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
: EffectUnit(4,1),delayA_(0), delayB_(0),
  cutoff_(cutoff), q_(q), mode_(mode)
{
    setGain(gain);
    
    // Initial coefficients
    calcCoefs_(mode,cutoff,q, gain);
    
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
        
        calcCoefs_(mode_, newCutoff, q_, gain_);
    }
    
    // And Q factor
    if (mods_[Q]->inUse())
    {
        double newQ = mods_[Q]->tick();
        
        calcCoefs_(mode_, cutoff_, newQ, gain_);
    }
    
    double temp = sample
                - (coefA1_ * delayA_)
                - (coefA2_ * delayB_);
    
    double output = (coefB0_ * temp)
                  + (coefB1_ * delayA_)
                  + (coefB2_ * delayB_);
    
    
    // Store values into delay line
    delayB_ = delayA_;
    delayA_ = temp;
    
    // Check the gain modulation
    if (mods_[GAIN]->inUse())
    {
        // Convert db to amplitude
        double newGain = Util::dbToAmp(1,mods_[GAIN]->tick());
        
        calcCoefs_(mode_, cutoff_, q_, newGain);
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

void Filter::calcCoefs_(short mode, double cutoff, double q, double gain)
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
    
    switch (mode_)
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
        
    mode_ = mode;
    
    calcCoefs_(mode_,cutoff_,q_, gain_);
}

unsigned short Filter::getMode() const
{
    return mode_;
}

void Filter::setCutoff(double cutoff)
{
    if (cutoff < 0 || cutoff > Global::nyquistLimit)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and nyquist limit (20 Khz)"); }
    
    mods_[CUTOFF]->setBaseValue(cutoff);
    
    cutoff_ = cutoff;
    
    calcCoefs_(mode_,cutoff_,q_, gain_);
}

double Filter::getCutoff() const
{
    return cutoff_;
}

void Filter::setQ(double q)
{
    // Q factor cannot be 0 because of division by (2.0 * q) in calcCoefs_
    if (q < 0.01 || q > 20)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 20!"); }
    
    mods_[Q]->setBaseValue(q);
    
    q_ = q;
    
    calcCoefs_(mode_,cutoff_,q_, gain_);
}

double Filter::getQ() const
{
    return q_;
}

void Filter::setGain(double gain)
{
    if (gain < -20 || gain > 20)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and +20dB! "); }
    
    mods_[GAIN]->setBaseValue(gain);
    
    gain_ = gain;
    
    // Convert decibels to amplitude
    amp_ = Util::dbToAmp(1,gain);
    
    calcCoefs_(mode_,cutoff_,q_, gain_);
}

double Filter::getGain() const
{
    return gain_;
}