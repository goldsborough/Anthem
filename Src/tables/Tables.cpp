#include "Tables.hpp"
#include "Wavetable.hpp"



namespace Tables
{
    WavetableDB wavetables;
    
    void init()
    {
        wavetables.init();
    }
};