//
//  Filter.cpp
//  Synth
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Filter.h"
#include "Global.h"
#include "Util.h"

#include <stdexcept>
#include <cmath>

// References: http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
// http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/
// and the BasicSynth of course


Filter::Filter(const unsigned short& mode,
       const double& cutoff,
       const double& q,
       const double& gain)
: _delayA(0), _delayB(0), _mode(mode),
  _cutoff(cutoff), _q(q), _gain(gain)
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
    
    sample *= _gain;
    
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
            b1 = -2 * cosine;
            b2 = 1;

            break;
        }
            
        case ALL_PASS:
        {
            b0 = 1 - alpha;
            b1 = -2 * cosine;
            b2 = 1 + alpha;
            
            break;
        }
            
        case PEAK:
        {
            double A = pow(10, (_gain/40));
        
            b0 = 1 + (alpha * A);
            b1 = -2 * cosine;
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
    
    _coefB0 = b0/a0;
    _coefB1 = b1/a0;
    _coefB2 = b2/a0;
    
    _coefA1 = a1/a0;
    _coefA2 = a2/a0;
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
    if (cutoff < 0 || cutoff > Global::nyquistLimit)
    { throw std::invalid_argument("Cutoff out of range, must be between 0 and 0.5!"); }
    
    _cutoff = cutoff;
    _calcCoefs();
}

void Filter::setQ(const double& q)
{
    if (q < 0 || q > 20)
    { throw std::invalid_argument("Bandwith out of range, must be between 0 and 1!"); }
    
    _q = q;
    _calcCoefs();
}

void Filter::setGain(const short& gain)
{
    if (_mode < PEAK)
    { throw std::invalid_argument("Gain only available for shelf and peak filters! "); }
        
    else if (gain < -20 || gain > 0)
    { throw std::invalid_argument("Gain out of range, must be between -20dB and 0dB! "); }
        
    _gain = Util::dbToAmp(1,gain);
    
    _calcCoefs();
}

