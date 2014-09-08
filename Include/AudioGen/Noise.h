//
//  Noise.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Noise__
#define __Anthem__Noise__

#include "GenUnits.h"

class Filter;

class Noise : public GenUnit
{
    
public:
    
    enum Type
    {
        WHITE,
        PINK,
        RED
    };
    
    Noise(const unsigned short& type = 0, const double& amp = 1);
    
    ~Noise();
    
    double tick();
    
    void setType(const unsigned short& type);
    
    void setAmp(const double& amp);
    
private:
    
    Filter* _filter;
    
    unsigned short _type;
    
    double _amp;
};

#endif /* defined(__Anthem__Noise__) */
