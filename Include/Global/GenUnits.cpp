//
//  GenUnits.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 19/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "GenUnits.h"
#include "Wavetable.h"

void AudioGenUnit::setWT(const int mode)
{
    _mode = mode;
    _WT = wavetable.getWaveform(_mode);
}

void AudioGenUnit::setWT(double* wt)
{
    _mode = Wavetable::USER;
    
    _WT = wt;
}