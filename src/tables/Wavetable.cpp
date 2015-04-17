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

Wavetable::Wavetable(double* data,
                     index_t length,
                     const std::string& id)
: LookupTable<double>(data, length, id)
{ }

Wavetable::Wavetable(MathematicalWaveform waveform,
                     index_t length,
                     const std::string& id)
: LookupTable<double>(length, id)
{
    switch (waveform)
    {
        case MathematicalWaveform::DIRECT_SAW:
             mathematicalSaw_();
             break;
        
        case MathematicalWaveform::DIRECT_SQUARE:
             mathematicalSquare_();
             break;
            
        case MathematicalWaveform::DIRECT_TRIANGLE:
             mathematicalTriangle_();
             break;
            
        case MathematicalWaveform::SMOOTH_SAW:
             smoothSaw_();
             break;
            
        case MathematicalWaveform::SMOOTH_RAMP:
             smoothRamp_();
             break;
            
        case MathematicalWaveform::SMOOTH_SQUARE:
             smoothSquare_();
             break;
    }
}

void Wavetable::smoothSaw_() const
{
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // For the falling part
    double amplitude = 1;

    // For the rising part
    double value = 0.9;
    
    // Increment value from -1 to 1 (range = 2.0; 9/10 of time)
    double ampIncrement = 2.0/(Global::wavetableLength * 0.9);
    
    // Increment value for the time (1/10 of time)
    double valueIncrement = 0.1/(Global::wavetableLength * 0.1);
    
    for (auto& sample : *data_)
    {
        // The first part is calculated linearly so
        // that the amplitude simply decrements from
        // 1 to -1. This is measured in amplitude
        if (amplitude > -1)
        {
            // Set the wavetable sample
            sample = amplitude;
            
            // simply decrement
            amplitude -= ampIncrement;
        }
        
        else
        {
            /*********************************************************************
            *
            *   The smooth transition is nothing else than a function I found
            *   empirically in graphing software (GeoGebra). There are actually
            *   two functions. The first increments from -1 to 0 (amplitude) from
            *   0.9 to 0.95 (time). This function is positive and right-shifted
            *   by 0.9. This first function intersects the second function in
            *   (0.95,0) which is basically the same function but with negative
            *   right shifted by 1. It is best to put these functions in graphing
            *   coefficient and software to get a clear picture. In any case,
            *   these functions are changed between so that from 0.9 to 0.95 the
            *   curvature is negative so the function is progressive and the
            *   second part has  a positive curvature and is degressive.
            *
            ***********************************************************************/
            
            // First part
            if (value < 0.95)
            {
                // Set the wavetable sample
                sample = 400 * pow(value - 1,2) - 1;
            }
            
            // Second part
            else
            {
                // Set the wavetable sample
                sample = -400 * pow(value - 0.9,2) + 1;
            }
            
            value += valueIncrement;
        }
    }
    
    data_->push_back(*data_->begin());
}

void Wavetable::smoothRamp_() const
{
    // First decrement from 1 to -1 in 9/10 of the cycle,
    // then go back up smoothly the last 1/10 of the cycle
    
    // The first part is calculated linearly so that the
    // amplitude simply decrements from 1 to -1. This is
    // measured in amplitude
    
    double amplitude = -1;
    
    // The second part is measured in time, going from 0.9
    // to 1 (of the Wavetable period)
    
    double value = 0.9;
    
    // Increment value from -1 to 1
    double ampIncrement = 2.0/(Global::wavetableLength * 0.9);
    
    // Increment value for the time
    double valueIncrement = 0.1/(Global::wavetableLength * 0.1);
    
    for (auto& sample : *data_)
    {
        if (amplitude < 1)
        {
            sample = amplitude;
            
            amplitude += ampIncrement;
        }
        
        else
        {
            if (value < 0.95)
            { sample = -400 * pow(value - 0.9,2) + 1; }
            
            else
            { sample = 400 * pow(value - 1,2) - 1; }
            
            value += valueIncrement;
        }
    }
    
    data_->push_back(*data_->begin());
}

