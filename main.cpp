#include "Anthem.hpp"

#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
    Global::init();
    
    Anthem anthem;
    /*
    std::vector<Partial> vec;
    
    vec.push_back(Partial(1,1));
    
    Wavetable wt(vec.begin(), vec.end(),Global::wtLen,1,false,3);
    
    wavetableDatabase.writeWavetable("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables/sine_4.wavetable", wt);
    */
    /*
    anthem.operators[Anthem::A].setWavetable(WavetableDatabase::SINE_4);
    
    anthem.operators[Anthem::A].setMode(1);
    
    anthem.operators[Anthem::A].setLevel(0.5);
    
    
    
    anthem.operators[Anthem::A].setFrequencyOffset(100);
    
    
    anthem.operators[Anthem::A].setLevel(2);
    
    anthem.audio.start();
    
    anthem.mixer.startRecording();
    
    clock_t t = clock();
    
    while(clock() < t + (1 * CLOCKS_PER_SEC));
    
    anthem.mixer.saveRecording();
    */
}