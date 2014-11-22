#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    const unsigned long end = t + 1 * CLOCKS_PER_SEC;
    
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(1);
    
    anthem.mixer.setMasterAmp(0.5);
    
    anthem.lfos[Anthem::A].lfos(LFOUnit::A).setWavetable(WavetableDatabase::SINE);
    
    anthem.lfos[Anthem::A].lfos(LFOUnit::A).setFrequency(2);
    
    anthem.mixer.attachMod(Mixer::PAN, &anthem.lfos[Anthem::A]);
    
    for (int i = 0; i < 4; ++i)
    {
    
        anthem.operators[i].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::A]);
        
        anthem.operators[i].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::B]);
        
        anthem.operators[i].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::C]);
        
        anthem.operators[i].attachMod(Operator::LEVEL, &anthem.envelopes[Anthem::D]);
        
    }
    
    anthem.mixer.startRecording();
    
    //anthem.audio.start();
    
    //while (clock() != end);
    
    for (int n = 0; n < 44100; ++n)
    {
        anthem.tick();
        
        anthem.increment();
    }
    
    anthem.mixer.saveRecording();
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}