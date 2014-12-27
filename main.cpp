#include "Anthem.hpp"

#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(1);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setActive(true);
    
    anthem.flanger.setCenter(0.001);//2.0/48000);
    
    anthem.flanger.setDepth(0.001);
    
    anthem.flanger.setRate(0.15);
    
    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    clock_t t = clock();
    
    while(clock() < t + (10 * CLOCKS_PER_SEC));
    
    anthem.mixer.saveRecording();
}