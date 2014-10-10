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
    
    lfo.setRate(2);
    
    LFO lfo2;
    
    lfo2.setRate(2);

    Envelope env;
    
    env.setSegLen(Envelope::ATK, 1000);
    
    env.attachMod_Seg(Envelope::ATK, Envelope::SEG_LEVEL, &lfo);
    
    env.attachMod_Seg(Envelope::ATK, Envelope::SEG_LEVEL, &lfo2);
    
    env.setSidechain_Seg(Envelope::ATK, Envelope::SEG_LEVEL, 0, 1);
    
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