void Wavetable::smoothSquare_() const
{
    double value = 0;
    
    double incr = 1.0 / Global::wavetableLength;
    
    for (auto& sample : *data_)
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
        
        sample = val;
        
        value += incr;
    }
    
    data_->push_back(*data_->begin());
}

void Wavetable::mathematicalSquare_() const
{
    // the sample buffer
    double * wavetable = new double [Global::wavetableLength + 1];
    
    // time for one sample
    double sampleTime = 1.0 / Global::wavetableLength;
    
    // the midpoint of the period
    double mid = 0.5;
    
    double value = 0;
    
    // fill the sample buffer
    for (auto& sample : *data_)
    {
        sample = (value < mid) ? -1 : 1;
        
        value += sampleTime;
    }
    
    data_->push_back(*data_->begin());
}

void Wavetable::mathematicalSaw_() const
{
    // how much we must decrement the count
    // by at each iteration
    // 2.0 because the range is from 1 to -1
    double incr = 2.0 / Global::wavetableLength;
    
    double value = 1;
    
    for (auto& sample : *data_)
    {
        sample = value;
        
        value -= incr;
    }
    
    data_->push_back(*data_->begin());
}

void Wavetable::mathematicalTriangle_() const
{
    double value = -1;
    
    // 4.0 because we're incrementing/decrementing
    // half the period and the range is 2, so it's
    // actually 2 / wavetableLength / 2.
    double incr = 4.0 / Global::wavetableLength;
    
    // Boolean to indicate direction
    bool reachedMid = false;
    
    for (auto& sample : *data_)
    {
        sample = value;
        
        // Increment or decrement depending
        // on the current direction
        value += (reachedMid) ? -incr : incr;
        
        // Change direction every time
        // the value hits a maximum
        if (value >= 1 || value <= -1)
        { reachedMid = !reachedMid; }
    }
    
    data_->push_back(*data_->begin());
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
        // Read wavetables with i as their id and push them into the tables_ vector.
        tables_[i] = Wavetable(readWavetable_(names[i]), Global::wavetableLength, names[i]);
    }
}

double* WavetableDatabase::readWavetable_(const std::string &name) const
{
    std::ifstream file("../../../rsc/wavetables/" + name + ".wavetable");
    
    if (! file.good())
    {
        throw FileOpenError("Error opening wavetable: " + name);
    }
    
    char signature[6];
    
    file.read(signature, 6);
    
    if (strncmp(signature, "ANTHEM", 6))
    {
        throw ParseError("Invalid signature for Anthem file!");
    }
    
    int len = Global::wavetableLength + 1;
    
    int size = len * sizeof(double);
    
    double * wavetable = new double [len];
    
    file.read(reinterpret_cast<char*>(wavetable), size);
    
    return wavetable;
}

void WavetableDatabase::writeWavetable(const std::string &name,
                                       const Wavetable& wavetable,
                                       bool addToDefaults) const
{
    std::ofstream file("../../../rsc/wavetables/" + name + ".wavetable", std::ios::binary);
    
    if (! file.good())
    {
        throw FileOpenError();
    }
    
    file.write("ANTHEM", 6);
    
    int size = (Global::wavetableLength + 1) * sizeof(double);
    
    file.write(reinterpret_cast<const char*>(wavetable.data()), size);
    
    if (! file.good())
    { throw FileWriteError("Error writing to wavetable file!"); }
    
    if (addToDefaults)
    {
        file.close();
        
        file.open("../../../rsc/wavetables/wavetables.md", std::ios::app);
        
        if (! file.good())
        {
            throw FileOpenError("Error opening wavetable configuration file!");
        }
        
        file << name << "\n";
        
        if (! file.good())
        {
            throw FileWriteError("Error writing to wavetable configuration file!");
        }
    }
}

Wavetable::index_t WavetableDatabase::size() const
{
    return tables_.size();
}

Wavetable& WavetableDatabase::operator[](index_t wavetable)
{
    return tables_[wavetable];
}

const Wavetable& WavetableDatabase::operator[](index_t wavetable) const
{
    return tables_[wavetable];
}
