#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    /*
    for (std::size_t p = 2; p <= 64; p *= 2)
    {
        std::vector<Partial> partials(p);
        
        for (short q = 1; q <= p; ++q)
        {
            partials[q - 1].number = q;
            partials[q - 1].amp = -1.0/q;
        }
        
        Wavetable wavetable(partials.begin(),
                            partials.end(),
                            Global::wavetableLength);
        
        wavetableDatabase.writeWavetable("ramp_" + std::to_string(p), wavetable);
    }*/

    
    Anthem anthem;
    
    anthem.operators[Anthem::D].setWavetable(WavetableDatabase::RAMP_2);
    
    anthem.operators[Anthem::D].setActive(true);
    
    anthem.operators[Anthem::D].setLevel(1);
    
    
    /*
    anthem.operators[Anthem::C].setActive(true);
    
    anthem.operators[Anthem::C].setRatio(1);
    
    anthem.operators[Anthem::C].setLevel(1);
    
    */
     
    anthem.setNote(69, true);
    

    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    const unsigned long len = Global::samplerate * 2;
    
    while (anthem.getSampleCount() < len);
    
    anthem.mixer.saveRecording();
}
