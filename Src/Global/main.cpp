//
//  main.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"
#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    double freq = 440;
    
    uint32_t len = global.samplerate * 5;
    
    Oscillator osc(Wavetable::Modes::SINE,freq);
    
    Mixer mixer(false,true);
    
    EnvSegSeq env(2);
    
    env.setSegEndLevel(0, 0.8);
    env.setSegStartLevel(1, 0.8);
    env.setSegEndLevel(1, 0.0);
    
    env.setSegLen(0, 500);
    env.setSegLen(1, 500);
    
    for (int i = 0; i < len; i++)
    {
        mixer.processTick(osc.tick() * env.tick());
    }
    
    mixer.play();
    
    //while (global.getPassedTime(start) < 5);

    std::cout << "Total program duration: " << getPassedTime(t) << "\n";
}

