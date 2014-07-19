//
//  Filter.h
//  Vibe
//
//  Created by Peter Goldsborough on 13/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Filter__
#define __Vibe__Filter__

#include <vector>

class SampleBuffer;

typedef std::vector<unsigned short> ImpulseResponse;

class FIR_Filter
{
public:
    
    FIR_Filter(const ImpulseResponse& ir)
    : _ir(ir)
    { }
    
    void convolute(SampleBuffer& queue);
    
private:
    
    ImpulseResponse _ir;
};

#endif /* defined(__Vibe__Filter__) */
