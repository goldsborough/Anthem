#include "Mixer.h"
#include "Global.h"
#include "Operator.h"
#include "Oscillator.h"
#include "Util.h"
#include "Envelope.h"
#include "LFO.h"
#include "Wavetable.h"
#include "Filter.h"
#include "Delay.h"
#include "Sample.h"
#include "Effects.h"
#include "Noise.h"
#include "Crossfader.h"
#include "Macro.h"
#include "FM.h"
#include "Midi.h"

#include <iostream>

int main(int argc, const char * argv[])
{   
    clock_t t = clock();
    
    Global::init();
    
    const unsigned long len = Global::samplerate * 5;
    
    Operator op(WavetableDB::SINE);
    
    op.setMode(1);
    
    op.setLevel(1);
    
    op.setNote(48);
    
    Mixer mixer(0,1);
    
    Midi midi;
    
    while(!midi.hasMessage());
    
    std::cout << midi.getLastMessage().note << std::endl;
    
    op.setNote(midi.getLastMessage().note);
    
    double tick;
    
    for (int i = 0; i < len; ++i)
    {
        tick = op.tick();
        
        op.increment();
        
        mixer.process(tick);
    }

    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}