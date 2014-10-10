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
    
    LFO lfo;
    
    lfo.setFreq(2);
    
    LFO lfo2;
    
    lfo2.setFreq(4);
    
    LFO lfo3;
    
    lfo3.setFreq(3);
    
    Envelope env;
    
    env.setSegLen(Envelope::ATK, 1000);
    
    env.attachMod_Seg(Envelope::ATK, Envelope::SEG_RATE, &lfo);
    
    env.attachMod_Seg(Envelope::ATK, Envelope::SEG_RATE, &lfo2);
    
    env.attachMod_Seg(Envelope::ATK, Envelope::SEG_RATE, &lfo3);
    
    env.setSidechain_Seg(Envelope::ATK, Envelope::SEG_RATE, 0, 2);
    
    env.setSidechain_Seg(Envelope::ATK, Envelope::SEG_RATE, 0, 1);
    
    op.attachMod(Operator::AMP, &env);
    
    Mixer mixer(0,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        mixer.process(tick);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}