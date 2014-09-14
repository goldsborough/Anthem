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
: _delayA(0), _delayB(0), _mode(mode),
  _cutoff(cutoff), _q(q)
{
    setGain(gain);
    
    // Initial coefficients
    _calcCoefs(mode,cutoff,q, gain);
    
    // Initialize mod docks
    _initModDocks();
}

void Filter::_initModDocks()
{
    // 2 Mod units each for cutoff, Q, gain and dry/wet
    _mods = {new ModDock(2), new ModDock(2), new ModDock(2), new ModDock(2)};
}

double Filter::process(double sample)
{
    // Modulate cutoff
    if (_mods[CUTOFF]->inUse())
    {
        // Get current cutoff (can't set as filter's cutoff or else the base
        // value would change, same for all other changes too)
        double newCutoff = _mods[CUTOFF]->checkAndTick(_cutoff, 0, 20000);
        
        _calcCoefs(_mode, newCutoff, _q, _gain);
    }
    
    // And Q factor
    if (_mods[Q]->inUse())
    {
        double newQ = _mods[Q]->checkAndTick(_q, 0.01, 20);
        
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
    
    // Check the gain
    if (_mods[GAIN]->inUse())
    {
        double newGain = _mods[GAIN]->checkAndTick(_gain, -20, 20);
        
        _calcCoefs(_mode, _cutoff, _q, newGain);
    }
    
    output *= _gain;
    
    // Set the dry/wet
    if (_mods[DRYWET]->inUse())
    {
        double dw = _mods[DRYWET]->checkAndTick(_dw, 0, 1);
        
        // Call _dryWet with custom dry/wet value (instead of _dw)
        return _dryWet(sample, output, dw);
    }
    
    return _dryWet(sample, output);
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

void Filter::setCutoff(double cutoff)
{
    if (cutoff < 0 || cutoff > Global::nyquistLimit)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and nyquist limit (20 Khz)"); }
    
    _cutoff = cutoff;
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

void Filter::setQ(double q)
{
    // Q factor cannot be 0 because of division by (2.0 * q) in _calcCoefs
    if (q < 0.01 || q > 20)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 20!"); }
    
    _q = q;
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}

void Filter::setGain(double gain)
{
    if (gain < -20 || gain > 20)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and 20dB! "); }
    
    _gain = Util::dbToAmp(1,gain);
    
    _calcCoefs(_mode,_cutoff,_q, _gain);
}