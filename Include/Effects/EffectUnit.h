//
//  EffectUnit.h
//  Vibe
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef Vibe_EffectUnit_h
#define Vibe_EffectUnit_h

class EffectUnit
{
public:
    
    virtual ~EffectUnit() { }
    
    virtual void setDryWet(const double& dw)
    { _dw = dw; }
    
    virtual void process(double& sample) = 0;
    
protected:
    
    void _dryWet(double& originalSample, const double& processedSample)
    {
        double diff = processedSample - originalSample;
        
        originalSample += (diff * _dw);
    }
    
    double _dw = 1;
};


#endif
