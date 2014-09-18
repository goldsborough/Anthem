//
//  main.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"
#include "Mixer.h"
#include "Global.h"
#include "Operator.h"
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
    
    uint32_t len = Global::samplerate * 5;
    
    Operator op;
    
    op.addNote(440);
    /*
    LFOUnit lfo;
    
    lfo.env.setSegLen(0, 1000);
    
    lfo.env.setOneShot(false);
    
    lfo.env.setLoopEnd(1);
    lfo.env.setLoopStart(0);
    
    lfo.env.setLoopInf();
    
    //op.attachMod(Operator::AMP, 0, &lfo);*/
    
    LFOUnit::Env env;
    
    env.setSegLen(0, 1000);
    env.setSegLen(1, 1000);
    
    env.setLoopStart(0);
    env.setLoopEnd(1);
    //env.setLoopInf();

    
    Mixer mixer(0,1);

    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        double envTick = env.tick();
        
        tick *= envTick;
        
        mixer.process(tick);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
    
}