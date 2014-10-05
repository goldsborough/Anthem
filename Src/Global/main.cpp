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
#include <vector>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    uint32_t len = Global::samplerate * 5;
    
    Operator op(WavetableDB::SINE);
    
    op.addNote(440);
    
    LFO lfo;
    
    lfo.setRate(4);

    Envelope env;
    
    env.attachMod(Envelope::ATK, ModEnvSegSeq::SEG_LEVEL, &lfo);
    
    env.setModUnitDepth(Envelope::ATK, ModEnvSegSeq::SEG_LEVEL, 0, 0.4);
    
    op.attachMod(Operator::AMP, &env);

    Mixer mixer(0,1);

    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        mixer.process(tick);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
    
}