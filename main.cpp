#include "Anthem.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    const unsigned long end = t + 10 * CLOCKS_PER_SEC;
    
    Global::init();
    
    Anthem anthem;
    
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE);
    
    anthem.operators[Anthem::A].setNote(48);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(1);
    
    anthem.operators[Anthem::A].attachMod(Operator::LEVEL, &anthem.lfos[Anthem::A]);
    
    anthem.operators[Anthem::A].setLevel(0.5);
    
    anthem.operators[Anthem::A].setModUnitDepth(Operator::LEVEL, 0, 0.5);
    
    anthem.lfos[Anthem::A].lfos(LFOUnit::A).setFrequency(4);
    
    anthem.mixer.setMasterAmp(0.5);
    
    anthem.mixer.startRecording();
    
    anthem.audio.start();
    
    while (clock() != end);
    
    anthem.mixer.saveRecording();
    
    //std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}