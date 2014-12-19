#include "Anthem.hpp"

#include <iostream>

#include "Pantable.hpp"

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(0.5);
    
    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    anthem.operators[Anthem::A].setFrequencyOffset(1);
    
    anthem.operators[Anthem::A].setLevel(2.8);
    
    anthem.mixer.setPanType(PantableDatabase::LINEAR);
    
    anthem.mixer.setMasterAmp(1);
    
    clock_t t = clock();
    
    while(clock() < t + (1 * CLOCKS_PER_SEC));
    
    anthem.mixer.saveRecording();
    
}