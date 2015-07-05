#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::D].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::D].setActive(true);
    
    anthem.operators[Anthem::D].setLevel(1);
    
    
    anthem.lfos[Anthem::A].setMode(LFOUnit::Mode::LFO);
    
    anthem.lfos[Anthem::A].setActive(true);
    
    anthem.lfos[Anthem::A].lfos(Anthem::A).setAmp(1);
    
    anthem.operators[Anthem::D].attachMod(Operator::LEVEL, &anthem.lfos[Anthem::A]);
    
    
    
    anthem.mixer.setMasterAmp(1);
     
    anthem.setNote(69, true);
    

    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    const unsigned long len = Global::samplerate * 3;
    
    while (anthem.getSampleCount() < len);
    
    anthem.mixer.saveRecording();
    
}
