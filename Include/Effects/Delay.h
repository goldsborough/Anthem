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

class Delay : public EffectUnit
{
    
public:
    
    typedef unsigned long size_t;
    typedef double* iterator;
    
    Delay(const double& maxDelay,
          const double& decayTime = 1,
          const double& feedbackLevel = 0);
    
    virtual ~Delay();
    
    virtual void process(double& sample);
    
    virtual void setDelayTime(const double& delayTime);
    
    virtual void setDecay(unsigned int decayTime);
    
    virtual void setFeedback(const double& feedbackLevel);
    
protected:
    
    iterator _end;
    iterator _curr;
    iterator _tap;
    
    double _decay;
    double _feedback;
    
    double * _buffer;
};

struct Echo : public Delay
{
    
    Echo(const double& maxTime,
         const double& decayTime = 1,
         const double& feedbackLevel = 0)
    : Delay(maxTime,decayTime,feedbackLevel)
    { }
    
    void process(double& sample);

};

#endif /* defined(__Vibe__Delay__) */
