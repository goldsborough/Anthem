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

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    uint32_t len = Global::samplerate * 5;
    
    //Operator op;
    
    //op.addNote(440);
    
    Noise op(Noise::WHITE);
    
    Filter filter(Filter::LOW_PASS,1000);
    
    LFO lfo;
    
    filter.attachMod(Filter::Q, 0, &lfo);
    
    filter.setDepth(Filter::Q, 0, 0.05);
    
    Mixer mixer(0,1);

    for (int i = 0; i < len; ++i)
    {
        if (i > Global::samplerate * 0.505)
        {
                
        }
        
        double tick = op.tick();
        
        tick = filter.process(tick);
        
        Sample sample(tick);
        
        mixer.process(sample);
    }
    
    mixer.play();
    
    //while (clock() != t + (5 * CLOCKS_PER_SEC));
    
    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
    
}