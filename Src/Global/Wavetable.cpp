//
//  Wavetable.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 22/03/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Wavetable.hpp"
#include "Global.hpp"
#include "Parsley.hpp"

#include <string>
#include <fstream>
#include <cmath>

void round(double& val, unsigned int bitWidth)
{
    // mind = blown
    
    // the rounding factor
    double factor = 1.0 / bitWidth;

    double n = val / factor;
    
    int nFloor = static_cast<int>(n);
    
    // if the division is greater 0.5, round to the next whole factor
    // else take the floor value
    
    if (n > 0)
    {
        if (n - nFloor >= 0.5)
        { val = (nFloor + 1) * factor; }
    }
    
    else if (n + nFloor <= -0.5)
    { val = (nFloor - 1) * factor; }
}

Wavetable::Wavetable(double * ptr,
                     size_t wtLength,
                     size_t id)
: data_(ptr), size_(wtLength),
  refptr_(new size_t(1)), id_(id)
{ }

template <class PartItr>
Wavetable::Wavetable(PartItr start,
                     PartItr end,
                     size_t wtLength,
                     double masterAmp,
                     bool sigmaAprox,
                     unsigned int bitWidth,
                     size_t id)

: refptr_(new size_t(1)), id_(id)
{
    // calculate number of partials
    size_t partNum = end - start;
    
    data_ = new double [wtLength + 1];
    
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
    double sigmaK = Global::pi / partNum;
    
    // variable part
    double sigmaV;
    
    // convert the bit number to decimal
    bitWidth = pow(2, bitWidth);
    
    // the fundamental increment of one period
    // in radians
    static double fundIncr = Global::twoPi / wtLength;
    
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
    for (unsigned int n = 0; n < wtLength; n++)
    {
        double value = 0;
        
        // do additive magic
        for (unsigned short p = 0; p < partNum; p++)
        {
            value += sin(phase[p]) * amp[p];
            
            phase[p] += phaseIncr[p];
            
            if (phase[p] >= Global::twoPi)
                phase[p] -= Global::twoPi;
        }
        
        // round if necessary
        if (bitWidth < 65536)
            round(value, bitWidth);
        
        data_[n] = value;
    }
    
    // Append the last item for interpolation
    data_[wtLength] = data_[0];
    
    delete [] phase;
    delete [] phaseIncr;
    delete [] amp;
}

Wavetable::Wavetable(const Wavetable& other)
{
    if (&other != this)
    {
        // copy data
        this->data_ = other.data_;
        this->size_ = other.size_;
        this->id_ = other.id_;
        
        this->refptr_ = other.refptr_;
        
        // now one more instance is pointing to
        // the same data
        ++(*refptr_);
    }
}

Wavetable& Wavetable::operator=(const Wavetable &other)
{
    if (&other != this)
    {
        // delete current data if this is the last
        // instance pointing to this data
        if (! --(*refptr_))
        {
            delete [] data_;
            delete refptr_;
        }
        
        // copy other's data
        this->data_ = other.data_;
        this->size_ = other.size_;
        this->id_ = other.id_;
        
        this->refptr_ = other.refptr_;
        
        // one more pointing to this data now
        ++(*refptr_);
    }
    
    return *this;
}

double& Wavetable::operator[] (size_t ind)
{
    // Make this object's data unique as it might
    // have to be changed
    makeUnique();
    
    return data_[ind];
}

double Wavetable::interpolate(double ind) const
{
    int indexBase = static_cast<int>(ind);  // The truncated integer part
    double indexFract = ind - indexBase;    // The remaining fractional part
    
    // grab the two items in-between which the actual value lies
    double value1 = data_[indexBase];
    double value2 = data_[indexBase+1];
    
    // interpolate: integer part + (fractional part * difference between value2 and value1)
    double final = value1 + ((value2 - value1) * indexFract);
    
    return final;
}

const double& Wavetable::operator[] (size_t ind) const
{
    return data_[ind];
}

size_t Wavetable::size() const
{
    return size_;
}

double* Wavetable::get() const
{
    return data_;
}

size_t Wavetable::id() const
{
    return id_;
}

Wavetable::~Wavetable()
{
    if (! --(*refptr_))
    {
        delete [] data_;
        delete refptr_;
    }
}

Wavetable& Wavetable::makeUnique()
{
    if ((*refptr_) > 1)
    {
        --(*refptr_);
        
        refptr_ = new size_t(1);
        
        double* temp = data_;
        
        data_ = new double [size_];
        
        for (size_t i = 0; i < size_; ++i)
        { data_[i] = temp[i]; }
    }
    
    return *this;
}

void WavetableDB::init()
{
    // The wavetable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/wavetables.txt");
    
    std::string fname;
    
    std::vector<std::string> names = textParser.getAllWords();
    
    // Fetch all wavetable names and read their respective data files
    for (int i = 0; i < names.size(); ++i)
    {
        fname = "/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/" + names[i] + ".wavetable";
        
        // Read wavetables with i as their id and push them into the tables_ vector. 
        tables_.push_back(readWavetable(fname,i));
    }
    
    // For NONE
    tables_.push_back(Wavetable());
}

