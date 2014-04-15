//
//  Crossfade.h
//  Vibe
//
//  Created by Peter Goldsborough on 15/04/14.
//  Copyright (c) 2014 Peter Goldsborough. All rights reserved.
//

#ifndef __Vibe__Crossfade__
#define __Vibe__Crossfade__

// Base class
// Others differ only in wavetable generation methods
class XFadeUnit
{
    
public:
    
    virtual void init(bool scale = false) = 0;
    
    void setValue(char value);
    
    float left() { return _data->left; }
    
    float right() { return _data->right; }
    
    virtual ~XFadeUnit() {}
    
protected:
    
    struct XFadeData
    {
        float left = 0.5;
        float right = 0.5;
    };
    
    XFadeData * _data = 0;
    
    XFadeData _table[201];
};

class XFadeSine : public XFadeUnit
{
    void init(bool scale = false);
};

class XFadeLinear : public XFadeUnit
{
    void init(bool scale = false);
};

class XFadeSqrt : public XFadeUnit
{
    void init(bool scale = false);
};

#endif /* defined(__Vibe__Crossfade__) */
