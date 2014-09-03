//
//  EffectUni.cpp
//  Anthem
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

double EffectUnit::_dryWet(const double& originalSample, const double& processedSample)
{
    double a= (originalSample * (1 - _dw)) + (processedSample * _dw);
    return a;
}