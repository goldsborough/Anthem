//
//  SampleData.h
//  Synth
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Synth__SampleData__
#define __Synth__SampleData__

#include <queue>

struct SampleData
{
    SampleData(const double& val = 0)
    : left(val), right(val)
    {};
    
    SampleData(double lf, double ri)
    : left(lf), right(ri)
    {}
    
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

class SampleQueue
{
public:
    
    typedef std::queue<SampleData>::size_type size_type;
    
    SampleData getpop()
    {
        SampleData sampleD = _queue.front();
        
        _queue.pop();
        
        return sampleD;
    }
    
    void push(const SampleData& item) { _queue.push(item); };
    
    void pop() { _queue.pop(); };
    
    SampleData& front() { return _queue.front(); };
    const SampleData& front() const { return _queue.front(); };
    
    SampleData& back() { return _queue.back(); };
    const SampleData& back() const { return _queue.back(); };
    
    bool empty() {return _queue.empty(); };
    
    size_type size() const { return _queue.size(); };
    
private:
    
    std::queue<SampleData> _queue;
};


#endif /* defined(__Synth__SampleData__) */
