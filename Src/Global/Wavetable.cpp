//
//  Wavetable.cpp
//  Synth
//
//  Created by Peter Goldsborough on 22/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Wavetable.h"
#include "Global.h"
#include "Parser.h"
#include <fstream>
#include <iostream>

void Wavetable::Init(unsigned int wtLen)
{
    _wtLength = wtLen;
    _fundIncr = twoPI / _wtLength;
    
    TextParser textParser("/Users/petergoldsborough/Documents/vibe/Resources/Wavetables/wavetables.txt");
    
    _wtNames = textParser.readAllItems();
    
    /*
    for (int i = 0; i < _wtNames.size(); i++)
        _tables.push_back( _readWavetable(i) );
     
     */
    
    VibeWTParser parser;
    
    partVec vec;
    
    for (int i = 1; i <= 64; i++)
    {
        vec.push_back(Partial(i, 1.0/i));
    }
    
    parser.writeWT("/Users/petergoldsborough/Documents/vibe/Resources/Wavetables/saw64.vwt", genWave(vec,0.635));
    
    _tables.push_back( _readWavetable(16) );
    
}

double * Wavetable::getWaveform(const int mode)
{
    if (mode < 0 || mode >= _tables.size())
        throw std::out_of_range("Mode out of range");
    
    if (mode != NONE)
        return _tables[mode];
    
    return 0;
}


void Wavetable::round(double& val, unsigned int bitWidth)
{
    double factor = 1.0/bitWidth;
    
    std::vector<double> vec;
    
    // Fill vector with all possible values
    
    for (double i = -1.0; i <= 1.0; i += factor) vec.push_back(i);
    
    // Find the first value that is bigger than the current num,
    // then compute the midpoint between the current and the next
    // value and see whether it is is bigger than or smaller than
    // that and round appropriately
    
    for (uint32_t i = 0; i < vec.size() - 1; i++)
    {
        if (val <= vec[i + 1])
        {
            double mid = vec[i] + (factor/2);
            
            if (val < mid) val = vec[i];

            else val = vec[i + 1];

            break;
        }
    }
}

double * Wavetable::genWave(const partVec& partials,
                            double masterAmp, bool sigmaAprox,
                            unsigned int bitWidth)
{
    double * wt = new double [_wtLength + 1];
    
    double * amp = new double [partials.size()];
    double * phase = new double [partials.size()];
    double * phaseIncr = new double [partials.size()];
    
    double sigmaK = PI / partials.size();
    double sigmaV;
    
    // convert the binary bitwidth to decimal
    bitWidth = pow(2, bitWidth);
    
    for (unsigned short p = 0; p < partials.size(); p++)
    {
        const Partial& partial = partials[p];
        
        phase[p] = partial.phaseOffs;
        phaseIncr[p] = _fundIncr * partial.num;
        
        amp[p] = partial.amp * masterAmp;
        
        if (sigmaAprox)
        {
            sigmaV = sigmaK * partial.num;
            
            amp[p] *= sin(sigmaV) / sigmaV;
        }
    }
    
    for (unsigned int n = 0; n < _wtLength; n++)
    {
        double value = 0.0;
        
        for (unsigned short p = 0; p < partials.size(); p++)
        {
            value += sin(phase[p]) * amp[p];
            
            phase[p] += phaseIncr[p];
            
            if (phase[p] >= twoPI)
                phase[p] -= twoPI;
        }
        
        if (bitWidth < 65536)
            round(value, bitWidth);
        
        wt[n] = value;
        
    }
    
    // Append the last item for interpolation
    
    wt[_wtLength] = wt[0];
    
    delete [] phase;
    delete [] phaseIncr;
    delete [] amp;
    
    return wt;
    
}

