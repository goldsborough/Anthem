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
#include "Utils.h"
#include "Envelope.h"
#include "LFO.h"
#include "Wavetable.h"

#include <iostream>

int main(int argc, const char * argv[])
{
    
    clock_t t = clock();
    
    Synthesizer synth;
    
    double freq = 440;
    
    uint32_t len = Global::samplerate * 3;
    
    Operator op;
    
    op.addNote(freq);
    
    Mixer mixer(0,1);
    
    LFO lfo(WavetableDB::SAW_2);
    
   op.attachMod(Operator::FREQ_SEMI, 0, &lfo);
    
   op.setDepth(Operator::FREQ_SEMI, 0, 0.5);
    
    
    for (int i = 0; i < len; i++)
    {
        double t = op.tick();
        
        mixer.processTick(t);
    }
    
    mixer.play();
    
    //while (getPassedTime(start) < 5);

    std::cout << "Total program duration: " << getPassedTime(t) << "\n";
 
}

