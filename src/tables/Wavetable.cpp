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
{
    // Perform FFT here
}

Wavetable::Wavetable(MathematicalWaveform waveform,
                     index_t length,
                     const std::string& id)
: LookupTable<double>(length, id)
{
    switch (waveform)
    {
        case MathematicalWaveform::DIRECT_SAW:
             _mathematicalSaw();
             break;
        
        case MathematicalWaveform::DIRECT_SQUARE:
             _mathematicalSquare();
             break;
            
        case MathematicalWaveform::DIRECT_TRIANGLE:
             _mathematicalTriangle();
             break;
            
        case MathematicalWaveform::SMOOTH_SAW:
             _smoothSaw();
             break;
            
        case MathematicalWaveform::SMOOTH_RAMP:
             _smoothRamp();
             break;
            
        case MathematicalWaveform::SMOOTH_SQUARE:
             _smoothSquare();
             break;
    }
}

void Wavetable::_smoothSaw()
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
    
    for (auto& sample : _data)
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
    
    _data.push_back(_data.front());
}

void Wavetable::_smoothRamp()
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
    
    for (auto& sample : _data)
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
    
    _data.push_back(_data.front());
}

void Wavetable::_smoothSquare()
{
    double value = 0;
    
    double incr = 1.0 / Global::wavetableLength;
    
    for (auto& sample : _data)
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
    
    _data.push_back(_data.front());
}

void Wavetable::_mathematicalSquare()
{   
    // time for one sample
    double sampleTime = 1.0 / Global::wavetableLength;
    
    // the midpoint of the period
    double mid = 0.5;
    
    double value = 0;
    
    // fill the sample buffer
    for (auto& sample : _data)
    {
        sample = (value < mid) ? -1 : 1;
        
        value += sampleTime;
    }
    
    _data.push_back(_data.front());
}

void Wavetable::_mathematicalSaw()
{
    // how much we must decrement the count
    // by at each iteration
    // 2.0 because the range is from 1 to -1
    double incr = 2.0 / Global::wavetableLength;
    
    double value = 1;
    
    for (auto& sample : _data)
    {
        sample = value;
        
        value -= incr;
    }
    
    _data.push_back(_data.front());
}

void Wavetable::_mathematicalTriangle()
{
    double value = -1;
    
    // 4.0 because we're incrementing/decrementing
    // half the period and the range is 2, so it's
    // actually 2 / wavetableLength / 2.
    double incr = 4.0 / Global::wavetableLength;
    
    // Boolean to indicate direction
    bool reachedMid = false;
    
    for (auto& sample : _data)
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
    
    _data.push_back(_data.front());
}

void WavetableDatabase::init()
{
    // The wavetable configuration file
    TextParsley textParser("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/wavetables.md");
    
    std::string fname;
    
    auto names = textParser.getAllWords();
    
    _tables.resize(names.size());
    
    // Fetch all wavetable names and read their respective data files
    for (index_t i = 0; i < names.size(); ++i)
    {
        // Read wavetables with i as their id and push them into the _tables vector.
        _tables[i] = std::make_shared<Wavetable>(_readWavetable(names[i]), Global::wavetableLength, names[i]);
    }
}

double* WavetableDatabase::_readWavetable(const std::string &name) const
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

WavetableDatabase::index_t WavetableDatabase::size() const
{
    return _tables.size();
}

std::shared_ptr<Wavetable>& WavetableDatabase::operator[](index_t wavetable)
{
    return _tables[wavetable];
}

const std::shared_ptr<Wavetable>& WavetableDatabase::operator[](index_t wavetable) const
{
    return _tables[wavetable];
}
