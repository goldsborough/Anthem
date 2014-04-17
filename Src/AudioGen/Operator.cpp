//
//  Operator.cpp
//  Synth
//
//  Created by Peter Goldsborough on 21/01/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "Operator.h"
#include "Envelope.h"
#include "Oscillator.h"
#include "Wavetable.h"

Operator::Operator(const Wavetable::Modes& md, const double& amp)
{    
    _amp = amp;
    
    _mode = md;
    //_WT = wavetable.getWaveform(_mode);
}

/*
double * Operator::tick()
{
    
    double * ret = new double [_voice_number];
    
    for (uint8_t i = 0, end = _voice_number; i < end; i++)
    {
        
        ret[i] = _voices[i]->tick() * ampEnv->tick();
    }
    
    return ret;
}
*/