double* Wavetable::smoothSaw()
{
    double* wt = new double[_wtLength + 1];
    
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // The first part is calculated linearly so that the
    // amplitude simply decrements from 1 to -1. This is
    // measured in amplitude
    
    double amp = 1;
    
    // The second part is measured in time, going from 0.9
    // to 1 (of the wavetable period)
    
    double ind = 0.9;
    
    // Increment value from -1 to 1
    double ampIncr = 2.0/(_wtLength * 0.9);
    
    // Increment value for the time
    double indIncr = 0.1/(_wtLength * 0.1);
    
    for (unsigned int n = 0; n < _wtLength; n++)
    {
        if (amp > -1)
        {
            wt[n] = amp;
            
            // simply decrement
            amp -= ampIncr;
        }
        
        else
        {
            /*********************************************************************
             *
             * The smooth transition is nothing else than a function I found
             * empirically in graphing software (GeoGebra). There are actually
             * two functions. The first increments from -1 to 0 (amp) from 0.9
             * to 0.95 (time). This function is positive and right-shifted 0.9.
             * This first function meets the second function in (0.95,0), which
             * is basically the same function but with negative coefficient and
             * right shifted by 1. It is best to put these functions in graphing
             * software to get a clear picture. In any case, these functions are
             * changed between so that from 0.9 to 0.95 the curvature is negative
             * so the function is progressive and the second part has a positive
             * curvature and is degressive.
             *
             *********************************************************************/
            
            if (ind < 0.95)
                wt[n] = 400 * pow(ind - 0.9,2) - 1;
            
            else
                wt[n] = -400 * pow(ind - 1,2) + 1;
            
            ind += indIncr;
        }
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double* Wavetable::smoothSquare()
{
    double* wt = new double[_wtLength + 1];
    
    double ind = 0;
    
    double incr = 1.0 / _wtLength;
    
    float exp = 50;

    for (unsigned int n = 0; n < _wtLength; n++)
    {
        double val;
        
        // Change the function every pi/2 radians,
        // each is a simple function of 50th degree
        // which is just a very square looking
        // quadratic function, when interleaved
        // like this, there are no sharp transitions
        // any longer!
        
        if (ind < 0.25)
            val = pow(ind - 1, exp) - 1;
        
        else if (ind < 0.5)
            val = pow(ind + 0.5, exp) - 1;
        
        else if (ind < 0.75)
            val = -pow(ind - 1.5, exp) + 1;
        
        else
            val = -pow(ind, exp) + 1;
        
        wt[n] = val;
        
        if ( (ind += incr) >= _wtLength)
            ind -= _wtLength;
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double * Wavetable::directSquare()
{
    // the sample buffer
    double * wt = new double [_wtLength + 1];
    
    // time for one sample
    double sampleTime = 1.0 / _wtLength;
    
    // the midpoint of the period
    double mid = 0.5;
    
    double ind = 0;
    
    // fill the sample buffer
    for (int n = 0; n < _wtLength; n++)
    {
        wt[n] = (ind < mid) ? -1 : 1;
        
        if ( (ind += sampleTime) >= _wtLength)
            ind -= _wtLength;
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double * Wavetable::directSaw()
{
    // the sample buffer
    double * wt = new double [_wtLength];
    
    // how much we must decrement the count
    // by at each iteration
    // 2.0 because the range is from 1 to -1
    double incr = 2.0 / _wtLength;
    
    double ind = 1;
    
    for (int n = 0; n < _wtLength; n++)
    {
        wt[n] = ind;
        
        if ( (ind -= incr) <= -1)
            ind = 1;
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double* Wavetable::directTriangle()
{
    double* wt = new double[_wtLength + 1];
    
    double phase = 0;
    double phaseIncr = twoPI / _wtLength;
    
    // Basierend auf Pseudocode (vgl. Mitchell, 2008)
    
    double triValue;
    double twoDivPi = 2.0/PI;
    
    for (unsigned int n = 0; n < _wtLength; n++)
    {
        triValue = (phase * twoDivPi);
        
        if (phase < 0) triValue = 1.0 + triValue;
        
        else triValue = 1.0 - triValue;
        
        wt[n] = triValue;
        
        if ( (phase += phaseIncr) >= PI)
            phase -= twoPI;
        
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double* Wavetable::genNoise()
{
    double * wt = new double[_wtLength + 1];
    
    for (unsigned int n = 0; n < _wtLength; n++)
    {
        /*
         // We subtract half the maximum range from the generated number
         // to get a range between - RAND_MAX / 2 and + RAND_MAX / 2, then
         // we divide by that range to get a value between -1 and 1, thus
         // normalized.
         */
        
        double randHalf = RAND_MAX/2.0;
        
        double value =  (rand() - randHalf) / randHalf;
        
        wt[n] = value;
    }
    
    wt[_wtLength] = wt[0];
    
    return wt;
}

double * Wavetable::_readWavetable(const int waveNum)
{
    std::string fname = "/Users/petergoldsborough/Documents/vibe/Resources/Wavetables/" + _wtNames[waveNum] + ".vwt";
    
    VibeWTParser parser;
    
    return parser.readWT(fname);
}

Wavetable::~Wavetable()
{
     for (int i = 0; i < _tables.size(); i++)
         delete [] _tables[i];
}