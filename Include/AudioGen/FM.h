//
//  FM.h
//  Anthem
//
//  Created by Peter Goldsborough on 08/09/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__FM__
#define __Anthem__FM__

class FM
{
public:
    
    enum Algorithms
    {
        
    };
    
    FM();
    
    double tick();
    
    void setAlgorithm(const unsigned short& alg);
    
private:
    
    unsigned short _alg;
    
};

#endif /* defined(__Anthem__FM__) */
