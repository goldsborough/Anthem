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
    
    Delay delay(1.5);
    
    for (int i = 0; i < 10000; ++i)
    {
        mixer.processTick(0);
    }
    
    for (int i = 0; i < len - 10000; i++)
    {
        double t = op.tick();
        
        //delay.process(t);
        
        mixer.processTick(t);
    }
    
    mixer.play();
    
    //while (getPassedTime(start) < 5);

    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
 
}

