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
    
    anthem.operators[Anthem::A].attachMod(Operator::LEVEL, &anthem.lfos[Anthem::A].lfos(0));
    
    anthem.lfos[Anthem::A].lfos(Anthem::A).setFrequency(2);
    
    anthem.operators[Anthem::A].setModUnitDepth(Operator::LEVEL, 0, 0.5);
    
    anthem.mixer.setMasterAmp(0.8);
    
    anthem.mixer.startRecording();
    
    anthem.audio.start();
    
    clock_t t = clock();
    
    while(clock() < t + (10 * CLOCKS_PER_SEC));
    
    anthem.mixer.saveRecording();
    
    //std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}