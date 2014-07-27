//
//  Delay.h
//  Vibe
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Delay__
#define __Vibe__Delay__

#include "EffectUnit.h"

#include <deque>

class Delay : public EffectUnit
{
    
public:
    
    Delay(const double& delayTime = 0.5);
    
    void process(double& sample);
    
private:
    
    std::deque<double> _buffer;
};

#endif /* defined(__Vibe__Delay__) */
