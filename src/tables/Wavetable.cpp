/********************************************************************************************//*!
*
*  @file        Wavetable.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Wavetable.hpp"
#include "Parsley.hpp"

#include <fstream>
#include <cmath>

Wavetable::Wavetable(double * ptr,
                     index_t wtLength,
                     index_t id,
                     const std::string& name)
: LookupTable<double>(ptr, wtLength, id, name)
{ }

Wavetable::Wavetable(MathematicalWaveform waveform,
                     index_t wtLength,
                     index_t id,
                     const std::string& name)

: LookupTable<double>(0,wtLength,id, name)

{
    switch (waveform)
    {
        case MathematicalWaveform::DIRECT_SAW:
            data_ = directSaw_();
            break;
        
        case MathematicalWaveform::DIRECT_SQUARE:
            data_ = directSquare_();
            break;
            
        case MathematicalWaveform::DIRECT_TRIANGLE:
            data_ = directTriangle_();
            break;
            
        case MathematicalWaveform::SMOOTH_SAW:
            data_ = smoothSaw_();
            break;
            
        case MathematicalWaveform::SMOOTH_RAMP:
            data_ = smoothRamp_();
            break;
            
        case MathematicalWaveform::SMOOTH_SQUARE:
            data_ = smoothSquare_();
            break;
    }
}

double* Wavetable::smoothSaw_() const
{
    double* wt = new double[Global::wtLen + 1];
    
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // The first part is calculated linearly so that the
    // amplitude simply decrements from 1 to -1. This is
    // measured in amplitude
    
    double amp = 1;
    
    // The second part is measured in time, going from 0.9
    // to 1 (of the Wavetable period)
    
    double value = 0.9;
    
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
            
            if (value < 0.95)
            { wt[n] = 400 * pow(value - 1,2) - 1; }
            
            else
            { wt[n] = -400 * pow(value - 0.9,2) + 1; }
            
            value += indIncr;
        }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::smoothRamp_() const
{
    double* wt = new double[Global::wtLen + 1];
    
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // The first part is calculated linearly so that the
    // amplitude simply decrements from 1 to -1. This is
    // measured in amplitude
    
    double amp = -1;
    
    // The second part is measured in time, going from 0.9
    // to 1 (of the Wavetable period)
    
    double value = 0.9;
    
    // Increment value from -1 to 1
    double ampIncr = 2.0/(Global::wtLen * 0.9);
    
    // Increment value for the time
    double indIncr = 0.1/(Global::wtLen * 0.1);
    
    for (unsigned int n = 0; n < Global::wtLen; n++)
    {
        if (amp < 1)
        {
            wt[n] = amp;
            
            amp += ampIncr;
        }
        
        else
        {
            if (value < 0.95)
            { wt[n] = -400 * pow(value - 0.9,2) + 1; }
            
            else
            { wt[n] = 400 * pow(value - 1,2) - 1; }
            
            value += indIncr;
        }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::smoothSquare_() const
{
    double* wt = new double[Global::wtLen + 1];
    
    double value = 0;
    
    double incr = 1.0 / Global::wtLen;
    
    for (unsigned int n = 0; n < Global::wtLen; n++)
    {
        double val;
        
        // Change the function every pi/2 radians,
        // each is a simple function of 50th degree
        // which is just a very square looking
        // quadratic function, when interleaved
        // like this, there are no sharp transitions
        // any longer!
        
        if (value < 0.25)
        { val = pow(value - 1, 50) - 1; }
        
        else if (value < 0.5)
        { val = pow(value + 0.5, 50) - 1; }
        
        else if (value < 0.75)
        { val = -pow(value - 1.5, 50) + 1; }
        
        else
        { val = -pow(value, 50) + 1; }
        
        wt[n] = val;
        
        value += incr;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::directSquare_() const
{
    // the sample buffer
    double * wt = new double [Global::wtLen + 1];
    
    // time for one sample
    double sampleTime = 1.0 / Global::wtLen;
    
    // the midpoint of the period
    double mid = 0.5;
    
    double value = 0;
    
    // fill the sample buffer
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = (value < mid) ? -1 : 1;
        
        value += sampleTime;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::directSaw_() const
{
    // the sample buffer
    double * wt = new double [Global::wtLen];
    
    // how much we must decrement the count
    // by at each iteration
    // 2.0 because the range is from 1 to -1
    double incr = 2.0 / Global::wtLen;
    
    double value = 1;
    
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = value;
        
        value -= incr;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::directTriangle_() const
{
    double* wt = new double[Global::wtLen + 1];
    
    double value = -1;
    
    // 4.0 because we're incrementing/decrementing
    // half the period and the range is 2, so it's
    // actually 2 / wtLen / 2.
    double incr = 4.0 / Global::wtLen;
    
    // Boolean to indicate direction
    bool reachedMid = false;
    
    for (unsigned int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = value;
        
        // Increment or decrement depending
        // on the current direction
        value += (reachedMid) ? -incr : incr;
        
        // Change direction every time
        // the value hits a maximum
        if (value >= 1 || value <= -1)
        { reachedMid = !reachedMid; }
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

void WavetableDatabase::init()
{
    // The wavetable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/wavetables.md");
    
    std::string fname;
    
    std::vector<std::string> names = textParser.getAllWords();
    
    tables_.resize(names.size());
    
    // Fetch all wavetable names and read their respective data files
    for (index_t i = 0; i < names.size(); ++i)
    {
        fname = "/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/" + names[i] + ".wavetable";
        
        // Read wavetables with i as their id and push them into the tables_ vector.
        tables_[i] = Wavetable(readWavetable(fname), Global::wtLen, i, names[i]);
    }
}

double* WavetableDatabase::readWavetable(const std::string &fname) const
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
    
    return wt;
}

void WavetableDatabase::writeWavetable(const std::string &fname, const Wavetable& wt) const
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

Wavetable::index_t WavetableDatabase::size() const
{
    return tables_.size();
}

Wavetable& WavetableDatabase::operator[](index_t wt)
{
    return tables_[wt];
}

const Wavetable& WavetableDatabase::operator[](index_t wt) const
{
    return tables_[wt];
}
