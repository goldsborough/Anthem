//
//  EffectUnit.h
//  Anthem
//
//  Created by Peter Goldsborough on 26/07/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef Anthem_EffectUnit_h
#define Anthem_EffectUnit_h

class Sample;

class EffectUnit
{
public:
    
    EffectUnit(const double& dryWet = 1)
    : _dw(dryWet)
    { }
    
    virtual ~EffectUnit() { }
    
    virtual void setDryWet(const double& dw);
    
    /************************************************************************************************//*!
    *
    *  @brief       Processes a sample.
    *
    *  @param       sample The sample to process.
    *
    *  @return      The new sample.
    *
    *************************************************************************************************/
    
    virtual double process(double sample) = 0;
    
protected:
    
    double _dryWet(const double& originalSample, const double& processedSample);
    
    double _dw;
};

#endif
