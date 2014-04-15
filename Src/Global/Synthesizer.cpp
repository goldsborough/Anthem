//
//  Synthesizer.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"

Global global;
Wavetable wavetable;

Synthesizer::Synthesizer(unsigned int smplr, unsigned short wtLen)

{
    global.Init(smplr,wtLen);
    wavetable.Init(wtLen);
}




