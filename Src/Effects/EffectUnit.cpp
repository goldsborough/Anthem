//
//  EffectUni.cpp
//  Vibe
//
//  Created by Peter Goldsborough on 27/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#include "EffectUnit.h"
#include <stdexcept>

void EffectUnit::setDryWet(const double& dw)
{
    if (dw < 0 || dw > 1)
    { throw std::invalid_argument("Dry/wet control must be between 0 and 1!"); }
        
    _dw = dw;
}

void EffectUnit::_dryWet(double &originalSample, const double &processedSample)
{
    originalSample = (originalSample * (1 - _dw)) + (processedSample * _dw);
}