#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(0.5);
    
    //anthem.mixer.startRecording();
    
    anthem.audio.start();
    
    clock_t t = clock();
    
    while(1);//(clock() < t + (10 * CLOCKS_PER_SEC));
    
    //anthem.mixer.saveRecording();
    
    //std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}