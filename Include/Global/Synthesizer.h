//
//  Synthesizer.h
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Synthesizer__
#define __Synth__Synthesizer__

#include "Global.h"
#include "Oscillator.h"
#include "Mixer.h"
#include "EnvSeg.h"
#include "Utils.h"
#include "Envelope.h"

class Synthesizer
{
    
public:
    
    Synthesizer(unsigned int smplr = 44100, unsigned short wtLen = 4095);
    
    void process();
    
};

#endif /* defined(__Synth__Synthesizer__) */
