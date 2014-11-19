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

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Global::init();
    
    const unsigned long len = Global::samplerate * 5;
    
    Operator a(WavetableDB::SINE);
    
    Operator b(WavetableDB::SINE);
    
    Operator c(WavetableDB::SINE);
    
    Operator d(WavetableDB::SINE);
    
    FM fm(&a, &b, &c, &d);
    
    a.setFrequencyOffset(1760);
    
    a.setLevel(1);
    
    b.setFrequencyOffset(440);
    
    b.setLevel(5);
    
    c.setFrequencyOffset(880);
    
    c.setLevel(2.5);
    
    d.setFrequencyOffset(440);
    
    d.setLevel(1);
    
    fm.setAlgorithm(0);
    
    fm.setActive(0, false);
    fm.setActive(1, false);
    fm.setActive(2, false);
    fm.setActive(3, true);
    
    Envelope env;
    
    env.setSegLevel(Envelope::DEL, 0);
    
    LFO lfo(WavetableDB::SINE);
    
    lfo.setFrequency(8);
    
    env.attachMod_Seg(Envelope::ATK, 0, &lfo);
    
    d.attachMod(Operator::LEVEL, &env);
    
    Filter filter(Filter::LOW_PASS, 400);
    
    Reverb reverb;
    
    reverb.setReverbTime(2);
    
    Mixer mixer(0,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick;
        
        tick = (i < Global::samplerate) ? fm.tick() : 0;
        
        tick = filter.process(tick);
        
        tick = reverb.process(tick);
        
        env.increment();
        
        lfo.increment();
        
        a.increment();
        
        b.increment();
        
        c.increment();
        
        d.increment();
        
        mixer.process(tick);
    }

    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}