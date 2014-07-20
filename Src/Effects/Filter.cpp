//
//  Filter.cpp
//  Synth
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Filter.h"
#include "Global.h"

#include <stdexcept>
#include <cmath>

Filter::Filter(const unsigned short& mode,
       const double& cutoff,
       const double& bw,
       const double& gain)
: _delayA(0), _delayB(0), _mode(mode),
  _cutoff(cutoff), _q(10*bw), _gain(gain)
{
    // Initial coefficients
    _calcCoefs();
}

void Filter::process(double &sample)
{
    double temp = sample
                - (_coefA1 * _delayA)
                - (_coefA2 * _delayB);
    
    sample = (_coefB0 * temp)
           + (_coefB1 * _delayA)
           + (_coefB2 * _delayB);
    
    _delayB = _delayA;
    _delayA = temp;
}

void Filter::_calcCoefs()
{
    double omega = (Global::twoPi * _cutoff) / Global::samplerate;
    
    double cosine = cos(omega);
    double sine = sin(omega);
    
    double alpha = sine / (2 * _q);
    
    switch (_mode)
    {
        case LOW_PASS:
        {
            _coefB0 = (1 - cosine) / 2;
            _coefB1 = (1 - cosine);
            _coefB2 = (1 - cosine) / 2;
            
            _coefA1 = -2 * cosine;
            _coefA2 = 1 - alpha;
            
            break;
        }
        
        case HIGH_PASS:
        {
            _coefB0 = (1 + cosine) / 2;
            _coefB1 = -(1 + cosine);
            _coefB2 = (1 + cosine) / 2;
            
            _coefA1 = -2 * cosine;
            _coefA2 = 1 - alpha;
            
            break;
        }
            
        case BAND_PASS:
        {
            _coefB0 = sine / 2;
            _coefB1 = 0;
            _coefB2 = -sine / 2;
            
            _coefA1 = -2 * cosine;
            _coefA2 = 1 - alpha;
            
            break;
        }
            
        case BAND_REJECT:
        {
            _coefB0 = 1;
            _coefB1 = -2 * cosine;
            _coefB2 = 1;
            
            _coefA1 = -2 * cosine;
            _coefA2 = 1 - alpha;

            break;
        }
            
        case ALL_PASS:
        {
            _coefB0 = 1 - alpha;
            _coefB1 = -2 * cosine;
            _coefB2 = 1 + alpha;
            
            _coefA1 = -2 * cosine;
            _coefA2 = 1 - alpha;
            
            break;
        }
            
        case PEAK:
        {
            double A = pow(10, (_gain/40));
        
            _coefB0 = 1 + (alpha * A);
            _coefB1 = -2 * cosine;
            _coefB2 = 1 - (alpha * A);
            
            _coefA1 = _coefB1;
            _coefA2 = 1 - (alpha / A);
                            
            break;
        }

        case LOW_SHELF:
        {
            double A = pow(10, (_gain/40));
            
            double temp = 2 * sqrt(A) * alpha;
            
            _coefB0 = A * ((A + 1) - ((A-1) * cosine) + temp);
            _coefB1 = 2 * A * ((A - 1) - ((A + 1) * cosine));
            _coefB2 = A * ((A + 1) - ((A-1) * cosine) - temp);
            
            _coefA1 = -2 * ((A - 1) + ((A + 1) * cosine));
            _coefA2 = ((A + 1) + ((A - 1) * cosine) - temp);
            
            break;
        }
            
        case HIGH_SHELF:
        {
            double A = pow(10, (_gain/40));
            
            double temp = 2 * sqrt(A) * alpha;
            
            _coefB0 = A * ((A + 1) + ((A-1) * cosine) + temp);
            _coefB1 = -2 * A * ((A - 1) + ((A + 1) * cosine));
            _coefB2 = A * ((A + 1) + ((A-1) * cosine) - temp);
            
            _coefA1 = 2 * ( (A - 1) - ((A + 1) * cosine));
            _coefA2 = ((A + 1) - ((A - 1) * cosine) - temp);
            
            break;
        }
    }
}

void Filter::setMode(const unsigned short& mode)
{
    if (mode > HIGH_SHELF)
    { throw std::invalid_argument("Filter mode out of range!"); }
        
    _mode = mode;
    _calcCoefs();
}

void Filter::setCutoff(const double& cutoff)
{
    if (cutoff < 0 || cutoff > 0.5)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and 0.5!"); }
    
    _cutoff = cutoff;
    _calcCoefs();
}

void Filter::setBandWidth(const double& bw)
{
    if (bw < 0 || bw > 1)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 1!"); }
    
    _q= 10 * bw;
    _calcCoefs();
}

void Filter::setGain(const short& gain)
{
    if (_mode < PEAK)
    { throw std::invalid_argument("Gain only available for shelf and peak filters! "); }
        
    else if (gain < -20 || gain > 20)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and 20dB! "); }
        
    _gain = gain;
    _calcCoefs();
}

