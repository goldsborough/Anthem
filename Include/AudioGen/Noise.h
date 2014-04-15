//
//  Noise.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Noise__
#define __Synth__Noise__

#include "GenUnits.h"

class Noise : GenUnit
{
    
public:
    
    enum Type
    {
        WHITE,
        PINK,
        BROWN
    };
    
    Noise(const Type& type) { _type = type; };
    
    void setType(const Type& type) { _type = type; };
    
    void setColor(const double& col) {_Color = col; };
    
    double tick();
    
private:
    
    double _ind = 0.0;
    
    double _Color = 1.0;
    
    Type _type;
    
    double * _WT = wavetable.getNoise();
    
};

#endif /* defined(__Synth__Noise__) */
