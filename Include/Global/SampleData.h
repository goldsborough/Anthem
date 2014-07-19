//
//  SampleData.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__SampleData__
#define __Synth__SampleData__

#include <deque>

struct SampleData
{
    SampleData(const double& val = 0)
    : left(val), right(val)
    { }
    
    SampleData(double lf, double ri)
    : left(lf), right(ri)
    { }
    
    template <class T>
    SampleData& operator*= (T& value)
    {
        left *= value;
        right *= value;
        
        return *this;
    }
    
    double left;
    double right;
};

/* Deque instead of queue because you can traverse it */

class SampleBuffer
{
public:
    
    typedef std::deque<SampleData>::size_type size_type;
    
    // A deque/queue has no function to return the first element
    // and delete it from the queue. Only std::deque and std::list
    // have that. That's why this function pops and returns an item.
    SampleData getpop()
    {
        SampleData sampleD = _buffer.front();
        
        _buffer.pop_front();
        
        return sampleD;
    }
    
    SampleData& operator[] (size_type index) { return _buffer[index]; }
    const SampleData& operator[] (size_type index) const { return _buffer[index]; }
    
    void push(const SampleData& item) { _buffer.push_front(item); };
    
    void pop() { _buffer.pop_front(); };
    
    SampleData& front() { return _buffer.front(); };
    const SampleData& front() const { return _buffer.front(); };
    
    SampleData& back() { return _buffer.back(); };
    const SampleData& back() const { return _buffer.back(); };
    
    bool empty() const {return _buffer.empty(); };
    
    size_type size() const { return _buffer.size(); };
    
private:
    
    std::deque<SampleData> _buffer;
};


#endif /* defined(__Synth__SampleData__) */
