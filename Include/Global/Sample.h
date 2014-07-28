//
//  Sample.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__Sample__
#define __Synth__Sample__

#include <deque>

struct Sample
{
    Sample(const double& val = 0)
    : left(val), right(val)
    { }
    
    Sample(double lf, double ri)
    : left(lf), right(ri)
    { }
    
    Sample(const Sample& other)
    {
        this->left = other.left;
        this->right = other.right;
    }
    
    Sample& operator= (const double& value)
    {
        left = value;
        right = value;
        
        return *this;
    }
    
    Sample& operator*= (const double& value)
    {
        left *= value;
        right *= value;
        
        return *this;
    }
    
    Sample operator* (const double& value)
    {
        Sample ret(*this);
        
        return (ret *= value);
    }
    
    Sample& operator/= (const double& value)
    {
        left /= value;
        right /= value;
        
        return *this;
    }
    
    Sample operator/ (const double& value)
    {
        Sample ret(*this);
        
        return (ret /= value);
    }
    
    Sample& operator += (const double& value)
    {
        left += value;
        right += value;
        
        return *this;
    }
    
    Sample operator+ (const double& value)
    {
        Sample ret(*this);
        
        return (ret += value);
    }
    
    Sample& operator -= (const double& value)
    {
        left -= value;
        right -= value;
        
        return *this;
    }
    
    Sample operator- (const double& value)
    {
        Sample ret(*this);
        
        return (ret -= value);
    }
    
    double left;
    double right;
};

/* Deque instead of queue because you can traverse it */

class SampleBuffer
{
public:
    
    typedef std::deque<Sample>::size_type size_type;
    
    SampleBuffer(const size_type& size = 0)
    { _buffer.resize(size); }

    Sample getpop()
    {
        Sample sampleD = _buffer.front();
        
        _buffer.pop_front();
        
        return sampleD;
    }
    
    void resize(const size_type& newSize)
    { _buffer.resize(newSize); }
    
    Sample& operator[] (size_type index) { return _buffer[index]; }
    const Sample& operator[] (size_type index) const { return _buffer[index]; }
    
    void push(const Sample& item) { _buffer.push_back(item); };
    
    void pop() { _buffer.pop_front(); };
    
    Sample& front() { return _buffer.front(); };
    const Sample& front() const { return _buffer.front(); };
    
    Sample& back() { return _buffer.back(); };
    const Sample& back() const { return _buffer.back(); };
    
    bool empty() const {return _buffer.empty(); };
    
    size_type size() const { return _buffer.size(); };
    
private:
    
    std::deque<Sample> _buffer;
};


#endif /* defined(__Synth__Sample__) */
