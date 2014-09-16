//
//  LookupTable.h
//  Anthem
//
//  Created by Peter Goldsborough on 15/09/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Anthem__LookupTable__
#define __Anthem__LookupTable__

template <class T>
class LookupTable
{
public:

    typedef unsigned long size_t;
    
    LookupTable()
    : _data(0), _size(0)
    { }
    
    LookupTable(T * ptr, size_t size)
    : _data(ptr), _size(size)
    { }
    
    virtual ~LookupTable()
    {
        if (_baseDestructorEnabled)
        { delete [] _data; }
    }
    
    LookupTable(const LookupTable& other)
    {
        if (&other != this)
        {
            _size = other._size; // use other's size
            
            _data = new T[_size];
            
            // fill data with other's data
            for (size_t i = 0; i < _size; ++i)
            { _data[i] = other._data[i]; }
        }
    }
    
    virtual LookupTable& operator= (const LookupTable& other)
    {
        if (&other != this)
        {
            _size = other._size;
            
            // get rid of current data
            delete [] _data;
            
            // Allocate new memory, idiot
            _data = new T[_size];
            
            // copy other's data
            for (size_t i = 0; i < _size; ++i)
            { _data[i] = other._data[i]; }
        }
        
        return *this;
    }
    
    virtual T* get() const
    { return _data; }
    
    virtual void set(T* ptr, size_t size)
    { _data = ptr; _size = size; }
    
    virtual T& operator[] (size_t ind)
    { return _data[ind]; }
    
    virtual const T& operator[] (size_t ind) const
    { return _data[ind]; }
    
    virtual size_t size() const
    { return _size; }
    
protected:
    
    T * _data;
    
    size_t _size;
    
    bool _baseDestructorEnabled = true;
};


#endif /* defined(__Anthem__LookupTable__) */
