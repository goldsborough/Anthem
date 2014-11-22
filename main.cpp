#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    const unsigned long end = t + 5 * CLOCKS_PER_SEC;
    
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDB::SINE);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(0.5);
    
    anthem.mixer.setMasterAmp(0.2);
    
    anthem.mixer.startRecording();
    
    while (clock() != end);
    
    anthem.mixer.saveRecording();
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}