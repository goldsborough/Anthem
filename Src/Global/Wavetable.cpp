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

#include <string>
#include <fstream>
#include <cmath>

template <class T>
LookupTable<T>::LookupTable(T * ptr, size_t size)
{
    _data = ptr;
    _size = size;
}

template <class T>
LookupTable<T>::LookupTable(const LookupTable<T>& other)
{
    if (&other != this)
    {
        _size = other._size; // use other's size
        
        // fill data with other's data
        for (size_t i = 0; i < _size; ++i)
        { _data[i] = other._data[i]; }
    }
}

template <class T>
T LookupTable<T>::operator[] (const size_t ind) const
{
    if (! _data)
        throw TableUninitializedError();
    
    if (ind >= _size)
        throw TableLengthError();
    
    return _data[ind];
}

template <class T>
LookupTable<T>& LookupTable<T>::operator= (const LookupTable<T>&  other)
{
    if (&other != this)
    {
        _size = other._size;
        
        // get rid of current data
        delete [] _data;
        
        // copy other's data
        for (size_t i = 0; i < _size; ++i)
        { _data[i] = other._wt[i]; }
    }
    
    return *this;
}

template <class T>
T LookupTable<T>::interpolate(const double ind) const
{
    if (! _data)
        throw TableUninitializedError();
    
    if (ind >= _size)
        throw TableLengthError();
    
    int indexBase = (int) ind;         // The truncated integer part
    double indexFract = ind - indexBase;    // The remaining fractional part
    
    // grab the two items in-between which the actual value lies
    double value1 = _data[indexBase];
    double value2 = _data[indexBase+1];
    
    // interpolate
    double value = value1 + ((value2 - value1) * indexFract);
    
    return value;
}


void round(double& val, unsigned int bitWidth)
{
    // the rounding factor
    double factor = 1.0 / bitWidth;

    double n = val / factor;
    
    int nFloor = (int) n;
    
    // if the division is greater 0.5, round to the next whole factor
    // else take the floor value
    
    if (n > 0)
    {
        if (n - nFloor >= 0.5)
            val = (nFloor + 1) * factor;
    }
    
    else if (n + nFloor <= -0.5)
        val = (nFloor - 1) * factor;
}


template <class PartItr>
Wavetable::Wavetable(PartItr start, PartItr end, size_t wtLen, double masterAmp,
                     bool sigmaAprox, unsigned int bitWidth)

: _refptr(new size_t(1))
{
    // calculate number of partials
    size_t partNum = end - start;
    
    _data = new double [wtLen + 1];
    
    double * amp = new double [partNum];        // the amplitudes
    double * phase = new double [partNum];      // the current phase
    double * phaseIncr = new double [partNum];  // the phase increments
    
    /**********************************************************
     *
     *  The Lanczos sigma constant, aka sigma approximation,
     *  is a method of minimizing the effect of the Gibbs
     *  phenomenon, which leads to ripples and horns towards the
     *  ends of additively synthesized waveforms. It is defined
     *  as:
     *
     *  s = sin(x) / x
     *
     *  Where x is:
     *
     *  x = nπ / M
     *
     *  M being the total number of partials and n the current
     *  partial number (the fundamental frequency is seen as
     *  the first partial). π / M can be calculated
     *  loop-invariantly and is then mulitplied by each partial
     *  number, respectively.
     *
     **********************************************************/
    
    // constant sigma constant part
    double sigmaK = PI / partNum;
    
    // variable part
    double sigmaV;
    
    // convert the bit number to decimal
    bitWidth = pow(2, bitWidth);
    
    // the fundamental increment of one period
    // in radians
    static double fundIncr = twoPI / wtLen;
    
    // fill the arrays with the respective partial values
    for (size_t p = 0; start != end; ++p, ++start)
    {
        // initial phase
        phase[p] = start->phaseOffs;
        
        // fundIncr is two π / tablelength
        phaseIncr[p] = fundIncr * start->num;
        
        // reduce amplitude if necessary
        amp[p] = start->amp * masterAmp;
        
        // apply sigma approximation conditionally
        if (sigmaAprox)
        {
            // following the formula
            sigmaV = sigmaK * start->num;
            
            amp[p] *= sin(sigmaV) / sigmaV;
        }
    }
    
    // fill the wavetable
    for (unsigned int n = 0; n < wtLen; n++)
    {
        double value = 0;
        
        // do additive magic
        for (unsigned short p = 0; p < partNum; p++)
        {
            value += sin(phase[p]) * amp[p];
            
            phase[p] += phaseIncr[p];
            
            if (phase[p] >= twoPI)
                phase[p] -= twoPI;
        }
        
        // round if necessary
        if (bitWidth < 65536)
            round(value, bitWidth);
        
        _data[n] = value;
    }
    
    // Append the last item for interpolation
    _data[wtLen] = _data[0];
    
    delete [] phase;
    delete [] phaseIncr;
    delete [] amp;
}

