//
//  Delay.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Delay.h"
#include "Global.h"

Delay::Delay(const double& delayTime)
{
    _buffer.resize(delayTime * Global::samplerate);
}

void Delay::process(double& sample)
{
    _buffer.push_back(sample);
    
    sample = _buffer.front();
    
    _buffer.pop_front();
}