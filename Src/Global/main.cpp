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
    
    unsigned long len = Global::samplerate * 5;
    
    Operator op(WavetableDB::SINE);
    
    op.setNote(48);
    
    Operator op2(WavetableDB::SINE);
    
    op2.setNote(48);
    
    op2.setAmp(220);
    
    FM fm;
    
    fm.setOperator(1, &op);
    
    fm.setOperator(0, &op2);
    
    Mixer mixer(0,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick;
        
        tick = fm.tick();
        
        op.increment();
        
        op2.increment();
        
        mixer.process(tick);
    }

    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Execution time: " << Util::getPassedTime(t) << "\n";
    
}