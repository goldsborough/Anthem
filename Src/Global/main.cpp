#include "Mixer.hpp"
#include "Global.hpp"
#include "Operator.hpp"
#include "Oscillator.hpp"
#include "Util.hpp"
#include "Envelope.hpp"
#include "LFO.hpp"
#include "Wavetable.hpp"
#include "Filter.hpp"
#include "Delay.hpp"
#include "Sample.hpp"
#include "Effects.hpp"
#include "Noise.hpp"
#include "Crossfader.hpp"
#include "Macro.hpp"
#include "FM.hpp"
#include "AudioOutput.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Global::init();
    
    const unsigned short seconds = 1;
    
    const unsigned long clocks = seconds * CLOCKS_PER_SEC;
    
    const unsigned long len = Global::samplerate * seconds;
    
    Operator op(WavetableDB::SINE);
    
    op.setMode(1);
    
    op.setLevel(1);
    
    op.setNote(48);
    
    //Mixer mixer(1,1);
    
    double tick;
    
    clock_t t = clock();
    
    AudioOutput audio;
    
    audio.osc = &op;
    
    audio.start();
    
    /*
    //mixer.start();
    
    for (int i = 0; i < len; ++i)
    {
        tick = op.tick();
        
        op.increment();
        
        //mixer.process(tick);
    }*/
    
    while (clock() != t + clocks);
    
    audio.stop();
    
    //mixer.stop();
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}