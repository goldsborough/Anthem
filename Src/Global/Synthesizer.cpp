//
//  Synthesizer.cpp
//  Anthem
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Synthesizer.h"
#include "Global.h"
#include "Wavetable.h"

WavetableDB wavetableDB;

Synthesizer::Synthesizer(unsigned int smplr, unsigned short wtLen)

{
    Global::init(smplr,wtLen);
    wavetableDB.init();
}




