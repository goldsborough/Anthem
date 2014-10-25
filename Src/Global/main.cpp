#include "Synthesizer.h"
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

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    unsigned long len = Global::samplerate * 5;
    
    Operator op(WavetableDB::SINE);
    
    op.addNote(440);
    
    Noise noise;
    
    LFO lfo(WavetableDB::SINE);
    
    noise.attachMod(Noise::AMP, &lfo);
    
    noise.setAmp(0.5);
    
    noise.setModUnitDepth(Noise::AMP, 0, 0.5);
    
    Mixer mixer(0,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick = noise.tick();

        //op.increment();
        
        noise.increment();
        lfo.increment();
        
        mixer.process(tick);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}