Wavetable::Wavetable(const Wavetable& other)
{
    if (&other != this)
    {
        // copy data
        _data = other._data;
        _size = other._size;
        
        _refptr = other._refptr;
        
        // now one more instance is pointing to
        // the same data
        ++(*_refptr);
    }
}

Wavetable& Wavetable::operator=(const Wavetable &other)
{
    if (&other != this)
    {
        // delete current data if this is the last
        // instance pointing to this data
        if (! --(*_refptr))
        {
            delete [] _data;
            delete _refptr;
        }
        
        // copy other's data
        _data = other._data;
        _size = other._size;
        
        _refptr = other._refptr;
        
        // one more pointing to this data now
        ++(*_refptr);
    }
    
    return *this;
}

Wavetable::~Wavetable()
{
    // disable base destructor
    _baseDestructorEnabled = false;
    
    if (--(*_refptr) == 0)
    {
        delete [] _data;
        delete _refptr;
    }
}

Wavetable& Wavetable::makeUnique()
{
    if ((*_refptr) > 1)
    {
        --(*_refptr);
        
        _refptr = new size_t(1);
        
        double * temp = _data;
        
        _data = new double [_size];
        
        for (size_t i = 0; i < _size; ++i) _data[i] = temp[i];
    }
    
    return *this;
}

void WavetableDB::Init(unsigned int wtLen)
{
    _wtLength = wtLen;
    _fundIncr = twoPI / _wtLength;
    
    TextParser textParser("/Users/petergoldsborough/Documents/vibe/Resources/Wavetables/wavetables.txt");
    
    VibeWTParser wtParser;
    
    std::string fname;
    
    std::vector<std::string> names = textParser.readAllItems();
    
    for (int i = 0; i < names.size(); ++i)
    {
        fname = "/Users/petergoldsborough/Documents/vibe/Resources/Wavetables/" + names[i] + ".vwt";
        
        _tables.push_back(wtParser.readWT(fname));
    }
}
Wavetable& WavetableDB::getWaveform(const int mode)
{
    if (mode == NONE)
        return _noneTable;
    
    if (mode < 0 || mode >= _tables.size())
        throw std::out_of_range("Mode out of range");
    
    return _tables[mode];
}

Wavetable WavetableDB::smoothSaw()
{
    double* wt = new double[_wtLength + 1];
    
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // The first part is calculated linearly so that the
    // amplitude simply decrements from 1 to -1. This is
    // measured in amplitude
    
    double amp = 1;
    
    // The second part is measured in time, going from 0.9
    // to 1 (of the WavetableDB period)
    
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
            /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
             *
             * The smooth transition is nothing else than a function I found
             * empirically in graphing software (GeoGebra). There are actually
             * two functions. The first increments from -1 to 0 (amp) from 0.9
             * to 0.95 (time). This function is positive and right-shifted by 0.9
             * This first function intersects the second function in (0.95,0) which
             * is basically the same function but with negative coefficient and
             * right shifted by 1. It is best to put these functions in graphing
             * software to get a clear picture. In any case, these functions are
             * changed between so that from 0.9 to 0.95 the curvature is negative
             * so the function is progressive and the second part has a positive
             * curvature and is degressive.
             *
             * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
            
            if (ind < 0.95)
                wt[n] = 400 * pow(ind - 0.9,2) - 1;
            
            else
                wt[n] = -400 * pow(ind - 1,2) + 1;
            
            ind += indIncr;
        }
    }
    
    wt[_wtLength] = wt[0];
    
    return Wavetable(wt,_wtLength);
}

Wavetable WavetableDB::smoothSquare()
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
    
    return Wavetable(wt,_wtLength);
}

Wavetable WavetableDB::directSquare()
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
    
    return Wavetable(wt,_wtLength);
}

Wavetable WavetableDB::directSaw()
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
    
    return Wavetable(wt,_wtLength);
}

Wavetable WavetableDB::directTriangle()
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
    
    return Wavetable(wt,_wtLength);
}

Wavetable WavetableDB::genNoise()
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
    
    return Wavetable(wt,_wtLength);
}