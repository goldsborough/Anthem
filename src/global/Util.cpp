/********************************************************************************************//*!
*
*  @file        Util.cpp
*
*  @author      Peter Goldsborough
*
*  @date        29/11/2014
*
************************************************************************************************/

#include "Util.hpp"
#include <cmath>
#include <string>

namespace Util
{
    std::string getDate()
    {
        char date[100];
        
        time_t now = time(0);
        
        tm * ltm = localtime(&now);
        
        strftime(date, 100, "%d %m %Y", ltm);
        
        return date;
    }
    
    void round(double& val, unsigned int bins)
    {
        // the rounding factor
        double factor = 2.0 / bins;
        
        double n = val / factor;
        
        int nFloor = static_cast<int>(n);
        
        // If the absolute difference between the division
        // and the floored value is greater than 0.5, round
        // to the next whole factor else to the factor times
        // the floored value
        
        double fractional = n - nFloor;
        
        // Absolute value
        if (fractional < 0) fractional = -fractional;
        
        if (fractional >= 0.5)
        {
            // Next value depends on sign
            double next = (n > 0) ? 1 : -1;
            
            val = (nFloor + next) * factor;
        }
        
        else val = nFloor * factor;
    }

    double noteToFreq(unsigned short note)
    {
        // Source: http://en.wikipedia.org/wiki/MIDI_Tuning_Standard
        
        double exp = (note - 69) / 12.0;
        
        return pow(2, exp) * 440;
    }

    unsigned short freqToNote(double freq)
    {
        return 12 * log2(freq / 440.0) + 69;
    }

    double semitonesToFreq(double baseFreq, double semitoneOffset)
    {
        return pow(2, semitoneOffset / 12.0) * baseFreq;
    }
    
    double freqToSemitones(double baseFreq, double newFreq)
    {
        if (! baseFreq) return 0;
        
        return (12 * (log(newFreq/baseFreq))) / log(2);
    }

    float getPassedTime(clock_t start)
    {
        return (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
    }
    
    double dbToAmp(double baseAmp, double dB)
    {
        // 0 gain means no change
        return (dB) ? baseAmp * pow(10, (dB/20.0)) : baseAmp;
    }
    
    std::string checkFileName(std::string fname, const std::string& fileEnding)
    {
        // Get the date if no file name
        if (fname.empty()) { fname = Util::getDate(); }
        
        std::string::size_type ext_ind = fname.find('.'); // extension index
        
        std::string::size_type end = fname.size();
        
        // If the file name already ends with .wav, substitute spaces only up the dot
        if (ext_ind != std::string::npos && fname.substr(ext_ind,4) == fileEnding)
        { end = ext_ind; }
        
        // if no extension, add .wav
        else fname += fileEnding;
        
        for (std::string::size_type n = 0; n < end; n++)
        {
            // Replace anything non-alphanumeric
            if (! isalnum(fname[n]))
            { fname.replace(n, 1, "_"); }
        }
        
        fname.insert(0, "/Users/petergoldsborough/Documents/Anthem/wavefiles/");
        
        return fname;
    }
}