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
// Others differ only in table generation methods
class XFadeUnit
{
    
public:

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
    
    static const double sqrt2;
    
    XFadeData * _data = 0;
    
    XFadeData _table[201];
};

struct XFadeSine : public XFadeUnit
{
    XFadeSine(bool scale = false, char initVal = 0);
};

struct XFadeLinear : public XFadeUnit
{
    XFadeLinear(char initVal = 0);
};

struct XFadeSqrt : public XFadeUnit
{
    XFadeSqrt(bool scale = false, char initVal = 0);
};

#endif /* defined(__Vibe__Crossfade__) */
