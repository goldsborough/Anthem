//
//  main.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"
#include "Wavetable.h"
#include "Mixer.h"
#include "Global.h"
#include "Oscillator.h"
#include "Wavefile.h"
#include "Utils.h"
#include "Parser.h"

#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    double freq = 440;
    
    uint32_t len = global.samplerate * 5;
    
    Oscillator osc(0,freq);
    
    Mixer mixer(false,true);

    for (int i = 0; i < len; i++)
    {
        double val = osc.tick();
        
        mixer.processTick(val);
    }
    
    mixer.play();
    
    //while (getPassedTime(start) < 5);

    std::cout << "Total program duration: " << getPassedTime(t) << "\n";
    
}

