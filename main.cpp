#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::D].setWavetable(WavetableDatabase::SAW_2);
    
    anthem.operators[Anthem::D].setActive(true);
    
    anthem.operators[Anthem::D].setLevel(1);
    
    
    //anthem.operators[Anthem::C].setActive(true);
    
    //anthem.operators[Anthem::C].setRatio(1);
    
    //anthem.operators[Anthem::C].setLevel(-1);
    
    //anthem.operators[Anthem::C].setPhaseOffset(180);
    
     
    anthem.setNote(69, true);
    

    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    const unsigned long len = Global::samplerate * 1;
    
    while (anthem.getSampleCount() < len);
    
    anthem.mixer.saveRecording();
    
}
