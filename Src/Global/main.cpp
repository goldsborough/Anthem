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
    
    LFOSeq seq;
    
    seq.setModWavetable(0, WavetableDB::SINE);
    
    //seq.setModFreq(0, 3);
    
    seq.setModDepth(0, 0.5);
    
    seq.setSegBothLevels(0, 0.5);
    
    op.attachMod(Operator::AMP, &seq);
    
    Mixer mixer(0,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        seq.increment();
        op.increment();
        
        mixer.process(tick);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}