//
//  main.cpp
//  Synth
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

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    double freq = 440;
    
    uint32_t len = Global::samplerate * 5;
    
    Operator op;
    
    op.addNote(freq);
    
    Mixer mixer(0,1);
    /*
    Delay one(0.0437,1.5,0.001);
    Delay two(0.0411,1.5,0.001);
    Delay three(0.0371,1.5,0.001);
    Delay four(0.0297,1.5,0.001);
    
    one.setDryWet(1);
    two.setDryWet(1);
    three.setDryWet(1);
    four.setDryWet(1);*/
    
    Delay a(0.2,2);
    
    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        if (i < (Global::samplerate * 3))
        {
            tick = a.process(tick);
        }
        
        else
        {
            tick = 0;
            
            tick = a.process(tick);
        }
        
        Sample sample(tick);
        
        mixer.process(sample);
    }
    
    mixer.play();
    
    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
    
}

