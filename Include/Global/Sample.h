//
//  Sample.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__Sample__
#define __Anthem__Sample__

#include <deque>

struct Sample
{
    Sample(double val = 0)
    : left(val), right(val)
    { }
    
    Sample(double lf, double ri)
    : left(lf), right(ri)
    { }
    
    Sample(const Sample& other)
    {
        if (&other != this)
        {
            this->left = other.left;
            this->right = other.right;
        }
    }
    
    Sample& operator= (double value)
    {
        left = value;
        right = value;
        
        return *this;
    }
    
    Sample& operator*= (double value)
    {
        left *= value;
        right *= value;
        
        return *this;
    }
    
    Sample operator* (double value)
    {
        Sample ret(*this);
        
        return (ret *= value);
    }
    
    Sample& operator/= (double value)
    {
        left /= value;
        right /= value;
        
        return *this;
    }
    
    Sample operator/ (double value)
    {
        Sample ret(*this);
        
        return (ret /= value);
    }
    
    Sample& operator += (double value)
    {
        left += value;
        right += value;
        
        return *this;
    }
    
    Sample operator+ (double value)
    {
        Sample ret(*this);
        
        return (ret += value);
    }
    
    Sample& operator -= (double value)
    {
        left -= value;
        right -= value;
        
        return *this;
    }
    
    Sample operator- (double value)
    {
        Sample ret(*this);
        
        return (ret -= value);
    }
    
    double left;
    double right;
};

/* Deque instead of queue because you can traverse it */

struct SampleBuffer
{
    SampleBuffer(unsigned long size = 0)
    : buffer(size)
    { }

    Sample getpop()
    {
        Sample sampleD = buffer.front();
        
        buffer.pop_front();
        
        return sampleD;
    }
 
    std::deque<Sample> buffer;
};


#endif /* defined(__Anthem__Sample__) */
