//
//  main.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"
#include "Global.h"
#include "Oscillator.h"
#include "Mixer.h"
#include "EnvSeg.h"
#include "Utils.h"
#include "Envelope.h"
#include "LFO.h"
#include <iostream>

int main(int argc, const char * argv[])
{
    clock_t t = clock();
    
    Synthesizer synth;
    
    double freq = 440;
    
    uint32_t len = global.samplerate * 5;
    
    Oscillator osc(Wavetable::Modes::SINE,freq);
    
    Mixer mixer(false,true);
    
    LFO lfo;
    
    lfo.setAmp(0.5);
    
    lfo.setRate(0, 60);
    
    for (int i = 0; i < len; i++)
    {
        mixer.processTick(osc.tick() * lfo.tick());
    }
    
    mixer.play();
    
    //while (global.getPassedTime(start) < 5);

    std::cout << "Total program duration: " << getPassedTime(t) << "\n";
}

