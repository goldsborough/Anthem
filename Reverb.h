//
//  Reverb.h
//  Vibe
//
//  Created by Peter Goldsborough on 30/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Reverb__
#define __Vibe__Reverb__

#include "EffectUnit.h"

class Delay;
class AllPassDelay;

class Reverb : public EffectUnit
{
public:
    Reverb(const double& reverbTime,
           const double& reverbRate = 0.001,
           const double& dryWet = 0.1);
    
    ~Reverb();
    
    double process(const double& sample);
    
    void setReverbTime(const double& reverbTime);
    
    void setReverbRate(const double& reverbRate);
    
    void setDryWet(const double& dw);
    
private:

    double _attenuation;
    
    Delay** _delays;
    AllPassDelay** _allPasses;
    
};

#endif /* defined(__Vibe__Reverb__) */
