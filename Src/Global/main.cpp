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
    
    op.setAmp(0.8);
    
    Echo echo(0.2,20,1);
    
    for (int i = 0; i < len; ++i)
    {
        double tick = op.tick();
        
        if (i < Global::samplerate) echo.process(tick);
        
        else
        {
            tick=0;
            echo.process(tick);
        }
        
        mixer.processTick(tick);
    }
    
    mixer.play();
    
    //while (getPassedTime(start) < 5);

    std::cout << "Total program duration: " << Util::getPassedTime(t) << "\n";
 
}