Wavetable WavetableDB::readWavetable(const std::string &fname, size_t id)
{
    std::ifstream file(fname);
    
    if (! file.is_open())
    { throw FileNotOpenError("Could not find wavetable file: " + fname); }
    
    if (! file.good())
    { throw FileOpenError("Error opening wavetable: " + fname); }
    
    char signature[6];
    
    file.read(signature, 6);
    
    if (strncmp(signature, "ANTHEM", 6))
    { throw ParseError("Invalid signature for Anthem file!"); }
    
    int len = Global::wtLen + 1;
    int size = len * sizeof(double);
    
    double * wt = new double [len];
    
    file.read(reinterpret_cast<char*>(wt), size);
    
    return Wavetable(wt,Global::wtLen,id);
}

void WavetableDB::writeWavetable(const std::string &fname, const Wavetable& wt)
{
    std::ofstream file(fname);
    
    if (! file.is_open())
    { throw FileNotOpenError(); }
    
    if (! file.good())
    { throw FileOpenError(); }
    
    file.write("ANTHEM", 6);
    
    int size = (Global::wtLen + 1) * sizeof(double);
    
    file.write(reinterpret_cast<char*>(wt.get()), size);
}

Wavetable& WavetableDB::operator[](short wt)
{
    if (wt == NONE)
    { return tables_[tables_.size() - 1]; }
        
    if (wt < NONE || wt >= tables_.size())
    { throw std::invalid_argument("Wavetable ID out of range!"); }
    
    return tables_[wt];
}

const Wavetable& WavetableDB::operator[](short wt) const
{
    if (wt == NONE)
    { return tables_[tables_.size() - 1]; }
    
    if (wt < NONE || wt >= tables_.size())
    { throw std::invalid_argument("Wavetable ID out of range!"); }
    
    return tables_[wt];
}

Wavetable WavetableDB::smoothSaw_()
{
    double* wt = new double[Global::wtLen + 1];
    
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
    double ampIncr = 2.0/(Global::wtLen * 0.9);
    
    // Increment value for the time
    double indIncr = 0.1/(Global::wtLen * 0.1);
    
    for (unsigned int n = 0; n < Global::wtLen; n++)
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
            { wt[n] = 400 * pow(ind - 0.9,2) - 1; }
            
            else
            { wt[n] = -400 * pow(ind - 1,2) + 1; }
            
            ind += indIncr;
        }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return Wavetable(wt,Global::wtLen);
}

Wavetable WavetableDB::smoothSquare_()
{
    double* wt = new double[Global::wtLen + 1];
    
    double ind = 0;
    
    double incr = 1.0 / Global::wtLen;
    
    float exp = 50;

    for (unsigned int n = 0; n < Global::wtLen; n++)
    {
        double val;
        
        // Change the function every pi/2 radians,
        // each is a simple function of 50th degree
        // which is just a very square looking
        // quadratic function, when interleaved
        // like this, there are no sharp transitions
        // any longer!
        
        if (ind < 0.25)
        { val = pow(ind - 1, exp) - 1; }
        
        else if (ind < 0.5)
        { val = pow(ind + 0.5, exp) - 1; }
        
        else if (ind < 0.75)
        { val = -pow(ind - 1.5, exp) + 1; }
        
        else
        { val = -pow(ind, exp) + 1; }
        
        wt[n] = val;
        
        if ( (ind += incr) >= Global::wtLen)
        { ind -= Global::wtLen; }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return Wavetable(wt,Global::wtLen);
}

Wavetable WavetableDB::directSquare_()
{
    // the sample buffer
    double * wt = new double [Global::wtLen + 1];
    
    // time for one sample
    double sampleTime = 1.0 / Global::wtLen;
    
    // the midpoint of the period
    double mid = 0.5;
    
    double ind = 0;
    
    // fill the sample buffer
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = (ind < mid) ? -1 : 1;
        
        if ( (ind += sampleTime) >= Global::wtLen)
        { ind -= Global::wtLen; }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return Wavetable(wt,Global::wtLen);
}

Wavetable WavetableDB::directSaw_()
{
    // the sample buffer
    double * wt = new double [Global::wtLen];
    
    // how much we must decrement the count
    // by at each iteration
    // 2.0 because the range is from 1 to -1
    double incr = 2.0 / Global::wtLen;
    
    double ind = 1;
    
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = ind;
        
        if ( (ind -= incr) <= -1)
            ind = 1;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return Wavetable(wt,Global::wtLen);
}

Wavetable WavetableDB::directTriangle_()
{
    double* wt = new double[Global::wtLen + 1];
    
    double phase = 0;
    double phaseIncr = Global::twoPi / Global::wtLen;
    
    // Basierend auf Pseudocode (vgl. Mitchell, 2008)
    
    double triValue;
    double twoDivPi = 2.0/Global::pi;
    
    for (unsigned int n = 0; n < Global::wtLen; n++)
    {
        triValue = (phase * twoDivPi);
        
        if (phase < 0) triValue = 1.0 + triValue;
        
        else triValue = 1.0 - triValue;
        
        wt[n] = triValue;
        
        if ( (phase += phaseIncr) >= Global::pi)
            phase -= Global::twoPi;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return Wavetable(wt,Global::wtLen);
}