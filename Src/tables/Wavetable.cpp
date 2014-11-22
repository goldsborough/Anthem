#include "Wavetable.hpp"
#include "Global.hpp"
#include "Util.hpp"
#include "Parsley.hpp"

#include <fstream>
#include <cmath>

Wavetable::Wavetable(double * ptr,
                     index_t wtLength,
                     index_t id,
                     const std::string& name)
: LookupTable<double>(ptr, wtLength, id, name)
{ }

template <class PartItr>
Wavetable::Wavetable(PartItr start,
                     PartItr end,
                     index_t wtLength,
                     double masterAmp,
                     bool sigmaAprox,
                     unsigned int bitWidth,
                     index_t id,
                     const std::string& name)

: LookupTable<double>(0, wtLength, id, name)

{
    // calculate number of partials
    index_t partNum = end - start;
    
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
    for (index_t p = 0; start != end; ++p, ++start)
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
        {
            Util::round(value, bitWidth);
        }
        
        data_[n] = value;
    }
    
    // Append the last item for interpolation
    data_[wtLength] = data_[0];
    
    delete [] phase;
    delete [] phaseIncr;
    delete [] amp;
}

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
    
    return wt;
}

double* Wavetable::smoothSquare_() const
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
    
    double ind = 0;
    
    // fill the sample buffer
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = (ind < mid) ? -1 : 1;
        
        if ( (ind += sampleTime) >= Global::wtLen)
        { ind -= Global::wtLen; }
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
    
    double ind = 1;
    
    for (int n = 0; n < Global::wtLen; n++)
    {
        wt[n] = ind;
        
        if ( (ind -= incr) <= -1)
            ind = 1;
    }
    
    wt[Global::wtLen] = wt[0];
    
    return wt;
}

double* Wavetable::directTriangle_() const
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
